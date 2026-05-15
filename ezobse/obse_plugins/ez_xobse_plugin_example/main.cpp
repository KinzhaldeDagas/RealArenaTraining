// User Defines
#include "config.h"

// OBSE
#include "obse/GameAPI.h"
#include "obse/PluginAPI.h"
#include "obse_common/SafeWrite.h"

// Legacy SDK
#include "obse/CommandTable.h"
#include "obse/ParamInfos.h"
#include "obse/GameObjects.h"
#include "obse/GameOSDepend.h"
#include "obse/GameForms.h"
#include "obse/GameData.h"
#include "obse/GameMenus.h"
#include "obse/Script.h"

// C++
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>

// Windows
#include <shlobj.h>

// =========================================================================================
// Log / Handles
// =========================================================================================

IDebugLog gLog(PLUGIN_LOG_FILE);

PluginHandle g_pluginHandle = kPluginHandle_Invalid;
static OBSEInterface* g_obse = NULL;
static OBSESerializationInterface* g_serialization = NULL;
static OBSEMessagingInterface* g_messaging = NULL;

// =========================================================================================
// Constants
// =========================================================================================

#ifndef TOPICBLOCKER_OPCODE_BASE
#define TOPICBLOCKER_OPCODE_BASE 0x2000
#endif

// Hook staging:
// 0 = known-stable trampoline only
// 1 = helper scaffold only, never suppress
// 2 = real rule evaluation, never suppress
// 3 = full live suppression path
//
// Keep default at 0. That is the only branch proven stable in testing.
#ifndef TOPICBLOCKER_HOOK_STAGE
#define TOPICBLOCKER_HOOK_STAGE 0
#endif

static const UInt32 kSerializationVersion = 1;
static const UInt32 kRecordFlags = 'TBCF';
static const UInt32 kRecordLastWS = 'TBCW';

// Loop helpers / hook geometry
static const UInt32 kAddr_GetCurrentTopicEntry = 0x006B8650;
static const UInt32 kAddr_AdvanceTopicIterator = 0x006B85F0;

static const UInt32 kTopicLoopPatchAddr = 0x0059E77D;
static const UInt32 kTopicLoopContinueKeepAddr = 0x0059E78D;
static const UInt32 kTopicLoopContinueSkipAddr = 0x0059E971;
static const UInt32 kTopicLoopPatchLen = 16;

// Dialogue hooks (IDA-validated)
static const UInt32 kAddr_DialogValidateRequiredTiles = 0x0059D8A0;
static const UInt32 kAddr_DialogValidateCallSite = 0x0059EE57;
static const UInt32 kAddr_DialogHandleClick = 0x0059F010;
static const UInt32 kAddr_DialogHandleClickVtableEntry = 0x00A6B898;
static const UInt32 kStriderToggleButtonId = 0x30;
static const char* kPluginDialogTogglesXml = "Data\\Menus\\dialog\\plugin_dialog_toggles.xml";

// =========================================================================================
// Runtime Types
// =========================================================================================

struct TopicEntry
{
	const char* name;
};

struct TopicBlockRule
{
	std::string topic;
	std::string worldspaceEdid;
};

struct TopicBlockerFlags
{
	UInt32 enabled;
	UInt32 debugLogging;
};

// =========================================================================================
// Built-in Rules
// =========================================================================================

static const TopicBlockRule kBuiltInRules[] =
{
	{ "rumor",  "tamriel" },
	{ "rumors", "tamriel" },
	{ "rumor",  "wrldmorrowind" },
	{ "rumors", "wrldmorrowind" },
};

// =========================================================================================
// State
// =========================================================================================

static bool g_topicBlockerEnabled = true;
static bool g_debugLogging = true;
static bool g_hookInstalled = false;

static bool g_dialogHooksInstalled = false;
static bool g_dialogBuildHookInstalled = false;
static bool g_dialogClickHookInstalled = false;
static bool g_striderToggleEnabled = false;
static Menu* g_lastInjectedDialogMenu = NULL;

static std::vector<TopicBlockRule> g_topicBlockRules;
static char g_cachedWorldspaceEdid[128] = { 0 };

