# Real Arena Training

An engine plugin that makes arena practice dummies grant real player skill progress with multipliers and penalties.

The plugin uses Oblivion's decoded player skill-progress path instead of fake scripting, spell effects, or quest-side workarounds. When the player hits supported Arena training statics, the appropriate combat skill receives controlled progress based on the object and interaction type. It does not replace normal combat training, NPC combat, or interrupt actor hits. Progress is granted only when the player interacts with the supported practice statics listed below, matched by base form and model path.

## Features

- `ArenaHeavyBag01`: melee hits grant the currently used melee skill.
- `ChainDollArena01` and the live Arena Bloodworks `ChainDoll01`: melee hits grant the currently used melee skill.
- `ArenaDummy1`: Blade or Blunt weapon hits grant that weapon skill.
- `TargetHay01`: player-fired arrows grant Marksman; Blade or Blunt weapon hits grant that melee skill.
- Works for any placement of those decoded static base forms.
- Uses Oblivion 1.2.0416's decoded player skill-progress routine at `0x00668B30`.
- Logs accepted training with `base`, `multiplier`, and `applied` progress values for fidelity checks.

## Supported Training Objects

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

Unsupported weapons, unrelated statics, actors, and NPC combat are ignored by Real Arena Training's custom training path.

## Progress Multipliers

Each accepted interaction starts from a raw `1.0` skill-progress delta before the object multiplier is applied. A half-percent bonus is treated as an exact half percent, so `0.5%` becomes a `1.005x` multiplier.

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

1. Install xOBSE.
2. Download `RealArenaTraining-1.0.zip`.
3. Extract the archive into the Oblivion install folder.
4. Confirm the DLL is installed here:

```text
Oblivion\Data\OBSE\Plugins\RealArenaTraining.dll
```

5. Launch Oblivion through xOBSE.

## Credits

Daggers

## Source

https://github.com/KinzhaldeDagas/RealArenaTraining
