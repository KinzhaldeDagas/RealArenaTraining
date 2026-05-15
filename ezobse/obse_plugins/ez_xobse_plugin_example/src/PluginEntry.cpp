#include "DialogueExportCommand.h"
#include "Win32Util.h"

#include "obse/PluginAPI.h"
#include "obse/Utilities.h"

#include <Windows.h>
#include <ShlObj.h>

#include <string>
#include <vector>
#include <algorithm>

// =============================================================
// reVoice CSE OBSE Plugin
// Option B:
//   File -> Export -> reVoice CSV -> <Loaded Plugin>
// Output:
//   <OblivionRoot>\\csv_revoice\\<PluginName>.csv
// =============================================================

static OBSEInterface* g_obse = nullptr;
static PluginHandle   g_pluginHandle = kPluginHandle_Invalid;

static HWND  g_mainWnd = nullptr;
static WNDPROC g_oldWndProc = nullptr;

static HMENU g_reVoicePopup = nullptr; // popup inserted into File->Export

static constexpr UINT kCmdBase_ReVoiceMod = 0xB000; // 0xB000..0xB0FF reserved for mod-index selection

// TESDataHandler::Singleton (editor) from CSE/Core.cpp
static constexpr uintptr_t kTESDataHandler_Singleton = 0x00A0E064;
// TESDataHandler offsets (CSE/Core.h)
static constexpr ptrdiff_t kTESDataHandler_ActiveFile = 0x0E04;
static constexpr ptrdiff_t kTESDataHandler_FileCount  = 0x0E10;
static constexpr ptrdiff_t kTESDataHandler_FilesByID  = 0x0E14; // TESFile* filesByID[0xFF]

// TESFile fields (CSE/TESFile.h)
// NOTE: fileName is an inline char[260], not a char*.
static constexpr ptrdiff_t kTESFile_FileName   = 0x001C; // char[260]
static constexpr ptrdiff_t kTESFile_FileFlags  = 0x03DC; // UInt32
static constexpr ptrdiff_t kTESFile_FileIndex  = 0x0400; // UInt8

static constexpr UINT_PTR kTimer_MenuInstall = 0xBEEF;
static constexpr UINT     kTimer_IntervalMs = 50;
static constexpr UINT     kTimer_MaxTries   = 400; // ~20s

static UINT g_timerTries = 0;

static std::string WideToUtf8(const std::wstring& w)
{
    if (w.empty())
        return {};
    int needed = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    if (needed <= 0)
        return {};
    std::string out;
    out.resize((size_t)needed);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), out.data(), needed, nullptr, nullptr);
    return out;
}

static std::wstring GetDefaultLogPathW()
{
    // Prefer My Documents\My Games\Oblivion\OBSE\...
    wchar_t docs[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_MYDOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, docs)) && docs[0])
    {
        std::wstring p = docs;
        p += L"\\My Games\\Oblivion\\OBSE\\reVoice_cse_export.log";
        return p;
    }

    // Fallback: next to the executable.
    return win32::GetExeDir() + L"\\OBSE\\reVoice_cse_export.log";
}

static void OpenPluginLogIfNeeded()
{
    static bool opened = false;
    if (opened)
        return;
    opened = true;

    std::wstring logPathW = GetDefaultLogPathW();
    win32::EnsureDirForFile(logPathW);
    std::string logPathA = WideToUtf8(logPathW);
    if (!logPathA.empty())
        gLog.Open(logPathA.c_str());
}

static bool ReadMem(const void* addr, void* out, size_t size)
{
    if (!addr || !out || size == 0) return false;
    SIZE_T br = 0;
    return ReadProcessMemory(GetCurrentProcess(), addr, out, size, &br) && br == size;
}

static bool ReadPtr(const void* addr, void*& out)
{
    out = nullptr;
    return ReadMem(addr, &out, sizeof(out));
}

static bool ReadU32(const void* addr, UInt32& out)
{
    out = 0;
    return ReadMem(addr, &out, sizeof(out));
}

static bool ReadCString(const char* p, std::string& out)
{
    out.clear();
    if (!p) return false;

    // Fast path: try a single ReadProcessMemory on a modest buffer.
    char buf[4096] = {};
    SIZE_T br = 0;
    if (!ReadProcessMemory(GetCurrentProcess(), p, buf, sizeof(buf) - 1, &br) || br == 0)
        return false;

    buf[sizeof(buf) - 1] = 0;
    out.assign(buf);
    const size_t z = out.find('\0');
    if (z != std::string::npos)
        out.resize(z);
    return true;
}

static std::wstring ToWide(const std::string& s)
{
    if (s.empty()) return {};
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (n <= 0) return {};
    std::wstring w;
    w.resize(static_cast<size_t>(n - 1));
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    return w;
}