typedef bool(__thiscall* _DialogValidateRequiredTiles)(Menu* menu);
typedef void(__thiscall* _DialogHandleClick)(Menu* menu, UInt32 buttonID, Tile* clickedButton);
static _DialogHandleClick g_originalDialogHandleClick = NULL;

// =========================================================================================
// Command Params
// =========================================================================================

static ParamInfo kParams_OneString_Local[1] =
{
	{ "string", kParamType_String, 0 },
};

// =========================================================================================
// Helpers
// =========================================================================================

static std::string ToLowerCopy(const std::string& s)
{
	std::string out = s;
	std::transform(out.begin(), out.end(), out.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return out;
}

static std::string TrimCopy(const std::string& s)
{
	size_t start = 0;
	size_t end = s.size();

	while (start < end && std::isspace(static_cast<unsigned char>(s[start])))
		++start;

	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;

	return s.substr(start, end - start);
}

static std::string CollapseWhitespaceCopy(const std::string& s)
{
	std::string out;
	out.reserve(s.size());

	bool lastWasSpace = false;
	for (size_t i = 0; i < s.size(); ++i)
	{
		const unsigned char ch = static_cast<unsigned char>(s[i]);
		if (std::isspace(ch))
		{
			if (!lastWasSpace)
			{
				out.push_back(' ');
				lastWasSpace = true;
			}
		}
		else
		{
			out.push_back(static_cast<char>(ch));
			lastWasSpace = false;
		}
	}

	return out;
}

static std::string StripWrappingQuotesCopy(const std::string& s)
{
	if (s.size() >= 2)
	{
		const char a = s.front();
		const char b = s.back();
		if ((a == '"' && b == '"') || (a == '\'' && b == '\''))
			return s.substr(1, s.size() - 2);
	}
	return s;
}

static std::string NormalizeToken(const std::string& s)
{
	return ToLowerCopy(CollapseWhitespaceCopy(TrimCopy(StripWrappingQuotesCopy(TrimCopy(s)))));
}

static void NormalizeToBuffer(const char* in, char* out, size_t outSize)
{
	if (!out || outSize == 0)
		return;

	out[0] = '\0';

	if (!in || !*in)
		return;

	size_t write = 0;
	bool lastWasSpace = false;

	while (*in && std::isspace(static_cast<unsigned char>(*in)))
		++in;

	if ((*in == '"' || *in == '\''))
		++in;

	for (; *in && write + 1 < outSize; ++in)
	{
		const unsigned char ch = static_cast<unsigned char>(*in);

		if ((ch == '"' || ch == '\'') && in[1] == '\0')
			break;

		if (std::isspace(ch))
		{
			if (!lastWasSpace)
			{
				out[write++] = ' ';
				lastWasSpace = true;
			}
		}
		else
		{
			out[write++] = static_cast<char>(std::tolower(ch));
			lastWasSpace = false;
		}
	}

	while (write > 0 && out[write - 1] == ' ')
		--write;

	out[write] = '\0';
}

static void ResetRules()
{
	g_topicBlockRules.clear();
}

static void ResetState()
{
	g_topicBlockerEnabled = true;
	g_debugLogging = true;
	g_cachedWorldspaceEdid[0] = '\0';
	ResetRules();
}

static bool RuleExists(const TopicBlockRule& rule)
{
	for (size_t i = 0; i < g_topicBlockRules.size(); ++i)
	{
		if (g_topicBlockRules[i].topic == rule.topic &&
			g_topicBlockRules[i].worldspaceEdid == rule.worldspaceEdid)
		{
			return true;
		}
	}
	return false;
}

static bool LoadBuiltInTopicRules()
{
	ResetRules();

	for (UInt32 i = 0; i < (sizeof(kBuiltInRules) / sizeof(kBuiltInRules[0])); ++i)
	{
		TopicBlockRule rule;
		rule.topic = NormalizeToken(kBuiltInRules[i].topic);
		rule.worldspaceEdid = NormalizeToken(kBuiltInRules[i].worldspaceEdid);

		if (!rule.topic.empty() && !rule.worldspaceEdid.empty() && !RuleExists(rule))
			g_topicBlockRules.push_back(rule);
	}

	if (g_debugLogging)
	{
		_MESSAGE("TopicBlocker: loaded %u built-in rules",
			static_cast<UInt32>(g_topicBlockRules.size()));

		for (size_t i = 0; i < g_topicBlockRules.size(); ++i)
		{
			_MESSAGE("TopicBlocker: rule [%u] topic=\"%s\" worldspace=\"%s\"",
				static_cast<UInt32>(i),
				g_topicBlockRules[i].topic.c_str(),
				g_topicBlockRules[i].worldspaceEdid.c_str());
		}
	}

	return true;
}

static void RefreshCachedWorldspaceFromGameFast()
{
	TES* tes = TES::GetSingleton();
	if (!tes)
		return;

	TESWorldSpace* ws = NULL;

	if (tes->currentExteriorCell && tes->currentExteriorCell->worldSpace)
		ws = tes->currentExteriorCell->worldSpace;
	else if (tes->currentWorldSpace)
		ws = tes->currentWorldSpace;
	else if (tes->currentInteriorCell && tes->currentInteriorCell->worldSpace)
		ws = tes->currentInteriorCell->worldSpace;

	if (!ws)
		return;

	const char* edid = ws->GetEditorID();
	if (!edid || !*edid)
		return;

	char normalized[128] = { 0 };
	NormalizeToBuffer(edid, normalized, sizeof(normalized));
	if (!normalized[0])
		return;

	strcpy_s(g_cachedWorldspaceEdid, sizeof(g_cachedWorldspaceEdid), normalized);
}

static bool ShouldSuppressTopicFast(const char* topicName)
{
	if (!g_topicBlockerEnabled)
		return false;

	if (!topicName || !*topicName)
		return false;

	RefreshCachedWorldspaceFromGameFast();

	if (!g_cachedWorldspaceEdid[0])
		return false;

	char normalizedTopic[256] = { 0 };
	NormalizeToBuffer(topicName, normalizedTopic, sizeof(normalizedTopic));
	if (!normalizedTopic[0])
		return false;

	for (size_t i = 0; i < g_topicBlockRules.size(); ++i)
	{
		const TopicBlockRule& rule = g_topicBlockRules[i];
		if (!_stricmp(rule.topic.c_str(), normalizedTopic) &&
			!_stricmp(rule.worldspaceEdid.c_str(), g_cachedWorldspaceEdid))
		{
			if (g_debugLogging)
			{
				_MESSAGE("TopicBlocker: suppress topic=\"%s\" worldspace=\"%s\"",
					normalizedTopic,
					g_cachedWorldspaceEdid);
			}
			return true;
		}
	}

	return false;
}

static void LogState()
{
	_MESSAGE("TopicBlocker: enabled=%d debug=%d rules=%u cachedWorldspace=\"%s\" hookInstalled=%d hookStage=%d",
		g_topicBlockerEnabled ? 1 : 0,
		g_debugLogging ? 1 : 0,
		static_cast<UInt32>(g_topicBlockRules.size()),
		g_cachedWorldspaceEdid,
		g_hookInstalled ? 1 : 0,
		TOPICBLOCKER_HOOK_STAGE);
}

static Tile* GetDialogButtonLayout(Menu* menu)
{
	if (!menu)
		return NULL;

	Tile* layout = menu->GetComponentByName("dialog_topics\\dialog_button_layout");
	if (!layout)
		layout = menu->GetComponentByName("dialog_button_layout");
	if (!layout && menu->tile)
		layout = menu->tile->GetChildByName("dialog_button_layout");

	return layout;
}

static void InjectDialogTogglesXml(Menu* menu)
{
	if (!menu)
		return;

	if (menu == g_lastInjectedDialogMenu)
		return;

	Tile* buttonLayout = GetDialogButtonLayout(menu);
	if (!buttonLayout)
	{
		_MESSAGE("StriderToggle: dialog_button_layout not found; skipping XML inject");
		return;
	}

	Tile* injected = buttonLayout->ReadXML(kPluginDialogTogglesXml);
	if (!injected)
	{
		_MESSAGE("StriderToggle: ReadXML failed for %s", kPluginDialogTogglesXml);
		return;
	}

	// Ensure traits/actions from the newly attached subtree are processed immediately.
	buttonLayout->DoActionEnumeration();

	g_lastInjectedDialogMenu = menu;
	_MESSAGE("StriderToggle: injected %s for menu=%08X", kPluginDialogTogglesXml, menu);
}

static bool __fastcall DialogValidateAndInject_Hook(Menu* menu, void* edx)
{
	(void)edx;

	_DialogValidateRequiredTiles originalValidate = (_DialogValidateRequiredTiles)kAddr_DialogValidateRequiredTiles;
	const bool ok = originalValidate(menu);
	if (ok)
		InjectDialogTogglesXml(menu);

	return ok;
}

static void __fastcall DialogHandleClick_Hook(Menu* menu, void* edx, UInt32 buttonID, Tile* clickedButton)
{
	(void)edx;

	if (buttonID == kStriderToggleButtonId)
	{
		g_striderToggleEnabled = !g_striderToggleEnabled;
		_MESSAGE("StriderToggle: button id=0x%X toggled state=%d", buttonID, g_striderToggleEnabled ? 1 : 0);
		return;
	}

	if (g_originalDialogHandleClick)
		g_originalDialogHandleClick(menu, buttonID, clickedButton);
}

static bool InstallDialogueHooks()
{
	if (g_dialogHooksInstalled)
		return true;

	if (!g_dialogBuildHookInstalled)
	{
		WriteRelCall(kAddr_DialogValidateCallSite, (UInt32)&DialogValidateAndInject_Hook);
		g_dialogBuildHookInstalled = true;
		_MESSAGE("StriderToggle: installed dialog build hook at %08X", kAddr_DialogValidateCallSite);
	}

	if (!g_dialogClickHookInstalled)
	{
		g_originalDialogHandleClick = (_DialogHandleClick)(*(UInt32*)kAddr_DialogHandleClickVtableEntry);
		if (!g_originalDialogHandleClick)
			g_originalDialogHandleClick = (_DialogHandleClick)kAddr_DialogHandleClick;

		SafeWrite32(kAddr_DialogHandleClickVtableEntry, (UInt32)&DialogHandleClick_Hook);
		g_dialogClickHookInstalled = true;
		_MESSAGE("StriderToggle: installed dialog click hook vtbl[%08X] old=%08X new=%08X",
			kAddr_DialogHandleClickVtableEntry,
			(UInt32)g_originalDialogHandleClick,
			(UInt32)&DialogHandleClick_Hook);
	}

	g_dialogHooksInstalled = g_dialogBuildHookInstalled && g_dialogClickHookInstalled;
	return g_dialogHooksInstalled;
}

// =========================================================================================
// Serialization
// =========================================================================================

static void TopicBlocker_SaveCallback(void* reserved)
{
	if (!g_serialization)
		return;

	TopicBlockerFlags flags;
	flags.enabled = g_topicBlockerEnabled ? 1 : 0;
	flags.debugLogging = g_debugLogging ? 1 : 0;

	g_serialization->OpenRecord(kRecordFlags, kSerializationVersion);
	g_serialization->WriteRecordData(&flags, sizeof(flags));

	g_serialization->OpenRecord(kRecordLastWS, kSerializationVersion);
	if (g_cachedWorldspaceEdid[0])
	{
		g_serialization->WriteRecordData(
			g_cachedWorldspaceEdid,
			static_cast<UInt32>(strlen(g_cachedWorldspaceEdid)));
	}
}

static void TopicBlocker_LoadCallback(void* reserved)
{
	if (!g_serialization)
		return;

	ResetState();

	UInt32 type = 0;
	UInt32 version = 0;
	UInt32 length = 0;

	while (g_serialization->GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
		case kRecordFlags:
		{
			TopicBlockerFlags flags;
			memset(&flags, 0, sizeof(flags));
			if (g_serialization->ReadRecordData(&flags, sizeof(flags)) == sizeof(flags))
			{
				g_topicBlockerEnabled = (flags.enabled != 0);
				g_debugLogging = (flags.debugLogging != 0);
			}
		}
		break;

		case kRecordLastWS:
		{
			std::vector<char> buffer(length + 1, 0);
			if (length > 0)
				g_serialization->ReadRecordData(&buffer[0], length);
			buffer[length] = '\0';

			char normalized[128] = { 0 };
			NormalizeToBuffer(&buffer[0], normalized, sizeof(normalized));
			if (normalized[0])
				strcpy_s(g_cachedWorldspaceEdid, sizeof(g_cachedWorldspaceEdid), normalized);
		}
		break;

		default:
		{
			if (length > 0)
			{
				std::vector<char> skip(length);
				g_serialization->ReadRecordData(&skip[0], length);
			}
		}
		break;
		}
	}

	LoadBuiltInTopicRules();
	RefreshCachedWorldspaceFromGameFast();
	LogState();
}

