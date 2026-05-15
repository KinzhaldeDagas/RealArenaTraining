#include "CSDialogDriver.h"
#include "Addresses.h"
#include "TESCompat.h"

#include "obse/Utilities.h"

#include <windows.h>
#include <winnt.h>

#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

ResponseEditorData** ResponseEditorData::EditorCache =
reinterpret_cast<ResponseEditorData**>(addr::ResponseEditorData_EditorCache);

namespace
{
    static ResponseEditorData** g_cacheAddr =
        reinterpret_cast<ResponseEditorData**>(addr::ResponseEditorData_EditorCache);

    static bool  g_cacheResolved = false;
    static DWORD g_lastScanTick = 0;

    template <class T>
    static bool ReadMem(const void* addr, T& out)
    {
        SIZE_T br = 0;
        out = {};
        return ReadProcessMemory(GetCurrentProcess(), addr, &out, sizeof(T), &br) && br == sizeof(T);
    }

    static bool ReadPtr(const void* addr, void*& out)
    {
        out = nullptr;
        SIZE_T br = 0;
        return ReadProcessMemory(GetCurrentProcess(), addr, &out, sizeof(out), &br) && br == sizeof(out);
    }

    static bool IsReadableRange(const void* p, size_t bytes)
    {
        if (!p || bytes == 0) return false;

        const unsigned char* cur = reinterpret_cast<const unsigned char*>(p);
        size_t remaining = bytes;

        while (remaining)
        {
            MEMORY_BASIC_INFORMATION mbi{};
            if (!VirtualQuery(cur, &mbi, sizeof(mbi)))
                return false;

            if (mbi.State != MEM_COMMIT)
                return false;

            const DWORD prot = mbi.Protect;
            if (prot == 0)
                return false;

            if (prot & PAGE_GUARD)
                return false;

            if (prot & PAGE_NOACCESS)
                return false;

            const unsigned char* regionEnd =
                reinterpret_cast<const unsigned char*>(mbi.BaseAddress) + mbi.RegionSize;

            size_t chunk = static_cast<size_t>(regionEnd - cur);
            if (chunk > remaining) chunk = remaining;

            cur += chunk;
            remaining -= chunk;
        }
        return true;
    }

    static bool IsPlausibleAsciiPtr(const char* s)
    {
        if (!s) return false;
        if (!IsReadableRange(s, 8)) return false;

        int alpha = 0;
        for (int i = 0; i < 96; ++i)
        {
            unsigned char c = 0;
            SIZE_T br = 0;
            if (!ReadProcessMemory(GetCurrentProcess(), s + i, &c, 1, &br) || br != 1)
                return false;

            if (c == 0) break;
            if (c < 0x20 || c > 0x7E) return false;
            if (std::isalpha((int)c)) alpha++;
        }
        return alpha >= 3;
    }

    static bool TryReadFormIDLike(const void* p, UInt32& outFormID)
    {
        outFormID = 0;
        if (!p) return false;
        if (!IsReadableRange(p, 0x10)) return false;

        // TESForm::formID @ +0x0C (TESCompat.h layout)
        if (!ReadMem(reinterpret_cast<const UInt8*>(p) + 0x0C, outFormID))
            return false;

        if (outFormID == 0 || outFormID == 0xCCCCCCCCu || outFormID == 0xCDCDCDCDu)
            return false;

        return true;
    }

    static bool IsValidTESFormLike(const void* p, int expectedFormType)
    {
        if (!p) return false;
        if (!IsReadableRange(p, 0x10)) return false;

        void* vtbl = nullptr;
        if (!ReadPtr(p, vtbl) || !vtbl) return false;
        if (!IsReadableRange(vtbl, sizeof(void*))) return false;

        UInt8 formType = 0;
        if (!ReadMem(reinterpret_cast<const UInt8*>(p) + 0x04, formType)) return false;
        if (expectedFormType >= 0 && formType != static_cast<UInt8>(expectedFormType)) return false;

        UInt32 formID = 0;
        if (!ReadMem(reinterpret_cast<const UInt8*>(p) + 0x0C, formID)) return false;

        if (formID == 0 || formID == 0xCCCCCCCCu || formID == 0xCDCDCDCDu)
            return false;

        return true;
    }

    static std::string SafeReadCStringRPM(const char* p, size_t maxLen)
    {
        std::string out;
        if (!p || maxLen == 0) return out;

        maxLen = (maxLen > 8192) ? 8192 : maxLen;
        out.reserve((maxLen > 256) ? 256 : maxLen);

        for (size_t i = 0; i < maxLen; ++i)
        {
            char c = 0;
            SIZE_T br = 0;
            if (!ReadProcessMemory(GetCurrentProcess(), p + i, &c, 1, &br) || br != 1)
            {
                _MESSAGE("SafeReadCStringRPM: RPM failed p=%p i=%u GLE=%lu", p, (unsigned)i, GetLastError());
                break;
            }
            if (c == 0) break;
            out.push_back(c);
        }
        return out;
    }