static std::wstring StripExt(const std::wstring& s)
{
    const size_t pos = s.find_last_of(L".");
    if (pos == std::wstring::npos) return s;
    return s.substr(0, pos);
}

static std::wstring SanitizeFileStem(std::wstring s)
{
    for (auto& ch : s)
    {
        if (ch == L'<' || ch == L'>' || ch == L':' || ch == L'"' || ch == L'/' || ch == L'\\' || ch == L'|' || ch == L'?' || ch == L'*')
            ch = L'_';
    }
    if (s.empty()) s = L"plugin";
    return s;
}

static void* GetTESDataHandler()
{
    void* dh = nullptr;
    (void)ReadPtr(reinterpret_cast<const void*>(kTESDataHandler_Singleton), dh);
    return dh;
}

static std::wstring BuildDefaultOutPathForPlugin(const std::wstring& pluginName)
{
    std::wstring baseDir = win32::GetExeDir();
    if (!baseDir.empty() && (baseDir.back() == L'\\' || baseDir.back() == L'/'))
        baseDir.pop_back();

    std::wstring outDir = baseDir + L"\\csv_revoice";
    std::wstring stem = SanitizeFileStem(StripExt(pluginName));
    std::wstring outPath = outDir + L"\\" + stem + L".csv";
    return outPath;
}

// ------------------------------
// Menu discovery utilities
// ------------------------------

static std::wstring NormalizeMenuLabel(std::wstring s)
{
    // Drop accelerator markers '&' and trailing ellipsis, normalize whitespace.
    s.erase(std::remove(s.begin(), s.end(), L'&'), s.end());
    while (!s.empty() && (s.back() == L'.' || s.back() == L' ' || s.back() == L'\t'))
    {
        if (s.size() >= 3 && s.substr(s.size() - 3) == L"...")
        {
            s.resize(s.size() - 3);
            continue;
        }
        s.pop_back();
    }
    // Trim leading spaces
    while (!s.empty() && (s.front() == L' ' || s.front() == L'\t')) s.erase(s.begin());
    return s;
}

static bool GetMenuItemTextW(HMENU menu, UINT pos, std::wstring& out)
{
    out.clear();
    if (!menu) return false;
    wchar_t buf[512] = {};
    MENUITEMINFOW mii{};
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STRING;
    mii.dwTypeData = buf;
    mii.cch = (UINT)(std::size(buf) - 1);
    if (!GetMenuItemInfoW(menu, pos, TRUE, &mii))
        return false;
    out.assign(buf);
    return true;
}

static int FindTopMenuIndexByLabel(HMENU menuBar, const wchar_t* target)
{
    if (!menuBar || !target) return -1;
    const std::wstring want = NormalizeMenuLabel(target);
    const int n = GetMenuItemCount(menuBar);
    for (int i = 0; i < n; ++i)
    {
        std::wstring txt;
        if (!GetMenuItemTextW(menuBar, (UINT)i, txt)) continue;
        if (NormalizeMenuLabel(txt) == want) return i;
    }
    return -1;
}

static HMENU GetSubMenuSafe(HMENU parent, int pos)
{
    if (!parent || pos < 0) return nullptr;
    return GetSubMenu(parent, pos);
}

static HMENU FindOrCreateExportMenu(HMENU fileMenu)
{
    if (!fileMenu) return nullptr;

    const int n = GetMenuItemCount(fileMenu);
    for (int i = 0; i < n; ++i)
    {
        HMENU sub = GetSubMenu(fileMenu, i);
        if (!sub) continue;
        std::wstring txt;
        if (!GetMenuItemTextW(fileMenu, (UINT)i, txt)) continue;
        if (NormalizeMenuLabel(txt) == L"Export")
            return sub;
    }

    // Not found: create an Export submenu at the end.
    HMENU exportMenu = CreatePopupMenu();
    if (!exportMenu) return nullptr;
    AppendMenuW(fileMenu, MF_POPUP, (UINT_PTR)exportMenu, L"&Export");
    return exportMenu;
}

// ------------------------------
// Submenu building
// ------------------------------

static void ClearMenu(HMENU menu)
{
    if (!menu) return;
    const int n = GetMenuItemCount(menu);
    for (int i = n - 1; i >= 0; --i)
        DeleteMenu(menu, (UINT)i, MF_BYPOSITION);
}

