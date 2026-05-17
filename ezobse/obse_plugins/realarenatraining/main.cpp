#include "config.h"

#include "obse/GameActorValues.h"
#include "obse/GameAPI.h"
#include "obse/GameExtraData.h"
#include "obse/GameForms.h"
#include "obse/GameObjects.h"
#include "obse/GameProcess.h"
#include "obse/PluginAPI.h"
#include "obse_common/SafeWrite.h"

#include <shlobj.h>
#include <string.h>
#include <windows.h>

PluginHandle g_pluginHandle = kPluginHandle_Invalid;
extern "C" UInt32 RealArenaTraining_ScriptAddEventAddress = 0x004FBF90;
extern "C" UInt32 RealArenaTraining_StaticMeleeProbeAddress = 0x005F9620;
extern "C" UInt32 RealArenaTraining_StaticMeleeImpactAddress = 0x005F05F0;
extern "C" UInt32 RealArenaTraining_ArrowCollisionFunctionAddress = 0x0060B120;
extern "C" volatile UInt32 RealArenaTraining_StaticAttackProbeDepth = 0;
extern "C" volatile UInt32 RealArenaTraining_StaticAttackProbeSequence = 0;

namespace
{
	enum RuntimeAddress
	{
		kAddr_ScriptAddEventToExtraScript = 0x004FBF90,
		kAddr_StaticMeleeProbeFunction = 0x005F9620,
		kAddr_StaticMeleeImpactFunction = 0x005F05F0,
		kAddr_ArrowCollisionFunction = 0x0060B120,
		kAddr_ActorValueGetGroupOffsetFromAV = 0x00565C50,
		kAddr_PlayerIncreaseSkillExperience = 0x00668B30,
		kAddr_PlayerModExperience = 0x00668C30,
		kAddr_StaticMeleeImpactCall = 0x005F9AC3,
		kAddr_StaticMeleeObjectEventCall = 0x005F9E62,
		kAddr_StaticMeleeAttackProbeCall = 0x005FF05D,
		kAddr_MeleeOnHitScriptEventCall = 0x005FF630,
		kAddr_ArrowOnHitWithScriptEventCall = 0x0060B387,
		kAddr_ArrowStaticObjectEventCall = 0x0060B8D1,
		kAddr_ArrowCollisionCall = 0x0060DB6B,
	};

	enum ScriptEventMask
	{
		kEvent_OnHit = 0x00000080,
		kEvent_OnHitWith = 0x00000100,
		kEvent_StaticReachProbe = 0x10000000,
	};

	enum TargetKind
	{
		kTarget_None = 0,
		kTarget_ArenaHeavyBag,
		kTarget_ChainDoll,
		kTarget_ArenaDummy1,
		kTarget_TargetHay,
	};

	const float kTrainingProgress_Normal = 1.0f;
	const float kTrainingProgress_HalfRate = 0.5f;
	const float kTrainingProgress_HalfPercentBonus = 1.005f;
	const float kTrainingProgress_ArenaDummyBladeBonus = 1.5f;
	const float kTrainingProgress_HeavyBagBladeBluntPenalty = 0.96f;
	const float kTrainingProgress_TargetHayBladeBluntPenalty = 0.98f;

	enum TrainingModelPathFingerprint
	{
		kModelHash_ArenaHeavyBag01 = 0x90A29F11,
		kModelHash_ChainDollArena01 = 0xD6B42764,
		kModelHash_ChainDoll01 = 0x4970EBAF,
		kModelHash_ChainDoll02 = 0x8432F02E,
		kModelHash_TargetHeavy01 = 0x5D13CC8B,
		kModelHash_TargetHay01 = 0xD1FB87BE,
	};

	enum TrainingModelPathLength
	{
		kModelLen_ArenaHeavyBag01 = 38,
		kModelLen_ChainDollArena01 = 39,
		kModelLen_ChainDoll01 = 37,
		kModelLen_ChainDoll02 = 37,
		kModelLen_TargetHeavy01 = 39,
		kModelLen_TargetHay01 = 37,
	};

	struct CallPatch
	{
		UInt32 callSite;
		UInt32 originalRel;
		bool installed;
	};

	typedef UInt8 (__cdecl * ScriptAddEventFn)(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask);
	typedef UInt8 (__cdecl * ActorValueGetGroupOffsetFromAVFn)(UInt32 group, UInt32 actorValue);
	typedef void (__thiscall * PlayerModExperienceFn)(PlayerCharacter* player, UInt32 actorValue, UInt32 useIndex, float baseDelta);
	typedef void (__thiscall * PlayerIncreaseSkillExperienceFn)(
		PlayerCharacter* player,
		UInt32 actorValue,
		float progress,
		TESSkill* skill,
		UInt8 suppressLevelUp);

	CallPatch g_staticMeleePatch = { kAddr_StaticMeleeObjectEventCall, 0, false };
	CallPatch g_staticMeleeAttackProbePatch = { kAddr_StaticMeleeAttackProbeCall, 0, false };
	CallPatch g_staticMeleeImpactPatch = { kAddr_StaticMeleeImpactCall, 0, false };
	CallPatch g_meleeOnHitPatch = { kAddr_MeleeOnHitScriptEventCall, 0, false };
	CallPatch g_arrowOnHitWithPatch = { kAddr_ArrowOnHitWithScriptEventCall, 0, false };
	CallPatch g_arrowStaticPatch = { kAddr_ArrowStaticObjectEventCall, 0, false };
	CallPatch g_arrowCollisionPatch = { kAddr_ArrowCollisionCall, 0, false };
	ScriptAddEventFn g_scriptAddEvent = reinterpret_cast<ScriptAddEventFn>(kAddr_ScriptAddEventToExtraScript);
	ActorValueGetGroupOffsetFromAVFn g_actorValueGetGroupOffsetFromAV =
		reinterpret_cast<ActorValueGetGroupOffsetFromAVFn>(kAddr_ActorValueGetGroupOffsetFromAV);
	PlayerModExperienceFn g_playerModExperience = reinterpret_cast<PlayerModExperienceFn>(kAddr_PlayerModExperience);
	PlayerIncreaseSkillExperienceFn g_playerIncreaseSkillExperience =
		reinterpret_cast<PlayerIncreaseSkillExperienceFn>(kAddr_PlayerIncreaseSkillExperience);
	UInt32 g_runtimeVersion = 0;

