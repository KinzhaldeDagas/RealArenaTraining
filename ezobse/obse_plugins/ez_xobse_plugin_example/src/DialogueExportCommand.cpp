#include "DialogueExportCommand.h"
#include "CSDialogDriver.h"
#include "CSVWriter.h"
#include "Win32Util.h"
#include "TESCompat.h"

#include "obse/Utilities.h"

#include <Windows.h>

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <cstring>

static UInt32 ReadFormIDSafe(const void* tesForm);
static std::string hex8(UInt32 v);

namespace
{
    // TESDataHandler::Singleton (editor) from CSE/Core.cpp
    static constexpr uintptr_t kTESDataHandler_Singleton = 0x00A0E064;

    // TESDataHandler offsets (CSE/Core.h)
    static constexpr ptrdiff_t kTESDataHandler_Races = 0x0044; // tList<TESRace>
    static constexpr ptrdiff_t kTESDataHandler_Topics = 0x007C; // tList<TESTopic>
    static constexpr ptrdiff_t kTESDataHandler_ActiveFile = 0x0E04;
    static constexpr ptrdiff_t kTESDataHandler_FilesByID  = 0x0E14;

    // TESTopic offsets (CSE/TESTopic.h)
    static constexpr ptrdiff_t kTESTopic_TopicData = 0x0034; // tList<TopicData>

    // TESTopic::TopicData offsets (CSE/TESTopic.h)
    static constexpr ptrdiff_t kTopicData_ParentQuest = 0x0000;
    static constexpr ptrdiff_t kTopicData_QuestInfos  = 0x0004; // TopicInfoArray (NiTLargeArray-derived), 0x18+

    // TESTopicInfo offsets (CSE/TESTopicInfo.h)
    static constexpr ptrdiff_t kTopicInfo_ResponseList = 0x0044; // tList<ResponseData>

    // TESTopicInfo::ResponseData offsets (CSE/TESTopicInfo.h)
    static constexpr ptrdiff_t kResponseData_ResponseNumber = 0x000C;
    static constexpr ptrdiff_t kResponseData_ResponseText   = 0x0010; // BSString

    // Form types (TES4)
    static constexpr UInt8 kFormType_Race     = 0x09;
    static constexpr UInt8 kFormType_Topic    = 0x39; // DIAL
    static constexpr UInt8 kFormType_TopicInfo= 0x3A; // INFO
    static constexpr UInt8 kFormType_Quest    = 0x22;

    // Hard safety caps to avoid runaway traversal if memory is corrupt.
    static constexpr UInt32 kMaxTopics = 250000;
    static constexpr UInt32 kMaxTopicDataNodes = 250000;
    static constexpr UInt32 kMaxInfosPerTopicData = 250000;
    static constexpr UInt32 kMaxResponsesPerInfo = 250000;
    static constexpr size_t kMaxStringBytes = 1u << 20; // 1MB hard cap

    struct ListNode
    {
        void* item;
        void* next;
    };

    struct TopicInfosRaw
    {
        UInt32 w0,w1,w2,w3,w4,w5;
    };

    static bool ReadMem(const void* addr, void* out, size_t size)
    {
        if (!addr || !out || size == 0) return false;
        SIZE_T br = 0;
        return ReadProcessMemory(GetCurrentProcess(), addr, out, size, &br) && br == size;
    }

    template <class T>
    static bool ReadT(const void* addr, T& out)
    {
        out = {};
        return ReadMem(addr, &out, sizeof(T));
    }

    static bool ReadPtr(const void* addr, void*& out)
    {
        out = nullptr;
        return ReadMem(addr, &out, sizeof(out));
    }

    static bool ReadU8Safe(const void* addr, UInt8& out)
    {
        out = 0;
        return ReadMem(addr, &out, sizeof(out));
    }

    static bool ReadU32Safe(const void* addr, UInt32& out)
    {
        out = 0;
        return ReadMem(addr, &out, sizeof(out));
    }