static void TopicBlocker_NewGameCallback(void* reserved)
{
	ResetState();
	LoadBuiltInTopicRules();
	RefreshCachedWorldspaceFromGameFast();
	LogState();
}

// =========================================================================================
// Hook
// =========================================================================================

static UInt32 __stdcall TopicBlocker_ShouldSuppressTopic_Hook(const char* topicName)
{
#if TOPICBLOCKER_HOOK_STAGE == 1
	(void)topicName;
	return 0u;
#elif TOPICBLOCKER_HOOK_STAGE == 2
	(void)ShouldSuppressTopicFast(topicName);
	return 0u;
#else
	return ShouldSuppressTopicFast(topicName) ? 1u : 0u;
#endif
}

static __declspec(naked) void TopicBlocker_TopicLoopHook(void)
{
#if TOPICBLOCKER_HOOK_STAGE == 0

	__asm
	{
		// Known-stable trampoline of the overwritten 16-byte block.
		mov ecx, [esp + 0x2C]
		mov edx, [edi + 0x28]
		push esi
		push ecx
		push edx
		mov ecx, edi
		mov[esp + 0x4C], ebx
		jmp kTopicLoopContinueKeepAddr
	}

#else

	__asm
	{
		// Preserve x87 state before calling helper logic.
		sub esp, 0x6C
		fnsave[esp]

		// Preserve GPRs while resolving current topic name.
		pushad

		// saved EAX slot in PUSHAD frame = suppress flag
		xor eax, eax
		mov[esp + 0x1C], eax

		// current topic entry = sub_6B8650(EBP)
		mov ecx, ebp
		mov eax, kAddr_GetCurrentTopicEntry
		call eax
		test eax, eax
		jz decide_done

		mov eax, [eax]
		test eax, eax
		jz decide_done

		push eax
		call TopicBlocker_ShouldSuppressTopic_Hook
		test eax, eax
		jz decide_done

		mov dword ptr[esp + 0x1C], 1

		decide_done:
		popad

			frstor[esp]
			add esp, 0x6C

			test eax, eax
			jnz skip_path

			keep_path :
		mov ecx, [esp + 0x2C]
			mov edx, [edi + 0x28]
			push esi
			push ecx
			push edx
			mov ecx, edi
			mov[esp + 0x4C], ebx
			jmp kTopicLoopContinueKeepAddr

			skip_path :
#if TOPICBLOCKER_HOOK_STAGE < 3
		// Stages 1-2 never actually suppress; force keep path.
		mov ecx, [esp + 0x2C]
			mov edx, [edi + 0x28]
			push esi
			push ecx
			push edx
			mov ecx, edi
			mov[esp + 0x4C], ebx
			jmp kTopicLoopContinueKeepAddr
#else
		// This branch remains unproven stable in current testing.
		mov[esp + 0x40], ebx
			mov ecx, ebp
			mov eax, kAddr_AdvanceTopicIterator
			call eax
			jmp kTopicLoopContinueSkipAddr
#endif
	}

#endif
}

