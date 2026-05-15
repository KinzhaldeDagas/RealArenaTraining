#include "Win32Util.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include <cstdio>
#include <vector>
#pragma comment(lib, "Shlwapi.lib")

std::wstring win32::GetModuleDir(HMODULE h)
{
    wchar_t buf[MAX_PATH]{};
    GetModuleFileNameW(h, buf, MAX_PATH);
    PathRemoveFileSpecW(buf);
    return buf;
}

std::wstring win32::JoinPath(const std::wstring& a, const std::wstring& b)
{
    if (a.empty()) return b;
    if (b.empty()) return a;
    std::wstring out = a;
    if (out.back() != L'\\' && out.back() != L'/')
        out.push_back(L'\\');
    out += b;
    return out;
}

bool win32::EnsureDirForFile(const std::wstring& filePath)
{
    wchar_t path[MAX_PATH]{};
    wcsncpy_s(path, filePath.c_str(), _TRUNCATE);
    PathRemoveFileSpecW(path);
    if (path[0] == 0) return true;
    return SHCreateDirectoryExW(nullptr, path, nullptr) == ERROR_SUCCESS
        || GetLastError() == ERROR_ALREADY_EXISTS;
}

// --------------------
// NEW helpers
// --------------------

std::wstring win32::GetExeDir()
{
    return GetModuleDir(nullptr);
}

std::wstring win32::GetDataDir()
{
    return JoinPath(GetExeDir(), L"Data");
}

static std::wstring GetCsidlDir(int csidl)
{
    wchar_t buf[MAX_PATH]{};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, csidl, nullptr, SHGFP_TYPE_CURRENT, buf)))
        return buf;
    return L"";
}

std::wstring win32::GetDocumentsDir()
{
    return GetCsidlDir(CSIDL_MYDOCUMENTS);
}

std::wstring win32::GetLocalAppDataDir()
{
    return GetCsidlDir(CSIDL_LOCAL_APPDATA);
}

std::string win32::WideToUtf8(const std::wstring& w)
{
    if (w.empty()) return {};
    int bytes = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    if (bytes <= 0) return {};
    std::string out;
    out.resize((size_t)bytes);
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), out.data(), bytes, nullptr, nullptr);
    return out;
}

std::wstring win32::Utf8ToWide(const char* s)
{
    if (!s || !*s) return {};
    int wchars = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
    if (wchars <= 0) return {};
    std::wstring out;
    out.resize((size_t)wchars - 1);
    MultiByteToWideChar(CP_UTF8, 0, s, -1, out.data(), wchars);
    return out;
}

bool win32::TryTouchFile(const std::wstring& path)
{
    FILE* f = nullptr;
    _wfopen_s(&f, path.c_str(), L"ab");
    if (!f) return false;
    fclose(f);
    return true;
}

std::wstring win32::BaseName(const std::wstring& path)
{
    const wchar_t* p = PathFindFileNameW(path.c_str());
    return p ? std::wstring(p) : path;
}