    static bool IsReadable(const void* addr, size_t size)
    {
        if (!addr || size == 0) return false;
        MEMORY_BASIC_INFORMATION mbi{};
        if (!VirtualQuery(addr, &mbi, sizeof(mbi))) return false;
        if (mbi.State != MEM_COMMIT) return false;
        if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;
        const auto start = reinterpret_cast<uintptr_t>(addr);
        const auto end = start + size;
        const auto regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        return end <= regionEnd;
    }

    static bool ReadCString(const char* p, std::string& out)
    {
        out.clear();
        if (!p) return false;

        constexpr size_t kChunk = 4096;
        char buf[kChunk];

        size_t total = 0;
        while (total < kMaxStringBytes)
        {
            MEMORY_BASIC_INFORMATION mbi{};
            if (!VirtualQuery(p + total, &mbi, sizeof(mbi)))
                return false;
            if (mbi.State != MEM_COMMIT) return false;
            if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;

            const auto cur = reinterpret_cast<uintptr_t>(p + total);
            const auto regionBase = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
            const size_t regionRemain = static_cast<size_t>((regionBase + mbi.RegionSize) - cur);
            size_t toRead = std::min({ regionRemain, kChunk, kMaxStringBytes - total });
            if (toRead == 0) return false;

            SIZE_T br = 0;
            if (!ReadProcessMemory(GetCurrentProcess(), p + total, buf, toRead, &br) || br == 0)
                return false;

            const char* z = (const char*)memchr(buf, 0, br);
            if (z)
            {
                const size_t n = static_cast<size_t>(z - buf);
                out.append(buf, n);
                return true;
            }

            out.append(buf, (size_t)br);
            total += (size_t)br;
        }

        return false;
    }