	bool g_logOpened = false;
	UInt32 g_lastArrowTrainingProjectile = 0;
	UInt32 g_lastArrowTrainingTarget = 0;
	UInt32 g_lastArrowTrainingSkill = 0;
	UInt32 g_lastStaticTrainingProbeSequence = 0;
	UInt32 g_lastStaticTrainingTarget = 0;
	UInt32 g_lastStaticTrainingSkill = 0;
	UInt32 g_trainingLogCount = 0;
	UInt32 g_candidateLogCount = 0;
	UInt32 g_rejectedTrainingLogCount = 0;
	UInt32 g_hookEntryLogCount = 0;

	void OpenPluginLogOnce()
	{
		if (!g_logOpened)
		{
			gLog.OpenRelative(CSIDL_MYDOCUMENTS, PLUGIN_LOG_FILE);
			g_logOpened = true;
		}
	}

	char NormalizeModelPathChar(char value)
	{
		if (value == '/')
			return '\\';
		if (value >= 'A' && value <= 'Z')
			return value + ('a' - 'A');
		return value;
	}

	UInt32 ModelPathHash(const char* path)
	{
		UInt32 hash = 0x811C9DC5;
		if (!path)
			return hash;

		for (const char* cur = path; *cur; ++cur)
		{
			hash ^= static_cast<UInt8>(NormalizeModelPathChar(*cur));
			hash *= 0x01000193;
		}

		return hash;
	}

	UInt32 ModelPathNormalizedLength(const char* path)
	{
		return path ? static_cast<UInt32>(strlen(path)) : 0;
	}

	bool ModelPathMatches(const char* path, UInt32 expectedHash, UInt32 expectedLength)
	{
		return path &&
			ModelPathNormalizedLength(path) == expectedLength &&
			ModelPathHash(path) == expectedHash;
	}

	UInt32 PtrValue(const void* ptr)
	{
		return reinterpret_cast<UInt32>(ptr);
	}

	UInt32 RefId(const TESForm* form)
	{
		return form ? form->refID : 0;
	}

	const char* SkillName(UInt32 actorValue)
	{
		switch (actorValue)
		{
			case kActorVal_Blade: return "Blade";
			case kActorVal_Blunt: return "Blunt";
			case kActorVal_HandToHand: return "HandToHand";
			case kActorVal_Marksman: return "Marksman";
			default: return "Unknown";
		}
	}

	const char* WeaponTypeName(TESObjectWEAP* weapon)
	{
		if (!weapon)
			return "None";

		switch (weapon->type)
		{
			case TESObjectWEAP::kType_BladeOneHand: return "BladeOneHand";
			case TESObjectWEAP::kType_BladeTwoHand: return "BladeTwoHand";
			case TESObjectWEAP::kType_BluntOneHand: return "BluntOneHand";
			case TESObjectWEAP::kType_BluntTwoHand: return "BluntTwoHand";
			case TESObjectWEAP::kType_Staff: return "Staff";
			case TESObjectWEAP::kType_Bow: return "Bow";
			default: return "Unknown";
		}
	}

	TESObjectREFR* RefFromExtraList(ExtraDataList* extraList)
	{
		if (!extraList)
			return 0;

		return reinterpret_cast<TESObjectREFR*>(reinterpret_cast<UInt8*>(extraList) - 0x44);
	}

	TESObjectREFR* CurrentInterfaceCrosshairTarget()
	{
		InterfaceManager* interfaceManager = InterfaceManager::GetSingleton();
		return interfaceManager ? reinterpret_cast<TESObjectREFR*>(interfaceManager->unk0C0[2]) : 0;
	}

	const TESModel* ModelForBaseForm(TESForm* form)
	{
		if (!form)
			return 0;

		switch (form->GetFormType())
		{
			case kFormType_Stat:
				return &static_cast<const TESObjectSTAT*>(form)->model;

			default:
				return 0;
		}
	}

	const char* ModelPathForRef(TESObjectREFR* ref)
	{
		if (!ref)
			return 0;

		TESForm* baseForm = ref->baseForm;
		const TESModel* model = ModelForBaseForm(baseForm);
		return model ? model->nifPath.m_data : 0;
	}

	TargetKind GetTrainingTargetKind(TESObjectREFR* ref)
	{
		if (!ref || !ref->baseForm || ref->baseForm->GetFormType() != kFormType_Stat)
			return kTarget_None;

		const char* path = ModelPathForRef(ref);
		if (!path)
			return kTarget_None;

		if (ModelPathMatches(path, kModelHash_ArenaHeavyBag01, kModelLen_ArenaHeavyBag01))
			return kTarget_ArenaHeavyBag;

		if (ModelPathMatches(path, kModelHash_ChainDollArena01, kModelLen_ChainDollArena01) ||
			ModelPathMatches(path, kModelHash_ChainDoll01, kModelLen_ChainDoll01) ||
			ModelPathMatches(path, kModelHash_ChainDoll02, kModelLen_ChainDoll02))
		{
			return kTarget_ChainDoll;
		}

		if (ModelPathMatches(path, kModelHash_TargetHeavy01, kModelLen_TargetHeavy01))
			return kTarget_ArenaDummy1;

		if (ModelPathMatches(path, kModelHash_TargetHay01, kModelLen_TargetHay01))
			return kTarget_TargetHay;

		return kTarget_None;
	}

