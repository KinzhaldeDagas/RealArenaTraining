# RealArenaTraining IDA Notes

Authoritative database: `Oblivion.exe` IDA instance `bys7`.

Fallout material was not used to choose behavior or addresses.

## Skill Experience

- `0x00668C30` is `Player_ModExperience`.
- IDA shows it resolving the `TESSkill` from the actor value, applying the selected skill use value, then forwarding positive progress into `Player_IncreaseSkillExperience?` at `0x00668B30`.
- `0x00565C50` is `ActorValue_GetGroupOffsetFromAV(group, actorValue)`. The player skill-progress paths call it with group `2` before indexing skill arrays.
- `0x00668C56` loads `TESSkill + 0x38 + (useIndex * 4)`, proving that `Player_ModExperience` depends on the selected skill-use trigger index before it reaches the actual progress routine.
- `0x00668B30` is the lower-level player skill-progress routine used by `Player_ModExperience`.
  - actor value is the first stack argument.
  - raw progress delta is the second stack argument.
  - optional `TESSkill*` is the third stack argument; when null, the function resolves it from the actor value.
  - suppress flag is the fourth stack argument.
  - `0x00668B90` / `0x00668B97` read current player skill progress from the skill-progress array using `ActorValue_GetGroupOffsetFromAV(2, actorValue)`.
  - `0x00668BA0` / `0x00668BA7` add the raw progress delta.
  - `0x00668BB3` writes the result through `Player_SetSkillProgress`.
  - `0x00668BC3` fetches required progress for the skill.
  - `0x00668BE2` calls `Player_SkillLevelIncrease` when the threshold is exceeded.
  - `0x00668C16` updates actor value UI.
- The vanilla caller sequence at `0x00668C9D` through `0x00668CA7` confirms the `0x00668B30` stack layout: suppress flag, `TESSkill*`, reserved raw-progress float slot, then actor value, with `ECX == PlayerCharacter*`.
- RealArenaTraining now calls `Player_IncreaseSkillExperience?(player, skillActorValue, 1.0f, nullptr, 0)` directly. This still uses Oblivion's normal skill-progress and level-up path, but avoids inventing a combat-specific `useIndex` for custom arena interactions.
- Build `0.1.7` changed plugin progress logging to call `ActorValue_GetGroupOffsetFromAV(2, actorValue)` for skill array reads, matching the decoded Oblivion path instead of relying on local subtraction.

## Melee Hits

- `0x005FF630` is the `Actor_AttackHandling:::RunOnHitByScript` call to `Script_AddEventToExtraScript`.
- The call arguments at this site are:
  - source: `edi`, the attacker reference.
  - target extra list: `esi + 0x44`, the hit reference's `TESObjectREFR::baseExtraList`.
  - event mask: `0x80`, `OnHit`.
- RealArenaTraining retargets only this callsite, preserves the original `Script_AddEventToExtraScript` call, and grants training only when the source is `*g_thePlayer` and the target passes the decoded static-target gate described below.
- The `v0.1.9` in-game log from the reported chain-doll failure showed the player `OnHit` hook firing:
  - event mask: `0x80`.
  - source ref: `00000014`, the player.
  - target ref: `00146843`.
  - target base: `0014683C`.
  - target base type: `35`, `kFormType_NPC`.
  - model path logged as empty because the plugin only read static/object model components in that build.
- `C:\src\OblivionPlugin` parser pass against `Oblivion.esm` resolved those IDs:
  - `00146843` is `ACHR` editor ID `ArenaICBlueTeamGladiatorRef`.
  - `0014683C` is `NPC_` editor ID `ArenaICBlueTeamGladiator`, full name `Blue Team Gladiator`, model `Characters\_Male\skeleton.NIF`.
  - This was not `ChainDollArena01`; it was a nearby/actual actor hit.