    static bool SectionLooksLikeData(const IMAGE_SECTION_HEADER& s)
    {
        const DWORD ch = s.Characteristics;

        if (!(ch & IMAGE_SCN_MEM_READ)) return false;
        if (ch & IMAGE_SCN_MEM_DISCARDABLE) return false;
        if (ch & IMAGE_SCN_CNT_UNINITIALIZED_DATA) return false;

        // Prefer non-exec regions first; second-pass scans allow exec.
        if (ch & IMAGE_SCN_MEM_EXECUTE) return false;

        return true;
    }

    static bool IsValidEditorData_Loose(ResponseEditorData* ed)
    {
        if (!ed) return false;
        if (!IsReadableRange(ed, sizeof(ResponseEditorData))) return false;

        UInt32 maxLen = 0;
        if (!ReadMem(&ed->maxResponseLength, maxLen)) return false;
        if (maxLen == 0 || maxLen > 8192) return false;

        const char* title = nullptr;
        if (!ReadMem(&ed->editorTitle, title)) return false;
        if (!IsPlausibleAsciiPtr(title)) return false;

        return true;
    }

    static bool IsValidEditorData_Strict(ResponseEditorData* ed)
    {
        if (!IsValidEditorData_Loose(ed)) return false;

        DialogResponse* sel = nullptr;
        if (!ReadMem(&ed->selectedResponse, sel)) return false;
        if (!sel || !IsReadableRange(sel, sizeof(DialogResponse))) return false;

        BSString resp{};
        if (!ReadMem(&sel->responseText, resp)) return false;
        if (resp.m_data && !IsReadableRange(resp.m_data, 1)) return false;

        TESTopicInfo* info = nullptr;
        if (!ReadMem(&ed->selectedInfo, info)) return false;

        UInt32 infoID = 0;
        if (!TryReadFormIDLike(info, infoID)) return false;

        TESQuest* q = nullptr;
        if (!ReadMem(&ed->selectedQuest, q)) return false;
        if (q)
        {
            UInt32 qid = 0;
            if (!TryReadFormIDLike(q, qid))
                return false;
        }

        tList<TESRace> races{};
        if (!ReadMem(&ed->voicedRaces, races)) return false;
        if (races.head && !IsReadableRange(races.head, sizeof(void*) * 2)) return false;

        return true;
    }

    static bool ValidateCacheAddr(ResponseEditorData** cand, ResponseEditorData*& outEd)
    {
        outEd = nullptr;
        if (!cand) return false;
        if (!IsReadableRange(cand, sizeof(void*))) return false;

        ResponseEditorData* ed = nullptr;
        if (!ReadMem(cand, ed))
            return false;

        if (!IsValidEditorData_Loose(ed))
            return false;

        outEd = ed;
        return true;
    }

    static bool ScanRegionForCache(unsigned char* start, size_t size, bool allowExecSections, ResponseEditorData**& outCand, ResponseEditorData*& outEd)
    {
        outCand = nullptr;
        outEd = nullptr;

        if (!start || size < sizeof(void*)) return false;

        // Hard clamp to avoid runaway if headers are weird.
        if (size > (1024u * 1024u * 64u))
            size = (1024u * 1024u * 64u);

        // Ensure at least the first bytes are readable before scanning.
        if (!IsReadableRange(start, std::min<size_t>(size, 16)))
            return false;

        // Scan DWORD-aligned.
        for (size_t off = 0; off + sizeof(void*) <= size; off += 4)
        {
            auto* cand = reinterpret_cast<ResponseEditorData**>(start + off);
            ResponseEditorData* ed = nullptr;

            if (ValidateCacheAddr(cand, ed))
            {
                outCand = cand;
                outEd = ed;
                return true;
            }
        }

        return false;
    }