	bool IsMeleeTrainingTarget(TargetKind kind)
	{
		return kind == kTarget_ArenaHeavyBag ||
			kind == kTarget_ChainDoll ||
			kind == kTarget_ArenaDummy1 ||
			kind == kTarget_TargetHay;
	}

	bool IsArcheryTrainingTarget(TargetKind kind)
	{
		return kind == kTarget_TargetHay;
	}

	bool CanTrainMeleeSkill(TargetKind kind, UInt32 actorValue)
	{
		switch (kind)
		{
			case kTarget_ArenaHeavyBag:
			case kTarget_ChainDoll:
				return actorValue == kActorVal_HandToHand ||
					actorValue == kActorVal_Blade ||
					actorValue == kActorVal_Blunt;

			case kTarget_ArenaDummy1:
			case kTarget_TargetHay:
				return actorValue == kActorVal_Blade ||
					actorValue == kActorVal_Blunt;

			default:
				return false;
		}
	}

	TESObjectWEAP* GetEquippedPlayerWeapon()
	{
		PlayerCharacter* player = (g_thePlayer && *g_thePlayer) ? *g_thePlayer : 0;
		if (!player || !player->process)
			return 0;

		ExtraContainerChanges::EntryData* weaponData = player->process->GetEquippedWeaponData(true);
		TESForm* weaponForm = weaponData ? weaponData->type : 0;
		if (!weaponForm || weaponForm->GetFormType() != kFormType_Weapon)
			return 0;

		return static_cast<TESObjectWEAP*>(weaponForm);
	}

	UInt32 SkillForEquippedPlayerMeleeWeapon()
	{
		TESObjectWEAP* weapon = GetEquippedPlayerWeapon();
		if (!weapon)
			return kActorVal_HandToHand;

		switch (weapon->type)
		{
			case TESObjectWEAP::kType_BladeOneHand:
			case TESObjectWEAP::kType_BladeTwoHand:
				return kActorVal_Blade;

			case TESObjectWEAP::kType_BluntOneHand:
			case TESObjectWEAP::kType_BluntTwoHand:
				return kActorVal_Blunt;

			default:
				return kActorVal_NoActorValue;
		}
	}

	bool IsPlayer(TESForm* form)
	{
		return g_thePlayer && *g_thePlayer && form == reinterpret_cast<TESForm*>(*g_thePlayer);
	}

	bool IsPlayer(Actor* actor)
	{
		return g_thePlayer && *g_thePlayer && actor == static_cast<Actor*>(*g_thePlayer);
	}

	bool IsStaticAttackProbeActive()
	{
		return RealArenaTraining_StaticAttackProbeDepth != 0;
	}

	bool PlayerBaseLuckIsTen()
	{
		PlayerCharacter* player = (g_thePlayer && *g_thePlayer) ? *g_thePlayer : 0;
		return player && player->GetBaseActorValue(kActorVal_Luck) == 10;
	}

	float TrainingProgressMultiplier(TargetKind kind, UInt32 actorValue)
	{
		switch (kind)
		{
			case kTarget_ArenaHeavyBag:
				if (actorValue == kActorVal_HandToHand)
					return kTrainingProgress_HalfPercentBonus;
				if (actorValue == kActorVal_Blade || actorValue == kActorVal_Blunt)
					return kTrainingProgress_HeavyBagBladeBluntPenalty;
				break;

			case kTarget_ChainDoll:
				if (actorValue == kActorVal_Blade)
					return kTrainingProgress_HalfRate;
				if (actorValue == kActorVal_Blunt)
					return kTrainingProgress_HalfPercentBonus;
				break;

			case kTarget_ArenaDummy1:
				if (actorValue == kActorVal_Blade)
					return kTrainingProgress_ArenaDummyBladeBonus;
				if (actorValue == kActorVal_Blunt)
					return PlayerBaseLuckIsTen() ? kTrainingProgress_Normal : kTrainingProgress_HalfRate;
				break;

			case kTarget_TargetHay:
				if (actorValue == kActorVal_Marksman)
					return kTrainingProgress_HalfPercentBonus;
				if (actorValue == kActorVal_Blade || actorValue == kActorVal_Blunt)
					return kTrainingProgress_TargetHayBladeBluntPenalty;
				break;

			default:
				break;
		}

		return kTrainingProgress_Normal;
	}

	float TrainingProgressForTargetSkill(TESObjectREFR* target, UInt32 actorValue)
	{
		return REALARENATRAINING_SKILL_PROGRESS_PER_INTERACTION *
			TrainingProgressMultiplier(GetTrainingTargetKind(target), actorValue);
	}

	SInt32 SkillOffset(UInt32 actorValue)
	{
		if (!IsSkill(actorValue))
			return -1;

		const UInt8 offset = g_actorValueGetGroupOffsetFromAV(2, actorValue);
		return (offset < 21) ? offset : -1;
	}

	float SkillProgressForPlayer(PlayerCharacter* player, UInt32 actorValue)
	{
		const SInt32 offset = SkillOffset(actorValue);
		return (player && offset >= 0) ? player->skillExp[offset] : 0.0f;
	}