- The same parser pass resolved the target training bases:
  - `ArenaHeavyBag01`: `STAT 00159450`, model `Architecture\Arena\ArenaHeavyBag01.NIF`.
  - `TargetHay01`: `STAT 00000D45`, model `Clutter\FightersGuild\TargetHay01.NIF`.
  - `ChainDollArena01`: `STAT 00027FD8`, model `Architecture\Arena\ChainDollArena01.NIF`.
  - `ArenaDummy1`: `STAT 000693D7`, model `Clutter\FightersGuild\TargetHeavy01.NIF`.
- The `v0.1.12` live log for the reported chain-doll failure showed the player static probe and impact hooks firing, but the resolved target was:
  - `REFR 00037F72`, editor ID `ICArenaDummy`.
  - base `STAT 00000E06`, editor ID `ChainDoll01`.
  - model `Clutter\FightersGuild\ChainDoll01.NIF`.
  - This is the actual Arena Bloodworks chain dummy; it is not base `00027FD8 ChainDollArena01`.
- Build `0.1.16` targets any placement of the decoded static training target kinds, not only one placed ref:
  - `STAT 00159450 ArenaHeavyBag01`, model `Architecture\Arena\ArenaHeavyBag01.NIF`.
  - `STAT 00027FD8 ChainDollArena01`, model `Architecture\Arena\ChainDollArena01.NIF`.
  - `STAT 00000E06 ChainDoll01`, model `Clutter\FightersGuild\ChainDoll01.NIF`; this is the base used by the live Arena Bloodworks chain dummy hit in the log.
  - `STAT 000693D7 ArenaDummy1`, model `Clutter\FightersGuild\TargetHeavy01.NIF`.
  - `STAT 00000D45 TargetHay01`, model `Clutter\FightersGuild\TargetHay01.NIF`.
- The gameplay scope remains only the requested practice target kinds: heavy bag, chain doll, arena dummy, and hay target. The chain-doll kind includes both decoded vanilla chain-doll static bases because the live Arena Bloodworks dummy is `ChainDoll01`, while the requested `ChainDollArena01` is a separate vanilla static.
- Target matching still requires `baseForm->GetFormType() == kFormType_Stat`, the expected base form ID, and the expected NIF/model name. NPCs and unrelated statics remain outside RealArenaTraining's custom skill path.
- Build `1.0.0` keeps the requested per-target progress multipliers on the raw progress delta before calling `0x00668B30`; it does not change Oblivion's decoded skill-progress or level-up routine. Requested `0.5%` adjustments are treated as exact half-percent multipliers (`1.005x`), not rounded to `1%`. This pass names the multiplier constants in source and logs `base`, `multiplier`, and `applied` for each accepted training hit.
  - `ArenaDummy1`: Blade `1.5x`; Blunt `0.5x`, or `1.0x` when `PlayerCharacter::GetBaseActorValue(kActorVal_Luck) == 10`.
  - chain-doll targets: Blade `0.5x`; Blunt `1.005x`; Hand to Hand remains `1.0x`.
  - `ArenaHeavyBag01`: Blade and Blunt `0.96x`; Hand to Hand `1.005x`.
  - `TargetHay01`: Blade and Blunt `0.98x`; Marksman `1.005x`.

### Excluded Actor Weapon Script Calls

The callsite sweep of all `Script_AddEventToExtraScript` xrefs found two nearby weapon-script calls in the actor melee path:

- `0x005FF608` pushes event mask `0x100` and target extra `esi + 0x44`.
- `0x005FF61A` also pushes event mask `0x100` and target extra `esi + 0x44`.
- Both are in `Actor_AttackHandling_::RunWeaponOnHitScript_` and flow immediately into `Actor_AttackHandling_::RunOnHitByScript` at `0x005FF630`.

These are actor-target weapon `OnHitWith` script events, not an observed static prop impact path. RealArenaTraining does not hook them for training because `0x005FF630` already covers the actor-target hit path and these calls do not prove bag/doll static interaction semantics.

## Static / Non-Actor Melee Props

The second decoding pass found the player-only static reach path that covers non-actor arena practice objects.