    static bool ReadBSStringAt(const void* base, ptrdiff_t off, std::string& out)
    {
        out.clear();
        void* p = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(base) + off, p)) return false;
        return ReadCString(reinterpret_cast<const char*>(p), out);
    }

    static bool ReadEditorIDSafe(const void* tesForm, std::string& out)
    {
        // TESForm::editorID (BSString) at +0x10
        return tesForm ? ReadBSStringAt(tesForm, 0x10, out) : false;
    }

    static bool IsTESFormType(void* p, UInt8 expectedType)
    {
        if (!p) return false;
        if (!IsReadable(p, 0x14)) return false;

        // vtbl must be readable
        void* vtbl = nullptr;
        if (!ReadPtr(p, vtbl) || (vtbl && !IsReadable(vtbl, sizeof(void*))))
            return false;

        UInt8 ft = 0;
        if (!ReadU8Safe(reinterpret_cast<const unsigned char*>(p) + 0x04, ft))
            return false;
        if (ft != expectedType)
            return false;

        const UInt32 id = ReadFormIDSafe(p);
        return id != 0;
    }

    static bool IsTopicDataLike(void* p)
    {
        if (!p) return false;
        if (!IsReadable(p, 0x20)) return false;

        void* parentQuest = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(p) + kTopicData_ParentQuest, parentQuest)) return false;
        if (parentQuest && !IsTESFormType(parentQuest, kFormType_Quest))
            return false;

        // The TopicInfoArray vtbl (at +0x04) should at least be readable.
        void* arrVtbl = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(p) + kTopicData_QuestInfos, arrVtbl))
            return false;
        if (arrVtbl && !IsReadable(arrVtbl, sizeof(void*)))
            return false;

        return true;
    }

    static bool IsTopicInfoPtr(void* p)
    {
        return IsTESFormType(p, kFormType_TopicInfo);
    }

    static bool IsTopicPtr(void* p)
    {
        return IsTESFormType(p, kFormType_Topic);
    }

    static bool IsRacePtr(void* p)
    {
        return IsTESFormType(p, kFormType_Race);
    }

    static bool IsResponseDataLike(void* p)
    {
        if (!p) return false;
        if (!IsReadable(p, 0x20)) return false;

        void* s = nullptr;
        (void)ReadPtr(reinterpret_cast<const unsigned char*>(p) + kResponseData_ResponseText, s);
        return !s || IsReadable(s, 1);
    }

    template <class Validator>
    static bool ReadListNodeGuess(const void* nodeAddr, void*& outItem, void*& outNext, Validator&& validItem)
    {
        outItem = nullptr;
        outNext = nullptr;
        if (!nodeAddr) return false;

        void* p0 = nullptr;
        void* p1 = nullptr;
        if (!ReadPtr(nodeAddr, p0)) return false;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(nodeAddr) + sizeof(void*), p1)) return false;

        if (validItem && validItem(p0)) { outItem = p0; outNext = p1; return true; }
        if (validItem && validItem(p1)) { outItem = p1; outNext = p0; return true; }

        // If neither looks like an item, assume [item,next] as a best-effort.
        outItem = p0;
        outNext = p1;
        return true;
    }

    static bool ReadResponseText(const void* respData, std::string& outText)
    {
        outText.clear();
        return ReadBSStringAt(respData, kResponseData_ResponseText, outText);
    }

    static UInt8 ReadResponseNumber(const void* respData)
    {
        UInt8 n = 0;
        (void)ReadU8Safe(reinterpret_cast<const unsigned char*>(respData) + kResponseData_ResponseNumber, n);
        return n;
    }

    static void* ReadTESDataHandlerPtr()
    {
        void* dh = nullptr;
        (void)ReadPtr(reinterpret_cast<const void*>(kTESDataHandler_Singleton), dh);
        return dh;
    }

    static bool GetActiveModIndex(UInt8& outModIndex)
    {
        outModIndex = 0xFF;
        void* dh = ReadTESDataHandlerPtr();
        if (!dh) return false;

        void* activeFile = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_ActiveFile, activeFile) || !activeFile)
            return false;

        for (UInt32 i = 0; i < 0xFF; ++i)
        {
            void* f = nullptr;
            if (!ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_FilesByID + (sizeof(void*) * i), f))
                break;
            if (f == activeFile)
            {
                outModIndex = static_cast<UInt8>(i);
                return true;
            }
        }

        return false;
    }

    static std::string NormalizeTextForCSV(std::string s)
    {
        std::string out;
        out.reserve(s.size());

        for (size_t i = 0; i < s.size(); ++i)
        {
            const char c = s[i];
            if (c == '\r')
            {
                if (i + 1 < s.size() && s[i + 1] == '\n')
                    ++i;
                out.push_back('\n');
                continue;
            }
            out.push_back(c);
        }
        return out;
    }

    static std::string VoiceTokenFromRace(void* race)
    {
        if (!race) return {};
        std::string edid;
        (void)ReadEditorIDSafe(race, edid);
        if (edid.empty()) edid = hex8(ReadFormIDSafe(race));
        return std::string("RACE|") + edid;
    }

    static std::vector<std::string> BuildVoicePool_AllRaces()
    {
        static std::vector<std::string> cache;
        static bool built = false;
        if (built) return cache;

        built = true;
        cache.clear();

        void* dh = ReadTESDataHandlerPtr();
        if (!dh)
        {
            _MESSAGE("BuildVoicePool_AllRaces: TESDataHandler is null.");
            return cache;
        }

        void* head = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_Races, head) || !head)
        {
            _MESSAGE("BuildVoicePool_AllRaces: races list head is null/unreadable.");
            return cache;
        }

        std::unordered_set<std::string> seen;
        seen.reserve(256);

        void* node = head;
        UInt32 steps = 0;
        while (node && steps++ < 500000)
        {
            void* race = nullptr;
            void* next = nullptr;
            if (!ReadListNodeGuess(node, race, next, IsRacePtr))
                break;
            node = next;

            if (!IsRacePtr(race))
                continue;

            std::string tok = VoiceTokenFromRace(race);
            if (!tok.empty() && seen.insert(tok).second)
                cache.push_back(std::move(tok));
        }

        std::sort(cache.begin(), cache.end());
        cache.erase(std::unique(cache.begin(), cache.end()), cache.end());

        _MESSAGE("BuildVoicePool_AllRaces: size=%u", (unsigned)cache.size());
        return cache;
    }

    static bool DecodeTopicInfos(const void* questInfosObj, void*& outData, UInt32& outCount)
    {
        outData = nullptr;
        outCount = 0;

        TopicInfosRaw r{};
        if (!ReadT(questInfosObj, r))
            return false;

        struct Cand { void* data; UInt32 count; UInt32 cap; };
        const Cand cands[] =
        {
            { (void*)r.w1, r.w2, r.w3 },
            { (void*)r.w1, r.w3, r.w2 },
            { (void*)r.w1, r.w4, r.w2 },
            { (void*)r.w1, r.w4, r.w3 },

            { (void*)r.w2, r.w3, r.w4 },
            { (void*)r.w2, r.w4, r.w3 },
            { (void*)r.w2, r.w1, r.w3 },
            { (void*)r.w2, r.w1, r.w4 },

            { (void*)r.w4, r.w2, r.w3 },
            { (void*)r.w4, r.w3, r.w2 },
            { (void*)r.w4, r.w1, r.w2 },
            { (void*)r.w4, r.w1, r.w3 },
        };

        for (const auto& c : cands)
        {
            if (c.count == 0)
            {
                outData = c.data;
                outCount = 0;
                return true;
            }

            if (!c.data) continue;
            if (c.count > c.cap && c.cap != 0) continue;
            if (c.count > kMaxInfosPerTopicData) continue;

            if (!IsReadable(c.data, sizeof(void*))) continue;
            void* first = nullptr;
            if (!ReadPtr(c.data, first)) continue;
            if (!IsTopicInfoPtr(first)) continue;

            outData = c.data;
            outCount = c.count;
            return true;
        }

        return false;
    }

    static std::string ReadTopicVoiceID(void* topic)
    {
        std::string edid;
        if (!ReadEditorIDSafe(topic, edid) || edid.empty())
            edid = std::string("TOPIC_") + hex8(ReadFormIDSafe(topic));
        return edid;
    }
}