	float RequiredSkillProgressForPlayer(PlayerCharacter* player, UInt32 actorValue)
	{
		const SInt32 offset = SkillOffset(actorValue);
		return (player && offset >= 0) ? player->requiredSkillExp[offset] : 0.0f;
	}

	bool ShouldRejectDuplicateArrowTraining(ArrowProjectile* arrow, TESObjectREFR* target, UInt32 actorValue)
	{
		const UInt32 arrowValue = PtrValue(arrow);
		const UInt32 targetValue = PtrValue(target);

		if (arrowValue &&
			g_lastArrowTrainingProjectile == arrowValue &&
			g_lastArrowTrainingTarget == targetValue &&
			g_lastArrowTrainingSkill == actorValue)
		{
			return true;
		}

		g_lastArrowTrainingProjectile = arrowValue;
		g_lastArrowTrainingTarget = targetValue;
		g_lastArrowTrainingSkill = actorValue;
		return false;
	}

	void NoteStaticMeleeTrainingAward(TESObjectREFR* target, UInt32 actorValue)
	{
		g_lastStaticTrainingProbeSequence = RealArenaTraining_StaticAttackProbeSequence;
		g_lastStaticTrainingTarget = PtrValue(target);
		g_lastStaticTrainingSkill = actorValue;
	}

	bool WasStaticMeleeTrainingAwardedThisProbe(TESObjectREFR* target, UInt32 actorValue)
	{
		return g_lastStaticTrainingProbeSequence == RealArenaTraining_StaticAttackProbeSequence &&
			g_lastStaticTrainingTarget == PtrValue(target) &&
			g_lastStaticTrainingSkill == actorValue;
	}

	void LogCandidate(TESObjectREFR* target, UInt32 eventMask, const char* reason)
	{
#if REALARENATRAINING_ENABLE_CANDIDATE_LOGGING
		if (g_candidateLogCount >= REALARENATRAINING_MAX_CANDIDATE_LOGS)
			return;

		++g_candidateLogCount;
		_MESSAGE(
			"RealArenaTraining:%s event=0x%08X target=0x%08X targetRef=%08X baseRef=%08X baseType=%u model=\"%s\"",
			reason,
			eventMask,
			PtrValue(target),
			RefId(target),
			RefId(target ? target->baseForm : 0),
			target && target->baseForm ? target->baseForm->GetFormType() : 0,
			ModelPathForRef(target) ? ModelPathForRef(target) : "");
#else
		(void)target;
		(void)eventMask;
		(void)reason;
#endif
	}

	void LogTraining(
		TESObjectREFR* target,
		UInt32 actorValue,
		const char* reason,
		float baseProgress,
		float multiplier,
		float appliedProgress,
		float beforeProgress,
		float afterProgress,
		float requiredProgress,
		UInt32 beforeLevel,
		UInt32 afterLevel)
	{
#if REALARENATRAINING_ENABLE_TRAINING_LOGGING
		if (g_trainingLogCount >= REALARENATRAINING_MAX_TRAINING_LOGS)
			return;

		++g_trainingLogCount;
		_MESSAGE(
			"RealArenaTraining:%s target=0x%08X targetRef=%08X baseRef=%08X model=\"%s\" skill=%s(%u) base=%.4f multiplier=%.4f applied=%.4f progress=%.4f->%.4f required=%.4f level=%u->%u delta=%.4f",
			reason,
			PtrValue(target),
			RefId(target),
			RefId(target ? target->baseForm : 0),
			ModelPathForRef(target) ? ModelPathForRef(target) : "",
			SkillName(actorValue),
			actorValue,
			baseProgress,
			multiplier,
			appliedProgress,
			beforeProgress,
			afterProgress,
			requiredProgress,
			beforeLevel,
			afterLevel,
			afterProgress - beforeProgress);
#else
		(void)target;
		(void)actorValue;
		(void)reason;
		(void)baseProgress;
		(void)multiplier;
		(void)appliedProgress;
		(void)beforeProgress;
		(void)afterProgress;
		(void)requiredProgress;
		(void)beforeLevel;
		(void)afterLevel;
#endif
	}

	void LogRejectedTraining(TESObjectREFR* target, UInt32 actorValue, const char* reason)
	{
#if REALARENATRAINING_ENABLE_TRAINING_LOGGING
		if (g_rejectedTrainingLogCount >= REALARENATRAINING_MAX_REJECTED_TRAINING_LOGS)
			return;

		++g_rejectedTrainingLogCount;
		TESObjectWEAP* weapon = GetEquippedPlayerWeapon();
		_MESSAGE(
			"RealArenaTraining:%s target=0x%08X targetRef=%08X baseRef=%08X model=\"%s\" skill=%s(%u) weapon=0x%08X weaponRef=%08X weaponType=%s(%u)",
			reason,
			PtrValue(target),
			RefId(target),
			RefId(target ? target->baseForm : 0),
			ModelPathForRef(target) ? ModelPathForRef(target) : "",
			SkillName(actorValue),
			actorValue,
			PtrValue(weapon),
			RefId(weapon),
			WeaponTypeName(weapon),
			weapon ? weapon->type : 0xFFFFFFFF);
#else
		(void)target;
		(void)actorValue;
		(void)reason;
#endif
	}