static bool InstallTopicHook()
{
	if (g_hookInstalled)
		return true;

	WriteRelJump(kTopicLoopPatchAddr, (UInt32)&TopicBlocker_TopicLoopHook);

	for (UInt32 i = 5; i < kTopicLoopPatchLen; ++i)
		SafeWrite8(kTopicLoopPatchAddr + i, 0x90);

	g_hookInstalled = true;

	_MESSAGE("TopicBlocker: installed hook stage %d at %08X keep=%08X skip=%08X",
		TOPICBLOCKER_HOOK_STAGE,
		kTopicLoopPatchAddr,
		kTopicLoopContinueKeepAddr,
		kTopicLoopContinueSkipAddr);

	return true;
}

// =========================================================================================
// Runtime Commands
// =========================================================================================

#ifdef RUNTIME

bool Cmd_TopicBlocker_SetEnabled_Execute(COMMAND_ARGS)
{
	UInt32 enabled = 0;
	*result = 0.0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &enabled))
	{
		g_topicBlockerEnabled = (enabled != 0);
		*result = 1.0;
	}

	return true;
}

bool Cmd_TopicBlocker_GetEnabled_Execute(COMMAND_ARGS)
{
	*result = g_topicBlockerEnabled ? 1.0 : 0.0;
	return true;
}