- A later caller pass checked every xref to `sub_5F9620`:
  - `0x005FCD83` in `Actor_ProcessAction` excludes the player before calling `sub_5F9620`.
  - `0x005FCE5E` in `Actor_ProcessAction` also excludes the player before calling `sub_5F9620`.
  - `0x005FF05D` in `Actor_AttackHandling:::DetermineTarget` is the only observed player caller.
- `sub_5F9620` is called from `Actor_AttackHandling:::DetermineTarget` at `0x005FF05D` when the attacking ref is `*g_thePlayer`.
- Near the end of `sub_5F9620`, IDA shows a guarded call to `Script_AddEventToExtraScript` at `0x005F9E62`.
- The local state and call arguments at that site are:
  - `esi == *g_thePlayer`.
  - `edi`: reached object reference.
  - `edi` must match `InterfaceManager + 0xC8`, the current crosshair reference.
  - `sub_536AE0` must accept the object from the parent-cell/world context.
  - source: `edi`.
  - target extra list: `edi + 0x44`.
  - event mask: `0x10000000`.
- RealArenaTraining now wraps the confirmed attack caller at `0x005FF05D` and only accepts the inner `0x005F9E62` static event while that wrapper is active. It still preserves both original calls. This avoids assigning invented semantics to the generic inner event and ties training to the attack target resolver path that IDA proves.
- After the in-game `v0.1.8` log showed all hooks installed but no hook-entry lines, a fidelity pass targeted the level before the optional script-event branch:
  - `0x005FF05D` is still the confirmed player static attack probe call.
  - the caller immediately tests `AL` after `sub_5F9620` returns.
  - inside `sub_5F9620`, the script-event branch compares the candidate target against `InterfaceManager + 0xC8`, the crosshair reference.
- Build `0.1.9` kept the inner `0x005F9E62` event hook, but also checked the post-call `AL` result at the outer `0x005FF05D` wrapper. Build `0.1.14` keeps that decoded hook path and accepts any placement of the decoded static bases listed above.
- The decoded path proves a player attack-target static reach/crosshair event. It does not prove Havok damage or mesh impact semantics for static props.
- The next fidelity pass decoded the actual collision-reference path inside `sub_5F9620`:
  - `0x005F9768` calls `sub_4DC270`, resolving a NiAVObject collision owner back to a `TESObjectREFR`.
  - `0x005F9772` stores that resolved hit reference in the local target slot.
  - `0x005F9AC3` calls `sub_5F05F0` with `ECX == attacker` and the resolved target reference as the stack argument at call-entry offset `+0x10`.
  - `sub_5F05F0` returns with `retn 18h`; the hook therefore preserves the original call-entry stack and tail-jumps to the original function after diagnostics/training.
  - Build `0.1.12` wraps the diagnostic/training call with `fnsave` / `frstor` so Oblivion's original impact function sees the same x87 state it would have seen at `0x005F9AC3`.
  - The hook preserves the original `sub_5F05F0` call and awards melee training only when the wrapper depth proves this is the player attack probe and the resolved target is one of the decoded static bases with the expected NIF/model name.
  - This targets the largest remaining ChainDoll gap without inventing a form-id shortcut or relying on the UI crosshair ref.

## Arrow Hits

- `0x0060B387` is the arrow projectile hit path's call to `Script_AddEventToExtraScript`.
- The surrounding IDA state identifies:
  - `ebx`: `ArrowProjectile*`.
  - `esi`: hit `TESObjectREFR*`.
  - `0x0060B313` loads `esi` from the hit-ref local/argument.
  - `ArrowProjectile::shooter` at `this + 0x78`, also present in the xOBSE headers.
  - event mask: `0x100`, `OnHitWith`.
- RealArenaTraining retargets this callsite, preserves the original event call, and grants Marksman only when `arrow->shooter == *g_thePlayer` and the hit reference is `STAT 00000D45 TargetHay01` with the expected NIF token.

## Arrow Static / Non-Actor Targets

The skills-not-leveling interaction pass found that `sub_60B120`, the arrow projectile hit routine, has a second static-object event path.