    static bool TryResolveEditorCache()
    {
        // 1) Fixed address first (fast path)
        {
            ResponseEditorData* ed = nullptr;
            if (ValidateCacheAddr(reinterpret_cast<ResponseEditorData**>(addr::ResponseEditorData_EditorCache), ed))
            {
                g_cacheAddr = reinterpret_cast<ResponseEditorData**>(addr::ResponseEditorData_EditorCache);
                g_cacheResolved = true;

                const char* title = nullptr;
                UInt32 maxLen = 0;
                ReadMem(&ed->editorTitle, title);
                ReadMem(&ed->maxResponseLength, maxLen);

                _MESSAGE("EditorCache resolved via fixed addr: 0x%08X cand=%p ed=%p title=%s maxLen=%u",
                    (unsigned)addr::ResponseEditorData_EditorCache,
                    g_cacheAddr, ed,
                    title ? title : "",
                    maxLen);

                return true;
            }
        }

        const DWORD now = GetTickCount();
        if (now - g_lastScanTick < 1500)
            return false;
        g_lastScanTick = now;

        HMODULE hMod = GetModuleHandleW(nullptr);
        if (!hMod) return false;

        auto* base = reinterpret_cast<unsigned char*>(hMod);

        IMAGE_DOS_HEADER dos{};
        if (!ReadMem(base, dos) || dos.e_magic != IMAGE_DOS_SIGNATURE)
            return false;

        IMAGE_NT_HEADERS32 nt{};
        if (!ReadMem(base + dos.e_lfanew, nt) || nt.Signature != IMAGE_NT_SIGNATURE)
            return false;

        const DWORD secOff = dos.e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + nt.FileHeader.SizeOfOptionalHeader;

        // 2) Pass 1: scan data-like sections (non-exec, readable, non-discard)
        for (WORD i = 0; i < nt.FileHeader.NumberOfSections; ++i)
        {
            IMAGE_SECTION_HEADER sh{};
            if (!ReadMem(base + secOff + i * sizeof(IMAGE_SECTION_HEADER), sh))
                continue;

            if (!SectionLooksLikeData(sh))
                continue;

            unsigned char* start = base + sh.VirtualAddress;
            size_t size = (size_t)std::max<DWORD>(sh.Misc.VirtualSize, sh.SizeOfRawData);

            ResponseEditorData** candFound = nullptr;
            ResponseEditorData* edFound = nullptr;

            if (ScanRegionForCache(start, size, false, candFound, edFound))
            {
                g_cacheAddr = candFound;
                g_cacheResolved = true;

                const char* title = nullptr;
                UInt32 maxLen = 0;
                ReadMem(&edFound->editorTitle, title);
                ReadMem(&edFound->maxResponseLength, maxLen);

                _MESSAGE("EditorCache auto-resolved (data-sections): cand=%p ed=%p title=%s maxLen=%u",
                    candFound, edFound, title ? title : "", maxLen);

                return true;
            }
        }

        // 3) Pass 2: scan *all* readable committed pages within module image (covers odd section flags / VirtualSize issues)
        {
            const size_t imgSize = (size_t)nt.OptionalHeader.SizeOfImage;
            unsigned char* imgEnd = base + imgSize;

            unsigned char* cur = base;
            while (cur < imgEnd)
            {
                MEMORY_BASIC_INFORMATION mbi{};
                if (!VirtualQuery(cur, &mbi, sizeof(mbi)))
                    break;

                unsigned char* regionBase = reinterpret_cast<unsigned char*>(mbi.BaseAddress);
                unsigned char* regionEnd = regionBase + mbi.RegionSize;

                if (regionEnd <= base)
                {
                    cur = regionEnd;
                    continue;
                }

                if (regionBase >= imgEnd)
                    break;

                unsigned char* scanStart = (regionBase < base) ? base : regionBase;
                unsigned char* scanEnd = (regionEnd > imgEnd) ? imgEnd : regionEnd;

                if (mbi.State == MEM_COMMIT)
                {
                    const DWORD prot = mbi.Protect;
                    const bool noAccess = (prot & PAGE_NOACCESS) != 0;
                    const bool guard = (prot & PAGE_GUARD) != 0;

                    if (!noAccess && !guard)
                    {
                        const size_t scanSize = (size_t)(scanEnd - scanStart);

                        ResponseEditorData** candFound = nullptr;
                        ResponseEditorData* edFound = nullptr;

                        if (ScanRegionForCache(scanStart, scanSize, true, candFound, edFound))
                        {
                            g_cacheAddr = candFound;
                            g_cacheResolved = true;

                            const char* title = nullptr;
                            UInt32 maxLen = 0;
                            ReadMem(&edFound->editorTitle, title);
                            ReadMem(&edFound->maxResponseLength, maxLen);

                            _MESSAGE("EditorCache auto-resolved (module page-walk): cand=%p ed=%p title=%s maxLen=%u",
                                candFound, edFound, title ? title : "", maxLen);

                            return true;
                        }
                    }
                }

                cur = regionEnd;
            }
        }

        _MESSAGE("EditorCache auto-resolve scan failed.");
        return false;
    }

