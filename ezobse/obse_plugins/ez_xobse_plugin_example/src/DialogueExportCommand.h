#pragma once
#include <string>
#include <vector>

namespace exporter
{
    bool ExportSelectedToCSV(const std::wstring& outPath);

    bool ExportReVoiceCSV(const std::wstring& outPath, const std::vector<unsigned char>& scopeModIndices);
    bool ExportReVoiceCSV(const std::wstring& outPath);

    // Convenience wrapper for the Option B menu: export for exactly one loaded plugin (modIndex).
    bool ExportReVoiceCSVForModIndex(const std::wstring& outPath, unsigned char modIndex);
}