- `0x0060B8D1` is another `Script_AddEventToExtraScript` call inside `sub_60B120`.
- The local state and call arguments at that site are:
  - `ebx`: `ArrowProjectile*`.
  - `edi`: hit object reference. `0x0060B894` reloads it from the same hit-ref local used by the earlier `0x0060B387` arrow event.
  - source: `edi`.
  - target extra list: `edi + 0x44`.
  - event mask: `0x10000000`.
  - before the call, Oblivion resolves the hit ref's parent-cell/world context and requires `sub_536AE0` to accept the ref.
- RealArenaTraining now retargets this callsite separately, preserves the original event call, and grants Marksman only when `arrow->shooter == *g_thePlayer` and the hit reference is `STAT 00000D45 TargetHay01` with the expected NIF token.
- This fixes a decoded gap in the previous build: actor/object `OnHitWith` at `0x0060B387` was not the only arrow interaction path, so arrows into non-actor targets could miss the training hook.
- Because both arrow event callsites are in the same `sub_60B120` collision routine and IDA shows the same `ebx` arrow and same hit ref flowing through them, build `0.1.6` suppresses duplicate Marksman progress only by same `ArrowProjectile* + target + skill` identity. The earlier broad timer throttle was removed because no Oblivion skill-progress or hit path decoded so far proves a time-based training gate.

## Arrow Collision Caller

The `v0.1.8` in-game log showed no arrow hook-entry lines, so the next pass decoded the sole caller of `sub_60B120`.

- `0x0060D950` resolves a Havok collidable to a NiAVObject and then to a `TESObjectREFR` with `sub_4DC270`.
- `0x0060D983` through `0x0060D989` dynamic-cast that resolved ref to `Actor`; the non-actor branch reaches `0x0060DB45`.
- At `0x0060DB59`, `esi` is loaded from the arrow argument and `edi` is still the resolved non-actor hit reference.
- `0x0060DB6B` calls `sub_60B120` with `ecx == ArrowProjectile*` and `edi == hit TESObjectREFR*`.
- Build `0.1.9` hooks this specific callsite. It grants Marksman only when `arrow->shooter == *g_thePlayer` and `edi` is `STAT 00000D45 TargetHay01` with the expected NIF token. The existing same-arrow duplicate guard prevents a second award if the later `sub_60B120` script-event callsites also fire.

## Runtime Guard

These callsite hooks are accepted only for Oblivion `1.2.0416` (`0x010201A0`). During load the plugin verifies each patched call still targets `Script_AddEventToExtraScript` at `0x004FBF90`.

## Skills-Not-Leveling Diagnostics

The latest plugin build logs both accepted training interactions and ignored candidate interactions without expanding the decoded behavior.

- Last observed logs from the in-game failure report:
  - `RealArenaTraining.log` contained only `OBSEPlugin_Query` for `v0.1.7`.
  - `RealArenaTraining.log0` contained `OBSEPlugin_Load` and all five successful hook installs for `v0.1.7`.
  - No accepted-training, rejected-training, or ignored-candidate interaction lines were present.