	void LogHookEntry(
		const char* label,
		TESForm* source,
		ExtraDataList* targetExtra,
		UInt32 eventMask,
		ArrowProjectile* arrow,
		TESObjectREFR* registerTarget)
	{
#if REALARENATRAINING_ENABLE_HOOK_ENTRY_LOGGING
		if (g_hookEntryLogCount >= REALARENATRAINING_MAX_HOOK_ENTRY_LOGS)
			return;

		++g_hookEntryLogCount;
		TESObjectREFR* extraTarget = RefFromExtraList(targetExtra);
		TESObjectREFR* target = registerTarget ? registerTarget : extraTarget;
		Actor* shooter = arrow ? arrow->shooter : 0;
		_MESSAGE(
			"RealArenaTraining:%s hook event=0x%08X source=0x%08X sourceRef=%08X sourceType=%u targetExtra=0x%08X extraTarget=0x%08X target=0x%08X targetRef=%08X baseRef=%08X baseType=%u kind=%u model=\"%s\" arrow=0x%08X shooter=0x%08X shooterRef=%08X staticDepth=%u",
			label,
			eventMask,
			PtrValue(source),
			RefId(source),
			source ? source->GetFormType() : 0,
			PtrValue(targetExtra),
			PtrValue(extraTarget),
			PtrValue(target),
			RefId(target),
			RefId(target ? target->baseForm : 0),
			target && target->baseForm ? target->baseForm->GetFormType() : 0,
			GetTrainingTargetKind(target),
			ModelPathForRef(target) ? ModelPathForRef(target) : "",
			PtrValue(arrow),
			PtrValue(shooter),
			RefId(shooter),
			RealArenaTraining_StaticAttackProbeDepth);
#else
		(void)label;
		(void)source;
		(void)targetExtra;
		(void)eventMask;
		(void)arrow;
		(void)registerTarget;
#endif
	}

	void AddPlayerSkillUse(TESObjectREFR* target, UInt32 actorValue, float progress, const char* reason)
	{
		if (!IsSkill(actorValue))
		{
			LogRejectedTraining(target, actorValue, "TrainingRejectedNoSkill");
			return;
		}

		PlayerCharacter* player = (g_thePlayer && *g_thePlayer) ? *g_thePlayer : 0;
		if (!player)
		{
			LogRejectedTraining(target, actorValue, "TrainingRejectedNoPlayer");
			return;
		}

		const float beforeProgress = SkillProgressForPlayer(player, actorValue);
		const float requiredProgress = RequiredSkillProgressForPlayer(player, actorValue);
		const UInt32 beforeLevel = player->GetBaseActorValue(actorValue);
		g_playerIncreaseSkillExperience(player, actorValue, progress, 0, 0);
		const float afterProgress = SkillProgressForPlayer(player, actorValue);
		const UInt32 afterLevel = player->GetBaseActorValue(actorValue);
		const float baseProgress = REALARENATRAINING_SKILL_PROGRESS_PER_INTERACTION;
		const float multiplier = (baseProgress != 0.0f) ? progress / baseProgress : 0.0f;
		LogTraining(target, actorValue, reason, baseProgress, multiplier, progress, beforeProgress, afterProgress, requiredProgress, beforeLevel, afterLevel);
	}

	void AddMeleeSkillUse(TESObjectREFR* target, UInt32 actorValue, const char* reason)
	{
		if (!IsSkill(actorValue))
		{
			AddPlayerSkillUse(target, actorValue, 0.0f, reason);
			return;
		}

		const TargetKind kind = GetTrainingTargetKind(target);
		if (!CanTrainMeleeSkill(kind, actorValue))
		{
			LogRejectedTraining(target, actorValue, "TrainingRejectedTargetSkill");
			return;
		}

		AddPlayerSkillUse(target, actorValue, TrainingProgressForTargetSkill(target, actorValue), reason);
	}