static void RebuildReVoicePopup()
{
    if (!g_reVoicePopup) return;

    ClearMenu(g_reVoicePopup);

    void* dh = GetTESDataHandler();
    if (!dh)
    {
        AppendMenuW(g_reVoicePopup, MF_STRING | MF_GRAYED, 0, L"(TESDataHandler unavailable)");
        return;
    }

    // Prefer TESDataHandler::filesByID (loaded files only) over fileList (all files in Data). (CSE/Core.h)
    void* activeFile = nullptr;
    (void)ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_ActiveFile, activeFile);

    UInt32 fileCount = 0;
    (void)ReadU32(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_FileCount, fileCount);

    UInt32 added = 0;
    for (UInt32 i = 0; i < 0xFF; ++i)
    {
        void* file = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_FilesByID + (sizeof(void*) * i), file) || !file)
            continue;

        // Validate fileIndex if present; do not hard-fail on mismatch.
        UInt8 fileIndex = 0xFF;
        (void)ReadMem(reinterpret_cast<const unsigned char*>(file) + kTESFile_FileIndex, &fileIndex, sizeof(fileIndex));

        char nameBuf[260] = {};
        if (!ReadMem(reinterpret_cast<const unsigned char*>(file) + kTESFile_FileName, nameBuf, sizeof(nameBuf)))
            continue;
        nameBuf[sizeof(nameBuf) - 1] = 0;

        std::string nameA(nameBuf);
        if (nameA.empty())
            continue;

        std::wstring nameW = ToWide(nameA);
        if (nameW.empty())
            continue;

        // Mark the active file for operator visibility.
        if (file == activeFile)
            nameW += L" (Active)";

        const UINT cmd = kCmdBase_ReVoiceMod + (UINT)i;
        AppendMenuW(g_reVoicePopup, MF_STRING, cmd, nameW.c_str());
        ++added;
    }

    if (!added)
    {
        if (fileCount == 0)
            AppendMenuW(g_reVoicePopup, MF_STRING | MF_GRAYED, 0, L"(no loaded plugins)");
        else
            AppendMenuW(g_reVoicePopup, MF_STRING | MF_GRAYED, 0, L"(loaded plugin list unavailable)");
    }
}

// ------------------------------
// Main window discovery & menu install
// ------------------------------

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM)
{
    DWORD pid = 0;
    GetWindowThreadProcessId(hWnd, &pid);
    if (pid != GetCurrentProcessId())
        return TRUE;

    if (!IsWindowVisible(hWnd))
        return TRUE;

    // Must have a menu bar.
    if (!GetMenu(hWnd))
        return TRUE;

    g_mainWnd = hWnd;
    return FALSE;
}

static HWND TryResolveMainWindow()
{
    g_mainWnd = nullptr;
    EnumWindows(EnumWindowsProc, 0);
    return g_mainWnd;
}

static bool InstallMenuOnce()
{
    if (!g_mainWnd)
        return false;

    HMENU menuBar = GetMenu(g_mainWnd);
    if (!menuBar)
        return false;

    const int fileIdx = FindTopMenuIndexByLabel(menuBar, L"File");
    if (fileIdx < 0)
        return false;

    HMENU fileMenu = GetSubMenuSafe(menuBar, fileIdx);
    if (!fileMenu)
        return false;

    HMENU exportMenu = FindOrCreateExportMenu(fileMenu);
    if (!exportMenu)
        return false;

    // Avoid duplicate injection.
    const int n = GetMenuItemCount(exportMenu);
    for (int i = 0; i < n; ++i)
    {
        HMENU sub = GetSubMenu(exportMenu, i);
        if (!sub) continue;
        std::wstring txt;
        if (!GetMenuItemTextW(exportMenu, (UINT)i, txt)) continue;
        if (NormalizeMenuLabel(txt) == L"reVoice CSV")
        {
            g_reVoicePopup = sub;
            return true;
        }
    }

    g_reVoicePopup = CreatePopupMenu();
    if (!g_reVoicePopup)
        return false;

    AppendMenuW(exportMenu, MF_POPUP, (UINT_PTR)g_reVoicePopup, L"reVoice CSV");
    DrawMenuBar(g_mainWnd);
    return true;
}