- A later `v0.1.8` log showed Query and Load in one file and all hooks installed, but still no hook-entry lines. That proved the tested interactions were not reaching the previously hooked script-event callsites.
- The split between `.log` and `.log0` was caused by calling `gLog.OpenRelative` in both `OBSEPlugin_Query` and `OBSEPlugin_Load`; xOBSE's `IDebugLog::Open` opens the current log with write-deny and then falls back to suffix files when called again. Build `0.1.8` opens the plugin log only once per process so Query, Load, hook install, and interaction diagnostics remain in the same file.
- Accepted interactions log the target pointer, ref id, base form id, model path, selected skill, base progress, multiplier, applied raw progress, progress before/after, required progress, skill level before/after, and raw progress delta.
- Ignored candidates are capped and log the event mask, target pointer, ref id, base form id, form type, and model path.
- Target-matched interactions that still do not apply training are also capped and logged. These include unsupported equipped weapon type, missing player pointer, and duplicate arrow-event rejection. The rejection log records the selected skill, equipped weapon form, and weapon type.
- Build `0.1.8` adds capped pre-filter hook-entry logs for the existing decoded hook sites. These entries record event mask, source form, target extra pointer, reconstructed target, target model path, decoded target kind, arrow pointer, shooter, and static-probe depth before any player/source/model filter can return.
- The live `v0.1.14` log after the ChainDoll fix confirms `StaticMeleeImpactHit` on `REFR 00037F72` / `STAT 00000E06 ChainDoll01` for both Hand to Hand and Blade progress. It also confirms unrelated NPC/static impacts enter the decoded impact callsite with `staticDepth=0` and do not award training.
- Build `0.1.15` keeps the same training gates but moves capped hook-entry diagnostics behind the decoded player/source filters. This prevents NPC arena combat at the shared static impact callsite from exhausting the diagnostic budget before an untested `TargetHay01` arrow interaction can log `ArrowCollisionEntry`, `ArrowStaticHookEntry`, or `ArrowHookEntry`.
- This is meant to distinguish the remaining live-game failure modes:
  - no relevant hook fires,
  - the hook fires but the source/player or arrow/shooter filter rejects before candidate logging,
  - a hook fires but the actual arena prop is not one of the decoded static bases,
  - the target matches but the equipped weapon does not decode to Blade, Blunt, or unarmed Hand to Hand,
  - the arrow hit was already awarded through the other decoded arrow event callsite for the same arrow and target,
  - the target and skill match but progress changes without immediately reaching the level-up threshold.

## Feature Completion Estimate

After the `v0.1.14` live log, the melee feature set is validated against the actual Arena Bloodworks statics. The remaining untested gameplay path is `TargetHay01` arrow-to-Marksman.

- Skill progress path: effectively complete. `Player_ModExperience` at `0x00668C30`, `ActorValue_GetGroupOffsetFromAV` at `0x00565C50`, and the lower-level progress routine at `0x00668B30` are decoded down to progress-array read, raw delta add, threshold check, level-up call, and UI refresh. The plugin now uses `0x00668B30` directly so custom arena interactions always add raw skill progress instead of depending on an unproven `TESSkill` use-index.
- Arrow-to-`TargetHay01` Marksman: high confidence, about `97%`. Both decoded arrow event callsites are covered: actor/object `OnHitWith` at `0x0060B387` and static/non-actor object event at `0x0060B8D1`. Build `0.1.9` also covers the earlier decoded non-actor collision caller at `0x0060DB6B`, so Marksman no longer depends on the optional script-event branches firing.
- Actor melee `OnHit` bag/doll handling: high confidence, about `95%`. The normal `OnHit` callsite is decoded and guarded to the player; current observed bag/doll leveling is coming through the resolved static impact path rather than needing the actor OnHit path.
- Static/non-actor bag/doll melee handling: high confidence, about `98%` for the observed Arena Bloodworks bag and chain dummy. The implementation uses the only decoded player caller, `Actor_AttackHandling:::DetermineTarget` at `0x005FF05D`, the inner `0x005F9E62` event when it fires, the post-call `AL` result, and the resolved impact target at `0x005F9AC3`.
- Resolved-target static melee handling for the chain dummy: high confidence, about `99%` for the live Arena Bloodworks chain dummy. The live `v0.1.14` log proves progress on `REFR 00037F72 ICArenaDummy`, whose base is `STAT 00000E06 ChainDoll01`, and Build `0.1.14+` accepts any placement of that decoded chain-doll static base while still accepting the separately parsed `STAT 00027FD8 ChainDollArena01` base.
- Weapon skill selection: about `95%` for unarmed and Blade after the `v0.1.14` log. Blunt uses the same decoded `TESObjectWEAP::type` enum family as Blade but still lacks a separate live log. The plugin does not separately decode a "weapon drawn" flag because the hooked paths are attack/hit paths, where use is already implied.

Overall estimate after melee live validation: about `96%` complete. The remaining completion risk is live validation of player arrows into `TargetHay01`; Build `0.1.15` keeps the arrow diagnostics from being starved by unrelated static melee hook entries.