	void HandleMeleeScriptEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask)
	{
		if (eventMask != kEvent_OnHit || !IsPlayer(source))
			return;

		LogHookEntry("MeleeHookEntry", source, targetExtra, eventMask, 0, 0);

		TESObjectREFR* target = RefFromExtraList(targetExtra);
		if (!IsMeleeTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, eventMask, "MeleeCandidateIgnored");
			return;
		}

		const UInt32 skill = SkillForEquippedPlayerMeleeWeapon();
		AddMeleeSkillUse(target, skill, "MeleeHit");
	}

	void HandleStaticMeleeObjectEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask)
	{
		if (eventMask != kEvent_StaticReachProbe || !IsStaticAttackProbeActive())
			return;

		LogHookEntry("StaticMeleeHookEntry", source, targetExtra, eventMask, 0, 0);

		TESObjectREFR* target = RefFromExtraList(targetExtra);
		if (!IsMeleeTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, eventMask, "StaticMeleeCandidateIgnored");
			return;
		}

		const UInt32 skill = SkillForEquippedPlayerMeleeWeapon();
		if (IsSkill(skill) && CanTrainMeleeSkill(GetTrainingTargetKind(target), skill))
			NoteStaticMeleeTrainingAward(target, skill);
		AddMeleeSkillUse(target, skill, "StaticMeleeHit");
	}

	void HandleStaticMeleeAttackProbeResult(Actor* attacker, UInt32 result)
	{
		if (!IsPlayer(attacker))
			return;

		TESObjectREFR* target = CurrentInterfaceCrosshairTarget();
		LogHookEntry(
			"StaticMeleeProbeResult",
			reinterpret_cast<TESForm*>(attacker),
			target ? &target->baseExtraList : 0,
			result,
			0,
			target);

		if (!result)
			return;

		if (!IsMeleeTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, result, "StaticMeleeProbeCandidateIgnored");
			return;
		}

		const UInt32 skill = SkillForEquippedPlayerMeleeWeapon();
		if (!IsSkill(skill))
		{
			AddMeleeSkillUse(target, skill, "StaticMeleeProbeHit");
			return;
		}

		if (!CanTrainMeleeSkill(GetTrainingTargetKind(target), skill))
		{
			LogRejectedTraining(target, skill, "TrainingRejectedTargetSkill");
			return;
		}

		if (WasStaticMeleeTrainingAwardedThisProbe(target, skill))
		{
			LogRejectedTraining(target, skill, "TrainingRejectedDuplicateStaticProbe");
			return;
		}

		AddMeleeSkillUse(target, skill, "StaticMeleeProbeHit");
	}

	void HandleStaticMeleeImpactTarget(Actor* attacker, TESObjectREFR* target)
	{
		if (!IsStaticAttackProbeActive() || !IsPlayer(attacker))
			return;

		LogHookEntry(
			"StaticMeleeImpactEntry",
			reinterpret_cast<TESForm*>(attacker),
			target ? &target->baseExtraList : 0,
			0,
			0,
			target);

		if (!IsMeleeTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, 0, "StaticMeleeImpactCandidateIgnored");
			return;
		}

		const UInt32 skill = SkillForEquippedPlayerMeleeWeapon();
		if (!IsSkill(skill))
		{
			AddMeleeSkillUse(target, skill, "StaticMeleeImpactHit");
			return;
		}

		if (!CanTrainMeleeSkill(GetTrainingTargetKind(target), skill))
		{
			LogRejectedTraining(target, skill, "TrainingRejectedTargetSkill");
			return;
		}

		if (WasStaticMeleeTrainingAwardedThisProbe(target, skill))
		{
			LogRejectedTraining(target, skill, "TrainingRejectedDuplicateStaticProbe");
			return;
		}

		if (IsSkill(skill))
			NoteStaticMeleeTrainingAward(target, skill);
		AddMeleeSkillUse(target, skill, "StaticMeleeImpactHit");
	}

	void HandleArrowScriptEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask, ArrowProjectile* arrow, TESObjectREFR* registerTarget)
	{
		if (eventMask != kEvent_OnHitWith || !arrow || !IsPlayer(arrow->shooter))
			return;

		LogHookEntry("ArrowHookEntry", source, targetExtra, eventMask, arrow, registerTarget);

		TESObjectREFR* target = registerTarget ? registerTarget : RefFromExtraList(targetExtra);
		if (!IsArcheryTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, eventMask, "ArrowCandidateIgnored");
			return;
		}

		if (ShouldRejectDuplicateArrowTraining(arrow, target, kActorVal_Marksman))
		{
			LogRejectedTraining(target, kActorVal_Marksman, "TrainingRejectedDuplicateArrow");
			return;
		}

		AddPlayerSkillUse(target, kActorVal_Marksman, TrainingProgressForTargetSkill(target, kActorVal_Marksman), "ArrowHit");
	}

	void HandleArrowStaticObjectEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask, ArrowProjectile* arrow, TESObjectREFR* registerTarget)
	{
		if (eventMask != kEvent_StaticReachProbe || !arrow || !IsPlayer(arrow->shooter))
			return;

		LogHookEntry("ArrowStaticHookEntry", source, targetExtra, eventMask, arrow, registerTarget);

		TESObjectREFR* target = registerTarget ? registerTarget : RefFromExtraList(targetExtra);
		if (!IsArcheryTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, eventMask, "ArrowStaticCandidateIgnored");
			return;
		}

		if (ShouldRejectDuplicateArrowTraining(arrow, target, kActorVal_Marksman))
		{
			LogRejectedTraining(target, kActorVal_Marksman, "TrainingRejectedDuplicateArrow");
			return;
		}

		AddPlayerSkillUse(target, kActorVal_Marksman, TrainingProgressForTargetSkill(target, kActorVal_Marksman), "ArrowStaticHit");
	}

	void HandleArrowCollisionTarget(ArrowProjectile* arrow, TESObjectREFR* target)
	{
		if (!arrow || !IsPlayer(arrow->shooter))
			return;

		LogHookEntry(
			"ArrowCollisionEntry",
			0,
			target ? &target->baseExtraList : 0,
			0,
			arrow,
			target);

		if (!IsArcheryTrainingTarget(GetTrainingTargetKind(target)))
		{
			LogCandidate(target, 0, "ArrowCollisionCandidateIgnored");
			return;
		}

		if (ShouldRejectDuplicateArrowTraining(arrow, target, kActorVal_Marksman))
		{
			LogRejectedTraining(target, kActorVal_Marksman, "TrainingRejectedDuplicateArrow");
			return;
		}

		AddPlayerSkillUse(target, kActorVal_Marksman, TrainingProgressForTargetSkill(target, kActorVal_Marksman), "ArrowCollisionHit");
	}

	UInt32 RelativeCallTarget(UInt32 callSite)
	{
		const SInt32 rel = *reinterpret_cast<const SInt32*>(callSite + 1);
		return callSite + 5 + rel;
	}

	bool PatchRelativeCall(CallPatch& patch, void* hook, UInt32 expectedTarget, const char* label)
	{
		if (*reinterpret_cast<const UInt8*>(patch.callSite) != 0xE8)
		{
			_ERROR("ERROR::RealArenaTraining: %s site 0x%08X is not a near call", label, patch.callSite);
			return false;
		}

		const UInt32 target = RelativeCallTarget(patch.callSite);
		if (target != expectedTarget)
		{
			_ERROR(
				"ERROR::RealArenaTraining: %s call target mismatch at 0x%08X expected=0x%08X got=0x%08X",
				label,
				patch.callSite,
				expectedTarget,
				target);
			return false;
		}

		patch.originalRel = *reinterpret_cast<const UInt32*>(patch.callSite + 1);
		const UInt32 hookValue = reinterpret_cast<UInt32>(hook);
		const SInt32 newRel = static_cast<SInt32>(hookValue - (patch.callSite + 5));
		SafeWrite32(patch.callSite + 1, static_cast<UInt32>(newRel));
		FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(patch.callSite), 5);
		patch.installed = true;

		_MESSAGE(
			"RealArenaTraining: installed %s call hook site=0x%08X original=0x%08X hook=0x%08X",
			label,
			patch.callSite,
			target,
			hookValue);
		return true;
	}

	bool HasExactRuntime()
	{
		return g_runtimeVersion == OBLIVION_VERSION_1_2_416;
	}

	bool ValidateRuntime()
	{
		if (!HasExactRuntime())
		{
			_ERROR("ERROR::RealArenaTraining: exact Oblivion 1.2.0416 runtime required; got 0x%08X", g_runtimeVersion);
			return false;
		}

		if (reinterpret_cast<UInt32>(g_scriptAddEvent) != kAddr_ScriptAddEventToExtraScript ||
			reinterpret_cast<UInt32>(g_actorValueGetGroupOffsetFromAV) != kAddr_ActorValueGetGroupOffsetFromAV ||
			reinterpret_cast<UInt32>(g_playerModExperience) != kAddr_PlayerModExperience ||
			reinterpret_cast<UInt32>(g_playerIncreaseSkillExperience) != kAddr_PlayerIncreaseSkillExperience)
		{
			_ERROR("ERROR::RealArenaTraining: internal function address setup failed");
			return false;
		}

		if (RealArenaTraining_ScriptAddEventAddress != kAddr_ScriptAddEventToExtraScript ||
			RealArenaTraining_StaticMeleeProbeAddress != kAddr_StaticMeleeProbeFunction ||
			RealArenaTraining_StaticMeleeImpactAddress != kAddr_StaticMeleeImpactFunction ||
			RealArenaTraining_ArrowCollisionFunctionAddress != kAddr_ArrowCollisionFunction)
		{
			_ERROR("ERROR::RealArenaTraining: exported trampoline address setup failed");
			return false;
		}

		return true;
	}

	bool InstallRuntimeHooks();

	bool IsCompatible(const OBSEInterface* obse)
	{
		if (!obse)
			return false;

		if (obse->isEditor)
		{
			_ERROR("ERROR::RealArenaTraining: editor load is not supported");
			return false;
		}

		if (!IVersionCheck::IsCompatibleVersion(
				obse->oblivionVersion,
				MINIMUM_RUNTIME_VERSION,
				SUPPORTED_RUNTIME_VERSION,
				SUPPORTED_RUNTIME_VERSION_STRICT))
		{
			_ERROR("ERROR::RealArenaTraining: unsupported runtime version 0x%08X", obse->oblivionVersion);
			return false;
		}

		return true;
	}
}

