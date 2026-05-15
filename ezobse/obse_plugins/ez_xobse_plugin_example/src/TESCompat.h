#pragma once

#if defined(__has_include)
#if __has_include("common/ITypes.h")
#include "common/ITypes.h"   // Owns UInt8/UInt16/UInt32 in xOBSE-Plugins
#elif __has_include("../common/ITypes.h")
#include "../common/ITypes.h"
#else
#include <cstdint>
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
#endif
#else
// conservative fallback
#include <cstdint>
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
#endif


template <class T>
class tList
{
public:
    struct Node
    {
        T* item;
        Node* next;
    };

    Node* head;

    struct iterator
    {
        Node* n;
        T* operator*() const { return n ? n->item : nullptr; }
        iterator& operator++() { if (n) n = n->next; return *this; }
        bool operator!=(const iterator& o) const { return n != o.n; }
    };

    iterator begin() const { return iterator{ head }; }
    iterator end() const { return iterator{ nullptr }; }
};

class BSString
{
public:
    // Minimal ABI-compatible view used by the CS: pointer to char buffer.
    const char* m_data;
    const char* c_str() const { return m_data ? m_data : ""; }
};

class TESForm
{
public:


    void** vtbl;
    UInt8 formType;
    UInt8 pad05[3];
    UInt32 formFlags;
    UInt32 formID;
    BSString editorID;
};

struct TESRace : TESForm {};
struct TESQuest : TESForm {};

struct TESTopicInfo;

struct DialogResponse
{
    UInt32 emotionType;
    UInt32 emotionValue;
    UInt32 unk08;
    UInt8  responseNumber;
    UInt8  pad0D[3];
    BSString responseText;
    BSString actorNotes;
    UInt32 unk20;
};

struct ResponseEditorData
{
    const char* editorTitle;
    UInt32 maxResponseLength;
    DialogResponse* selectedResponse;
    DialogResponse* responseLocalCopy;
    void* recorderData;
    void* parentTopic;
    TESTopicInfo* infoLocalCopy;
    TESTopicInfo* selectedInfo;
    TESQuest* selectedQuest;
    tList<TESRace> voicedRaces;
    static ResponseEditorData** EditorCache;
};