static UInt32 ReadFormIDSafe(const void* tesForm)
{
    UInt32 id = 0;
    if (!tesForm) return 0;
    (void)ReadU32Safe(reinterpret_cast<const unsigned char*>(tesForm) + 0x0C, id);
    if (id == 0 || id == 0xCCCCCCCCu || id == 0xCDCDCDCDu) return 0;
    return id;
}

static std::string hex8(UInt32 v)
{
    std::ostringstream ss;
    ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << v;
    return ss.str();
}

bool exporter::ExportSelectedToCSV(const std::wstring& outPath)
{
    _MESSAGE("ExportSelectedToCSV: outPath=%S", outPath.c_str());

    if (outPath.empty())
    {
        _MESSAGE("ExportSelectedToCSV: outPath is empty.");
        return false;
    }

    cs::SelectedDialogueSnapshot snap{};
    if (!cs::TryReadSelectedDialogueSnapshot(snap))
    {
        _MESSAGE("ExportSelectedToCSV: TryReadSelectedDialogueSnapshot FAILED. (dialog not open / selection missing / address mismatch)");
        return false;
    }

    if (!win32::EnsureDirForFile(outPath))
    {
        _MESSAGE("ExportSelectedToCSV: EnsureDirForFile FAILED for %S", outPath.c_str());
        return false;
    }

    io::CSVWriter w(outPath);
    if (!w.ok())
    {
        _MESSAGE("ExportSelectedToCSV: CSVWriter open FAILED for %S", outPath.c_str());
        return false;
    }

    w.write_header({
        "DialogueKey",
        "InfoFormID",
        "QuestFormID",
        "ResponseText",
        "VoicedRaceFormIDs"
    });

    const UInt32 infoFormID = ReadFormIDSafe(snap.info);
    const UInt32 questFormID = ReadFormIDSafe(snap.quest);

    std::string races;
    for (size_t i = 0; i < snap.voicedRaces.size(); ++i)
    {
        TESRace* r = snap.voicedRaces[i];
        const UInt32 rid = ReadFormIDSafe(r);
        if (i) races.push_back('|');
        races += hex8(rid);
    }

    io::CSVRow row;
    row.cols = {
        std::string("SEL:") + hex8(infoFormID),
        hex8(infoFormID),
        hex8(questFormID),
        NormalizeTextForCSV(snap.responseText),
        races
    };

    w.write_row(row);

    _MESSAGE("ExportSelectedToCSV: wrote 1 row (Info=%08X Quest=%08X Races=%u)",
        infoFormID, questFormID, (unsigned)snap.voicedRaces.size());

    return true;
}