extern "C" void __cdecl RealArenaTraining_HandleMeleeScriptEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask)
{
	HandleMeleeScriptEvent(source, targetExtra, eventMask);
}

extern "C" void __cdecl RealArenaTraining_HandleStaticMeleeObjectEvent(TESForm* source, ExtraDataList* targetExtra, UInt32 eventMask)
{
	HandleStaticMeleeObjectEvent(source, targetExtra, eventMask);
}

extern "C" void __cdecl RealArenaTraining_HandleStaticMeleeAttackProbeResult(Actor* attacker, UInt32 result)
{
	HandleStaticMeleeAttackProbeResult(attacker, result);
}

extern "C" void __cdecl RealArenaTraining_HandleStaticMeleeImpactTarget(Actor* attacker, TESObjectREFR* target)
{
	HandleStaticMeleeImpactTarget(attacker, target);
}

extern "C" void __cdecl RealArenaTraining_HandleArrowScriptEvent(
	TESForm* source,
	ExtraDataList* targetExtra,
	UInt32 eventMask,
	ArrowProjectile* arrow,
	TESObjectREFR* registerTarget)
{
	HandleArrowScriptEvent(source, targetExtra, eventMask, arrow, registerTarget);
}

extern "C" void __cdecl RealArenaTraining_HandleArrowStaticObjectEvent(
	TESForm* source,
	ExtraDataList* targetExtra,
	UInt32 eventMask,
	ArrowProjectile* arrow,
	TESObjectREFR* registerTarget)
{
	HandleArrowStaticObjectEvent(source, targetExtra, eventMask, arrow, registerTarget);
}

extern "C" void __cdecl RealArenaTraining_HandleArrowCollisionTarget(ArrowProjectile* arrow, TESObjectREFR* target)
{
	HandleArrowCollisionTarget(arrow, target);
}

extern "C" __declspec(naked) void RealArenaTraining_MeleeOnHitScriptEventHook()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		push [ebp + 10h]
		push [ebp + 0Ch]
		push [ebp + 08h]
		call RealArenaTraining_HandleMeleeScriptEvent
		add esp, 0Ch
		popad
		pop ebp
		jmp dword ptr [RealArenaTraining_ScriptAddEventAddress]
	}
}

extern "C" __declspec(naked) void RealArenaTraining_StaticMeleeAttackProbeHook()
{
	__asm
	{
		inc dword ptr [RealArenaTraining_StaticAttackProbeDepth]
		inc dword ptr [RealArenaTraining_StaticAttackProbeSequence]
		call dword ptr [RealArenaTraining_StaticMeleeProbeAddress]
		push eax
		pushad
		movzx eax, byte ptr [esp + 20h]
		push eax
		push edi
		call RealArenaTraining_HandleStaticMeleeAttackProbeResult
		add esp, 08h
		popad
		dec dword ptr [RealArenaTraining_StaticAttackProbeDepth]
		pop eax
		ret
	}
}

