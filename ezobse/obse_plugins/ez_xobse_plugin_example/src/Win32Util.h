#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace win32
{
    std::wstring GetModuleDir(HMODULE h);
    std::wstring JoinPath(const std::wstring& a, const std::wstring& b);
    bool EnsureDirForFile(const std::wstring& filePath);

    // NEW
    std::wstring GetExeDir();                 // folder containing TESConstructionSet.exe
    std::wstring GetDataDir();                // <ExeDir>\Data
    std::wstring GetDocumentsDir();           // CSIDL_MYDOCUMENTS
    std::wstring GetLocalAppDataDir();        // CSIDL_LOCAL_APPDATA

    std::string  WideToUtf8(const std::wstring& w);
    std::wstring Utf8ToWide(const char* s);

    bool TryTouchFile(const std::wstring& path);     // create/append test
    std::wstring BaseName(const std::wstring& path); // filename.ext
}
