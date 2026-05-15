#pragma once

struct OBSEInterface;
struct PluginInfo;

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) bool __cdecl OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info);
	__declspec(dllexport) bool __cdecl OBSEPlugin_Load(const OBSEInterface* obse);

#ifdef __cplusplus
}
#endif