extern "C" __declspec(naked) void RealArenaTraining_StaticMeleeObjectEventHook()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		push [ebp + 10h]
		push [ebp + 0Ch]
		push [ebp + 08h]
		call RealArenaTraining_HandleStaticMeleeObjectEvent
		add esp, 0Ch
		popad
		pop ebp
		jmp dword ptr [RealArenaTraining_ScriptAddEventAddress]
	}
}

extern "C" __declspec(naked) void RealArenaTraining_StaticMeleeImpactHook()
{
	__asm
	{
		pushad
		mov eax, [esp + 18h]
		mov edx, [esp + 30h]
		sub esp, 6Ch
		fnsave [esp]
		wait
		push edx
		push eax
		call RealArenaTraining_HandleStaticMeleeImpactTarget
		add esp, 08h
		frstor [esp]
		add esp, 6Ch
		popad
		jmp dword ptr [RealArenaTraining_StaticMeleeImpactAddress]
	}
}

extern "C" __declspec(naked) void RealArenaTraining_ArrowOnHitWithScriptEventHook()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		push esi
		push ebx
		push [ebp + 10h]
		push [ebp + 0Ch]
		push [ebp + 08h]
		call RealArenaTraining_HandleArrowScriptEvent
		add esp, 14h
		popad
		pop ebp
		jmp dword ptr [RealArenaTraining_ScriptAddEventAddress]
	}
}

extern "C" __declspec(naked) void RealArenaTraining_ArrowStaticObjectEventHook()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		push edi
		push ebx
		push [ebp + 10h]
		push [ebp + 0Ch]
		push [ebp + 08h]
		call RealArenaTraining_HandleArrowStaticObjectEvent
		add esp, 14h
		popad
		pop ebp
		jmp dword ptr [RealArenaTraining_ScriptAddEventAddress]
	}
}

extern "C" __declspec(naked) void RealArenaTraining_ArrowCollisionCallHook()
{
	__asm
	{
		push ebp
		mov ebp, esp
		pushad
		push edi
		push ecx
		call RealArenaTraining_HandleArrowCollisionTarget
		add esp, 08h
		popad
		pop ebp
		jmp dword ptr [RealArenaTraining_ArrowCollisionFunctionAddress]
	}
}

namespace
{
	bool InstallRuntimeHooks()
	{
		if (!ValidateRuntime())
			return false;

		bool ok = true;
		ok = PatchRelativeCall(
			g_staticMeleeAttackProbePatch,
			reinterpret_cast<void*>(&RealArenaTraining_StaticMeleeAttackProbeHook),
			kAddr_StaticMeleeProbeFunction,
			"static melee attack probe") && ok;
		ok = PatchRelativeCall(
			g_staticMeleePatch,
			reinterpret_cast<void*>(&RealArenaTraining_StaticMeleeObjectEventHook),
			kAddr_ScriptAddEventToExtraScript,
			"static melee object") && ok;
		ok = PatchRelativeCall(
			g_staticMeleeImpactPatch,
			reinterpret_cast<void*>(&RealArenaTraining_StaticMeleeImpactHook),
			kAddr_StaticMeleeImpactFunction,
			"static melee impact") && ok;
		ok = PatchRelativeCall(
			g_meleeOnHitPatch,
			reinterpret_cast<void*>(&RealArenaTraining_MeleeOnHitScriptEventHook),
			kAddr_ScriptAddEventToExtraScript,
			"melee OnHit") && ok;
		ok = PatchRelativeCall(
			g_arrowOnHitWithPatch,
			reinterpret_cast<void*>(&RealArenaTraining_ArrowOnHitWithScriptEventHook),
			kAddr_ScriptAddEventToExtraScript,
			"arrow OnHitWith") && ok;
		ok = PatchRelativeCall(
			g_arrowStaticPatch,
			reinterpret_cast<void*>(&RealArenaTraining_ArrowStaticObjectEventHook),
			kAddr_ScriptAddEventToExtraScript,
			"arrow static object") && ok;
		ok = PatchRelativeCall(
			g_arrowCollisionPatch,
			reinterpret_cast<void*>(&RealArenaTraining_ArrowCollisionCallHook),
			kAddr_ArrowCollisionFunction,
			"arrow collision target") && ok;
		return ok;
	}
}

extern "C"
{
	bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
	{
		OpenPluginLogOnce();
		_MESSAGE(PLUGIN_VERSION_INFO);
		_MESSAGE("Plugin_Query: Querying");

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PLUGIN_NAME_LONG;
		info->version = PLUGIN_VERSION_DLL;

		if (!IsCompatible(obse))
			return false;

		_MESSAGE("Plugin_Query: Queried Successfully");
		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface* obse)
	{
		OpenPluginLogOnce();
		_MESSAGE(PLUGIN_VERSION_INFO);
		_MESSAGE("Plugin_Load: Loading");

		if (!IsCompatible(obse))
			return false;

		g_pluginHandle = obse->GetPluginHandle();
		g_runtimeVersion = obse->oblivionVersion;

		if (!InstallRuntimeHooks())
		{
			_ERROR("ERROR::RealArenaTraining: runtime hook installation failed");
			return false;
		}

		_MESSAGE("Plugin_Load: Loaded Successfully");
		return true;
	}
}
