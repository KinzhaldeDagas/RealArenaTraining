#pragma once
#include <string>

namespace exporter
{
    // Production contract:
    // - Enumerate NPC candidates (DataHandler-driven)
    // - Re-evaluate eligibility using CS pipeline (selection/refresh)
    // - Export only NPCs that pass
    //
    // This requires your repo's DataHandler iteration + "bind speaker" implementation.
    // The skeleton is provided in src/DialogueExportNPC.cpp.
    bool ExportSelectedNPCAccurateCSV(const std::wstring& outPath);
}