bool exporter::ExportReVoiceCSVForModIndex(const std::wstring& outPath, unsigned char modIndex)
{
    _MESSAGE("ExportReVoiceCSVForModIndex: modIdx=%u out=%S", (unsigned)modIndex, outPath.c_str());

    if (outPath.empty())
    {
        _MESSAGE("ExportReVoiceCSVForModIndex: outPath is empty.");
        return false;
    }

    if (!win32::EnsureDirForFile(outPath))
    {
        _MESSAGE("ExportReVoiceCSVForModIndex: EnsureDirForFile FAILED for %S", outPath.c_str());
        return false;
    }

    io::CSVWriter w(outPath);
    if (!w.ok())
    {
        _MESSAGE("ExportReVoiceCSVForModIndex: CSVWriter open FAILED for %S", outPath.c_str());
        return false;
    }

    w.write_header({
        "FormID",
        "VoiceID",
        "SpeakerInfo",
        "OutputPath",
        "Dialogue"
    });

    const auto speakers = BuildVoicePool_AllRaces();
    static const std::vector<std::string> kFallbackSpeaker = { "__UNRESOLVED__" };
    const std::vector<std::string>& speakersView = speakers.empty() ? kFallbackSpeaker : speakers;

    void* dh = ReadTESDataHandlerPtr();
    if (!dh)
    {
        _MESSAGE("ExportReVoiceCSVForModIndex: TESDataHandler is null.");
        return false;
    }

    void* topicsHead = nullptr;
    if (!ReadPtr(reinterpret_cast<const unsigned char*>(dh) + kTESDataHandler_Topics, topicsHead) || !topicsHead)
    {
        _MESSAGE("ExportReVoiceCSVForModIndex: topics list head is null/unreadable.");
        return true; // header-only is still a valid file
    }

    UInt32 topicsSeen = 0;
    UInt32 topicDataSeen = 0;
    UInt32 infosSeen = 0;
    UInt32 infosInScope = 0;
    UInt32 responsesSeen = 0;
    UInt32 rowsWritten = 0;
    UInt32 skippedNoResponse = 0;
    UInt32 skippedNoText = 0;

    void* tnode = topicsHead;
    UInt32 tsteps = 0;
    while (tnode && tsteps++ < kMaxTopics)
    {
        void* topic = nullptr;
        void* nextT = nullptr;
        if (!ReadListNodeGuess(tnode, topic, nextT, IsTopicPtr))
            break;
        tnode = nextT;

        if (!IsTopicPtr(topic))
            continue;

        ++topicsSeen;
        const std::string voiceID = ReadTopicVoiceID(topic);

        void* tdHead = nullptr;
        if (!ReadPtr(reinterpret_cast<const unsigned char*>(topic) + kTESTopic_TopicData, tdHead) || !tdHead)
            continue;

        void* tdNode = tdHead;
        UInt32 tdSteps = 0;
        while (tdNode && tdSteps++ < kMaxTopicDataNodes)
        {
            void* topicData = nullptr;
            void* nextTD = nullptr;
            if (!ReadListNodeGuess(tdNode, topicData, nextTD, IsTopicDataLike))
                break;
            tdNode = nextTD;

            if (!IsTopicDataLike(topicData))
                continue;

            ++topicDataSeen;

            const void* questInfosObj = reinterpret_cast<const unsigned char*>(topicData) + kTopicData_QuestInfos;
            void* infoPtrArray = nullptr;
            UInt32 infoCount = 0;
            if (!DecodeTopicInfos(questInfosObj, infoPtrArray, infoCount))
                continue;

            if (infoCount == 0 || !infoPtrArray)
                continue;

            for (UInt32 i = 0; i < infoCount; ++i)
            {
                void* info = nullptr;
                if (!ReadPtr(reinterpret_cast<const unsigned char*>(infoPtrArray) + sizeof(void*) * i, info))
                    break;

                if (!IsTopicInfoPtr(info))
                    continue;

                ++infosSeen;

                const UInt32 infoFormID = ReadFormIDSafe(info);
                if (!infoFormID)
                    continue;

                const UInt8 mi = static_cast<UInt8>(infoFormID >> 24);
                if (mi != modIndex)
                    continue;

                ++infosInScope;

                void* respHead = nullptr;
                if (!ReadPtr(reinterpret_cast<const unsigned char*>(info) + kTopicInfo_ResponseList, respHead) || !respHead)
                {
                    ++skippedNoResponse;
                    continue;
                }

                UInt32 nonEmptyResp = 0;
                {
                    void* rnode = respHead;
                    UInt32 rsteps = 0;
                    while (rnode && rsteps++ < kMaxResponsesPerInfo)
                    {
                        void* resp = nullptr;
                        void* nextR = nullptr;
                        if (!ReadListNodeGuess(rnode, resp, nextR, IsResponseDataLike))
                            break;
                        rnode = nextR;

                        if (!IsResponseDataLike(resp))
                            continue;

                        std::string text;
                        if (ReadResponseText(resp, text) && !text.empty())
                            ++nonEmptyResp;
                    }
                }

                if (nonEmptyResp == 0)
                {
                    ++skippedNoText;
                    continue;
                }

                const bool multi = (nonEmptyResp > 1);
                const std::string infoHex = hex8(infoFormID);

                void* rnode = respHead;
                UInt32 rsteps = 0;
                while (rnode && rsteps++ < kMaxResponsesPerInfo)
                {
                    void* resp = nullptr;
                    void* nextR = nullptr;
                    if (!ReadListNodeGuess(rnode, resp, nextR, IsResponseDataLike))
                        break;
                    rnode = nextR;

                    if (!IsResponseDataLike(resp))
                        continue;

                    std::string text;
                    if (!ReadResponseText(resp, text) || text.empty())
                        continue;

                    ++responsesSeen;

                    std::string fileKey = infoHex;
                    if (multi)
                    {
                        const UInt8 respNum = ReadResponseNumber(resp);
                        fileKey.push_back('_');
                        fileKey += std::to_string((unsigned)respNum);
                    }

                    const std::string normText = NormalizeTextForCSV(std::move(text));

                    io::CSVRow row;
                    row.cols.resize(5);
                    row.cols[0] = infoHex;
                    row.cols[1] = voiceID;

                    for (const auto& spk : speakersView)
                    {
                        row.cols[2] = spk;
                        row.cols[3] = spk + std::string("\\") + fileKey;
                        row.cols[4] = normText;

                        w.write_row(row);
                        ++rowsWritten;

                        if ((rowsWritten % 50000u) == 0)
                            _MESSAGE("ExportReVoiceCSVForModIndex: progress rows=%u infosInScope=%u topics=%u",
                                (unsigned)rowsWritten, (unsigned)infosInScope, (unsigned)topicsSeen);
                    }
                }
            }
        }
    }

    _MESSAGE("ExportReVoiceCSVForModIndex: done. topics=%u topicData=%u infos=%u inScope=%u responses=%u rows=%u skipped(noResponse=%u noText=%u)",
        (unsigned)topicsSeen,
        (unsigned)topicDataSeen,
        (unsigned)infosSeen,
        (unsigned)infosInScope,
        (unsigned)responsesSeen,
        (unsigned)rowsWritten,
        (unsigned)skippedNoResponse,
        (unsigned)skippedNoText);

    return true;
}

bool exporter::ExportReVoiceCSV(const std::wstring& outPath, const std::vector<unsigned char>& scopeModIndices)
{
    UInt8 mi = 0xFF;
    if (!scopeModIndices.empty())
        mi = scopeModIndices.front();
    else
        (void)GetActiveModIndex(mi);

    if (mi == 0xFF)
        mi = 0;

    return exporter::ExportReVoiceCSVForModIndex(outPath, mi);
}

bool exporter::ExportReVoiceCSV(const std::wstring& outPath)
{
    static const std::vector<unsigned char> emptyScope;
    return exporter::ExportReVoiceCSV(outPath, emptyScope);
}
