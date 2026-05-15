#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <vector>

struct TESQuest;
struct TESTopicInfo;
struct TESRace;

namespace cs
{
    struct SelectedDialogueSnapshot
    {
        TESQuest* quest = nullptr;
        TESTopicInfo* info = nullptr;
        std::string responseText;
        std::vector<TESRace*> voicedRaces;
    };

    bool TryReadSelectedDialogueSnapshot(SelectedDialogueSnapshot& out);
    HWND GetFilteredDialogHwnd();
}