static void EnsureWndProcSubclassed()
{
    if (!g_mainWnd || g_oldWndProc) return;
    g_oldWndProc = (WNDPROC)SetWindowLongPtrW(g_mainWnd, GWLP_WNDPROC, (LONG_PTR)+[](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        if (msg == WM_INITMENUPOPUP)
        {
            const HMENU hm = (HMENU)wParam;
            if (hm && g_reVoicePopup && hm == g_reVoicePopup)
                RebuildReVoicePopup();
        }
        else if (msg == WM_COMMAND)
        {
            const UINT id = LOWORD(wParam);
            if (id >= kCmdBase_ReVoiceMod && id <= (kCmdBase_ReVoiceMod + 0xFF))
            {
                const UInt8 modIdx = (UInt8)(id - kCmdBase_ReVoiceMod);

                // Resolve plugin name again (for filename). If we fail, fall back.
                std::wstring pluginName = L"plugin";
                {
                    // Find in current popup entries by ID.
                    const int cnt = GetMenuItemCount(g_reVoicePopup);
                    for (int i = 0; i < cnt; ++i)
                    {
                        MENUITEMINFOW mii{};
                        wchar_t buf[512] = {};
                        mii.cbSize = sizeof(mii);
                        mii.fMask = MIIM_ID | MIIM_STRING;
                        mii.dwTypeData = buf;
                        mii.cch = (UINT)(std::size(buf) - 1);
                        if (!GetMenuItemInfoW(g_reVoicePopup, (UINT)i, TRUE, &mii))
                            continue;
                        if (mii.wID == id)
                        {
                            pluginName.assign(buf);
                            break;
                        }
                    }
                }

                const std::wstring outPath = BuildDefaultOutPathForPlugin(pluginName);
                _MESSAGE("reVoice: export requested modIdx=%u out=%S", (unsigned)modIdx, outPath.c_str());

                const bool ok = exporter::ExportReVoiceCSVForModIndex(outPath, modIdx);
                if (ok)
                {
                    std::wstring msgW = L"reVoice export completed:\n" + outPath;
                    MessageBoxW(hWnd, msgW.c_str(), L"reVoice", MB_OK | MB_ICONINFORMATION);
                }
                else
                {
                    std::wstring msgW = L"reVoice export failed.\nSee OBSE log for details.";
                    MessageBoxW(hWnd, msgW.c_str(), L"reVoice", MB_OK | MB_ICONERROR);
                }
                return 0;
            }
        }

        return CallWindowProcW(g_oldWndProc, hWnd, msg, wParam, lParam);
    });
}

static void StartMenuInstallTimer()
{
    if (!g_mainWnd) return;
    g_timerTries = 0;
    SetTimer(g_mainWnd, kTimer_MenuInstall, kTimer_IntervalMs, nullptr);
}

static void StopMenuInstallTimer()
{
    if (!g_mainWnd) return;
    KillTimer(g_mainWnd, kTimer_MenuInstall);
}

// Intercept WM_TIMER via subclass. We keep it inside the same wndproc.
// Instead, we piggyback on standard timer delivery to the main window and
// detect it in our wndproc by watching WM_TIMER.

// For that, we need to modify the wndproc lambda to handle WM_TIMER.
// The simplest is: after subclassing, the lambda already sees WM_TIMER.
// We install the menu in a small state machine there.

// To implement that, we re-subclass with a non-lambda WndProc to keep code clean.

// NOTE: The above lambda already subclasses; we implement WM_TIMER support by
// installing the timer only AFTER subclassing and then catching WM_TIMER.

// Because the lambda is already set, we cannot modify it from here. Instead we
// handle menu install synchronously once we have a window + menu; and if not
// ready, we retry by polling from OBSEPlugin_Load with a short retry thread.

// The CS creates its main window early; a simple retry loop is sufficient.
static DWORD WINAPI DeferredInitThread(LPVOID)
{
    for (UINT i = 0; i < kTimer_MaxTries; ++i)
    {
        Sleep(kTimer_IntervalMs);

        if (!g_mainWnd)
            TryResolveMainWindow();
        if (!g_mainWnd)
            continue;

        if (!g_oldWndProc)
            EnsureWndProcSubclassed();

        if (InstallMenuOnce())
        {
            _MESSAGE("reVoice: menu installed");
            return 0;
        }
    }

    _MESSAGE("reVoice: menu install timed out");
    return 0;
}

extern "C"
{
    bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
    {
        OpenPluginLogIfNeeded();
        if (!obse || !info)
            return false;

        g_obse = const_cast<OBSEInterface*>(obse);

        info->infoVersion = PluginInfo::kInfoVersion;
        info->name = "reVoice_CSE";
        info->version = 1;

        // Must be editor.
        if (!obse->isEditor)
            return false;

        g_pluginHandle = obse->GetPluginHandle();

        // Always allow load; do not hard-fail on early UI state.
        return true;
    }

    bool OBSEPlugin_Load(const OBSEInterface* obse)
    {
        if (!obse || !obse->isEditor)
            return false;

        g_obse = const_cast<OBSEInterface*>(obse);

        OpenPluginLogIfNeeded();
        _MESSAGE("reVoice: OBSEPlugin_Load");

        // Defer menu installation; the CS may not have created its main window yet.
        TryResolveMainWindow();
        if (g_mainWnd)
        {
            EnsureWndProcSubclassed();
            (void)InstallMenuOnce();
        }

        // Always spin up a short-lived initializer to guarantee menu injection.
        CloseHandle(CreateThread(nullptr, 0, DeferredInitThread, nullptr, 0, nullptr));
        return true;
    }
}