    static ResponseEditorData* TryGetEditorData()
    {
        if (!g_cacheResolved)
            TryResolveEditorCache();

        if (!g_cacheAddr || !IsReadableRange(g_cacheAddr, sizeof(void*)))
            return nullptr;

        ResponseEditorData* ed = nullptr;
        if (!ReadMem(g_cacheAddr, ed))
            return nullptr;

        if (!IsValidEditorData_Loose(ed))
            return nullptr;

        return ed;
    }
}

bool cs::TryReadSelectedDialogueSnapshot(SelectedDialogueSnapshot& out)
{
    out = {};

    ResponseEditorData* ed = TryGetEditorData();
    if (!ed)
    {
        _MESSAGE("TryReadSelectedDialogueSnapshot: Could not resolve editor data.");
        return false;
    }

    // Selection must be active for a meaningful snapshot.
    if (!IsValidEditorData_Strict(ed))
    {
        _MESSAGE("TryReadSelectedDialogueSnapshot: editor data present but selection state is invalid/not active.");
        return false;
    }

    TESQuest* q = nullptr;
    TESTopicInfo* info = nullptr;
    DialogResponse* sel = nullptr;
    tList<TESRace> racesList{};

    if (!ReadMem(&ed->selectedQuest, q)) return false;
    if (!ReadMem(&ed->selectedInfo, info)) return false;
    if (!ReadMem(&ed->selectedResponse, sel)) return false;
    if (!ReadMem(&ed->voicedRaces, racesList)) return false;

    UInt32 infoID = 0;
    if (!TryReadFormIDLike(info, infoID))
    {
        _MESSAGE("TryReadSelectedDialogueSnapshot: INVALID selectedInfo=%p (no formID)", info);
        return false;
    }

    if (q)
    {
        UInt32 qid = 0;
        if (!TryReadFormIDLike(q, qid))
        {
            _MESSAGE("TryReadSelectedDialogueSnapshot: selectedQuest looks invalid quest=%p (dropping)", q);
            q = nullptr;
        }
    }

    if (!sel || !IsReadableRange(sel, sizeof(DialogResponse)))
    {
        _MESSAGE("TryReadSelectedDialogueSnapshot: INVALID selectedResponse=%p", sel);
        return false;
    }

    out.quest = q;
    out.info = info;

    BSString resp{};
    if (!ReadMem(&sel->responseText, resp))
        return false;

    if (resp.m_data && IsReadableRange(resp.m_data, 1))
        out.responseText = SafeReadCStringRPM(resp.m_data, 8192);
    else
        out.responseText.clear();

    // Voiced races list: tolerate node layouts that have 2 or 3 pointers
    struct Node2 { TESRace* item; void* next; };
    struct Node3 { TESRace* item; void* linkA; void* linkB; };

    std::vector<void*> visited;
    visited.reserve(64);

    auto isVisited = [&](void* p) -> bool
        {
            for (void* v : visited) if (v == p) return true;
            return false;
        };

    auto isNodePtr = [&](void* p) -> bool
        {
            return p && !isVisited(p) && IsReadableRange(p, sizeof(Node2));
        };

    void* n = racesList.head;
    while (n)
    {
        if (isVisited(n))
            break;
        visited.push_back(n);

        TESRace* r = nullptr;
        void* a = nullptr;
        void* b = nullptr;

        if (IsReadableRange(n, sizeof(Node3)))
        {
            Node3 node{};
            if (!ReadMem(n, node)) break;
            r = node.item;
            a = node.linkA;
            b = node.linkB;
        }
        else if (IsReadableRange(n, sizeof(Node2)))
        {
            Node2 node{};
            if (!ReadMem(n, node)) break;
            r = node.item;
            a = node.next;
            b = nullptr;
        }
        else
        {
            break;
        }

        UInt32 rid = 0;
        if (TryReadFormIDLike(r, rid))
            out.voicedRaces.push_back(r);

        void* next = nullptr;
        if (isNodePtr(a)) next = a;
        else if (isNodePtr(b)) next = b;

        n = next;
    }

    _MESSAGE("TryReadSelectedDialogueSnapshot: OK quest=%p info=%p textLen=%u races=%u",
        out.quest, out.info, (UInt32)out.responseText.size(), (UInt32)out.voicedRaces.size());

    return true;
}

HWND cs::GetFilteredDialogHwnd()
{
    HWND out = nullptr;
    auto* p = reinterpret_cast<HWND*>(addr::TESQuest_FilteredDialogWindowHandle);
    if (!p) return nullptr;

    if (!ReadMem(p, out))
        return nullptr;

    return out;
}
