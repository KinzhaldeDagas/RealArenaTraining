#pragma once
#include <cstdint>

namespace addr
{
    // From TESDialog.cpp in your project:
    // ResponseEditorData::EditorCache = (ResponseEditorData**)0x00A10E2C;
    static constexpr std::uintptr_t ResponseEditorData_EditorCache = 0x00A10E2C;

    // From TESQuest.cpp in your project:
    // TESQuest::FilteredDialogWindowHandle = (HWND*)0x00A0B038;
    static constexpr std::uintptr_t TESQuest_FilteredDialogWindowHandle = 0x00A0B038;

    // Anchors observed in Hooks-Dialog.cpp (documentation only; call only with correct signatures).
    static constexpr std::uintptr_t TESTopic_EnumerateDialogData = 0x004ED070;
    static constexpr std::uintptr_t TESDialog_EnableTopicControls = 0x004ECB40;
    static constexpr std::uintptr_t TESConditionItem_DisableDialogControls = 0x004E35C0;
    static constexpr std::uintptr_t TESTopicInfo_SetInDialog = 0x004F5E10;
}
