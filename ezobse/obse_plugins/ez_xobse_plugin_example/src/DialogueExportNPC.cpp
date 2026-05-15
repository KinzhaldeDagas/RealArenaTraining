#include "DialogueExportNPC.h"
#include "CSDialogDriver.h"
#include "DialogueExportCommand.h"

bool exporter::ExportSelectedNPCAccurateCSV(const std::wstring& outPath)
{
    // Current project-contained implementation exports the CS-evaluated voiced race set for the selected response.
    // NPC-accurate export depends on your repo's DataHandler + dialog driver bind/refresh hooks.
    // Implement it in your main CSE tree (same module as DataHandler + dialog hooks) and call into it here.
    return exporter::ExportSelectedToCSV(outPath);
}