bool Cmd_TopicBlocker_ReloadRules_Execute(COMMAND_ARGS)
{
	const bool ok = LoadBuiltInTopicRules();
	RefreshCachedWorldspaceFromGameFast();
	*result = ok ? 1.0 : 0.0;
	return true;
}

bool Cmd_TopicBlocker_LogState_Execute(COMMAND_ARGS)
{
	LogState();
	*result = 1.0;
	return true;
}

bool Cmd_TopicBlocker_TestRule_Execute(COMMAND_ARGS)
{
	char topic[512] = { 0 };
	*result = 0.0;

	if (ExtractArgs(PASS_EXTRACT_ARGS, &topic))
	{
		RefreshCachedWorldspaceFromGameFast();
		*result = ShouldSuppressTopicFast(topic) ? 1.0 : 0.0;
	}

	return true;
}

#endif

// =========================================================================================
// Command Definitions
// =========================================================================================

DEFINE_COMMAND_PLUGIN(TopicBlocker_SetEnabled, "Enable or disable TopicBlocker", 0, 1, kParams_OneInt)
DEFINE_COMMAND_PLUGIN(TopicBlocker_GetEnabled, "Returns 1 if TopicBlocker is enabled", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(TopicBlocker_ReloadRules, "Reload built-in topic rules", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(TopicBlocker_LogState, "Log TopicBlocker state and rules", 0, 0, NULL)
DEFINE_COMMAND_PLUGIN(TopicBlocker_TestRule, "Test current topic suppression for a topic string against effective worldspace", 0, 1, kParams_OneString_Local)

// =========================================================================================
// Messaging
// =========================================================================================

static void TopicBlocker_MessageHandler(OBSEMessagingInterface::Message* msg)
{
	if (!msg)
		return;

	switch (msg->type)
	{
	case OBSEMessagingInterface::kMessage_PostLoad:
		g_lastInjectedDialogMenu = NULL;
		RefreshCachedWorldspaceFromGameFast();
		break;

	case OBSEMessagingInterface::kMessage_LoadGame:
		g_lastInjectedDialogMenu = NULL;
		LoadBuiltInTopicRules();
		RefreshCachedWorldspaceFromGameFast();
		break;

	case OBSEMessagingInterface::kMessage_ExitToMainMenu:
		g_lastInjectedDialogMenu = NULL;
		break;

	default:
		break;
	}
}

// =========================================================================================
// Compatibility
// =========================================================================================

static bool IsCompatible(const OBSEInterface* obse)
{
	if (obse->isEditor)
	{
		if (obse->editorVersion < SUPPORTED_RUNTIME_VERSION_CS)
		{
			_MESSAGE("ERROR::IsCompatible: editor version too old (got %08X need >= %08X)",
				obse->editorVersion, SUPPORTED_RUNTIME_VERSION_CS);
			return false;
		}
	}
	else
	{
		if (!IVersionCheck::IsCompatibleVersion(
			obse->oblivionVersion,
			MINIMUM_RUNTIME_VERSION,
			SUPPORTED_RUNTIME_VERSION,
			SUPPORTED_RUNTIME_VERSION_STRICT))
		{
			_MESSAGE("ERROR::IsCompatible: incompatible runtime version");
			return false;
		}
	}

	return true;
}

// =========================================================================================
// Exports
// =========================================================================================

extern "C"
{

	bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, PLUGIN_LOG_FILE);

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME_LONG;
		info->version = PLUGIN_VERSION_DLL;

		_MESSAGE(PLUGIN_VERSION_INFO);
		_MESSAGE("TopicBlocker: query");

		if (!IsCompatible(obse))
			return false;

		if (!obse->isEditor)
		{
			g_obse = const_cast<OBSEInterface*>(obse);

			g_serialization = (OBSESerializationInterface*)obse->QueryInterface(kInterface_Serialization);
			if (!g_serialization)
			{
				_ERROR("TopicBlocker: serialization interface not found");
				return false;
			}

			if (g_serialization->version < OBSESerializationInterface::kVersion)
			{
				_ERROR("TopicBlocker: incorrect serialization version found");
				return false;
			}
		}

		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface* obse)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, PLUGIN_LOG_FILE);

		_MESSAGE(PLUGIN_VERSION_INFO);
		_MESSAGE("TopicBlocker: load");

		if (!IsCompatible(obse))
			return false;

		g_obse = const_cast<OBSEInterface*>(obse);
		g_pluginHandle = obse->GetPluginHandle();

		ResetState();
		LoadBuiltInTopicRules();
		RefreshCachedWorldspaceFromGameFast();

		obse->SetOpcodeBase(TOPICBLOCKER_OPCODE_BASE);

		if (TOPICBLOCKER_OPCODE_BASE == 0x2000)
		{
			_MESSAGE("WARNING: TopicBlocker is using default opcode base 0x2000. Replace before release.");
		}

		if (!obse->RegisterCommand(&kCommandInfo_TopicBlocker_SetEnabled))
			_ERROR("TopicBlocker: failed to register TopicBlocker_SetEnabled");
		if (!obse->RegisterCommand(&kCommandInfo_TopicBlocker_GetEnabled))
			_ERROR("TopicBlocker: failed to register TopicBlocker_GetEnabled");
		if (!obse->RegisterCommand(&kCommandInfo_TopicBlocker_ReloadRules))
			_ERROR("TopicBlocker: failed to register TopicBlocker_ReloadRules");
		if (!obse->RegisterCommand(&kCommandInfo_TopicBlocker_LogState))
			_ERROR("TopicBlocker: failed to register TopicBlocker_LogState");
		if (!obse->RegisterCommand(&kCommandInfo_TopicBlocker_TestRule))
			_ERROR("TopicBlocker: failed to register TopicBlocker_TestRule");

		if (!obse->isEditor)
		{
			g_serialization = (OBSESerializationInterface*)obse->QueryInterface(kInterface_Serialization);
			g_messaging = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);

			if (g_serialization)
			{
				g_serialization->SetSaveCallback(g_pluginHandle, TopicBlocker_SaveCallback);
				g_serialization->SetLoadCallback(g_pluginHandle, TopicBlocker_LoadCallback);
				g_serialization->SetNewGameCallback(g_pluginHandle, TopicBlocker_NewGameCallback);
			}

			if (g_messaging)
			{
				g_messaging->RegisterListener(g_pluginHandle, "OBSE", TopicBlocker_MessageHandler);
			}

			if (!InstallTopicHook())
			{
				_ERROR("TopicBlocker: failed to install live hook");
				return false;
			}

			if (!InstallDialogueHooks())
			{
				_ERROR("StriderToggle: failed to install dialogue hooks");
				return false;
			}
		}

		LogState();
		_MESSAGE("TopicBlocker: loaded");
		return true;
	}

} // extern "C"
