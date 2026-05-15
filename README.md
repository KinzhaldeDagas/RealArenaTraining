# RealArenaTraining

RealArenaTraining is an xOBSE plugin for Oblivion 1.2.0416 that makes arena practice statics grant real player skill progress through Oblivion's decoded skill-progress path.

The plugin is deliberately narrow. It does not replace normal combat training, does not interrupt NPC hits, and does not award progress for arbitrary props. Custom progress is granted only when the player interacts with the decoded practice statics listed below, matched by base form and NIF model path.

## Features

- `ArenaHeavyBag01`: melee hits grant the currently used melee skill.
- `ChainDollArena01` and the live Arena Bloodworks `ChainDoll01`: melee hits grant the currently used melee skill.
- `ArenaDummy1`: Blade or Blunt weapon hits grant that weapon skill.
- `TargetHay01`: player-fired arrows grant Marksman; Blade or Blunt weapon hits grant that melee skill.
- Works for any placement of those decoded static base forms.
- Uses Oblivion 1.2.0416's decoded player skill-progress routine at `0x00668B30`.
- Logs accepted training with `base`, `multiplier`, and `applied` progress values for fidelity checks.

## Supported Training

| Static | Interaction | Skill |
| --- | --- | --- |
| `ArenaHeavyBag01` | Unarmed melee hit | Hand to Hand |
| `ArenaHeavyBag01` | Blade weapon hit | Blade |
| `ArenaHeavyBag01` | Blunt weapon hit | Blunt |
| `ChainDollArena01` / `ChainDoll01` | Unarmed melee hit | Hand to Hand |
| `ChainDollArena01` / `ChainDoll01` | Blade weapon hit | Blade |
| `ChainDollArena01` / `ChainDoll01` | Blunt weapon hit | Blunt |
| `ArenaDummy1` | Blade weapon hit | Blade |
| `ArenaDummy1` | Blunt weapon hit | Blunt |
| `TargetHay01` | Player arrow hit | Marksman |
| `TargetHay01` | Blade weapon hit | Blade |
| `TargetHay01` | Blunt weapon hit | Blunt |

Unsupported weapons, unrelated statics, actors, and NPC combat are ignored by RealArenaTraining's custom training path.

## Progress Multipliers

Per-interaction progress is based on a raw `1.0` skill-progress delta before the target multiplier is applied. `0.5%` is treated as an exact half percent, so the multiplier is `1.005x`.

| Static | Skill | Multiplier |
| --- | --- | --- |
| `ArenaDummy1` | Blade | `1.5x` |
| `ArenaDummy1` | Blunt | `0.5x` |
| `ArenaDummy1` | Blunt, when player base Luck is exactly `10` | `1.0x` |
| `ChainDollArena01` / `ChainDoll01` | Hand to Hand | `1.0x` |
| `ChainDollArena01` / `ChainDoll01` | Blade | `0.5x` |
| `ChainDollArena01` / `ChainDoll01` | Blunt | `1.005x` |
| `ArenaHeavyBag01` | Hand to Hand | `1.005x` |
| `ArenaHeavyBag01` | Blade | `0.96x` |
| `ArenaHeavyBag01` | Blunt | `0.96x` |
| `TargetHay01` | Marksman | `1.005x` |
| `TargetHay01` | Blade | `0.98x` |
| `TargetHay01` | Blunt | `0.98x` |

## Installation

1. Install xOBSE for Oblivion 1.2.0416.
2. Download `RealArenaTraining-1.0.zip` from the GitHub release.
3. Extract the archive into the Oblivion install folder so the DLL lands at:

```text
Oblivion\Data\OBSE\Plugins\RealArenaTraining.dll
```

4. Launch Oblivion through xOBSE.

## Logging

The plugin writes to:

```text
Documents\My Games\Oblivion\OBSE\RealArenaTraining.log
```

Accepted training lines include:

- target reference and base form
- matched model path
- selected skill
- base progress
- multiplier
- applied raw progress delta
- progress before and after
- required progress
- skill level before and after

These logs are intended to prove that the plugin is adding progress through Oblivion's decoded skill-progress path and only for the supported statics.

## Building

The repository includes a copied xOBSE/ezobse tree under `ezobse`. Build the plugin from:

```text
ezobse\obse_plugins\realarenatraining\RealArenaTraining.sln
```

Use `Release|Win32`. The built DLL is emitted to:

```text
ezobse\obse_plugins\realarenatraining\Builds\Release\RealArenaTraining.dll
```

## Decoding Notes

The Oblivion IDA decoding notes are tracked in:

```text
ida_decode\RealArenaTraining_IDA_notes.md
```

Oblivion's IDA database is treated as authoritative. Fallout material was used only for naming or contrast after Oblivion behavior was observed.

## Changelog

See [CHANGELOG.md](CHANGELOG.md).
