# Real Arena Training

An engine plugin that makes arena practice dummies grant real player skill progress with multipliers and penalties.

The plugin uses Oblivion's decoded player skill-progress path instead of fake scripting, spell effects, or quest-side workarounds. When the player hits supported Arena training statics, the appropriate combat skill receives controlled progress based on the object and interaction type. It does not replace normal combat training, NPC combat, or interrupt actor hits. Progress is granted only when the player interacts with static objects using the supported practice NIF paths listed below.

## Features

- `Architecture\Arena\ArenaHeavyBag01.NIF`: melee hits grant the currently used melee skill.
- `Architecture\Arena\ChainDollArena01.NIF`, `Clutter\FightersGuild\ChainDoll01.NIF`, and `Clutter\FightersGuild\ChainDoll02.NIF`: melee hits grant the currently used melee skill.
- `Clutter\FightersGuild\TargetHeavy01.NIF`: Blade or Blunt weapon hits grant that weapon skill.
- `Clutter\FightersGuild\TargetHay01.NIF`: player-fired arrows grant Marksman; Blade or Blunt weapon hits grant that melee skill.
- Works for any static placement or static base form using one of those exact decoded model paths.
- Uses Oblivion 1.2.0416's decoded player skill-progress routine at `0x00668B30`.
- Logs accepted training with `base`, `multiplier`, and `applied` progress values for fidelity checks.

## Supported Training Objects

| Static model path | Interaction | Skill |
| --- | --- | --- |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Unarmed melee hit | Hand to Hand |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Blade weapon hit | Blade |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Blunt weapon hit | Blunt |
| `Architecture\Arena\ChainDollArena01.NIF` / `Clutter\FightersGuild\ChainDoll01.NIF` / `Clutter\FightersGuild\ChainDoll02.NIF` | Unarmed melee hit | Hand to Hand |
| `Architecture\Arena\ChainDollArena01.NIF` / `Clutter\FightersGuild\ChainDoll01.NIF` / `Clutter\FightersGuild\ChainDoll02.NIF` | Blade weapon hit | Blade |
| `Architecture\Arena\ChainDollArena01.NIF` / `Clutter\FightersGuild\ChainDoll01.NIF` / `Clutter\FightersGuild\ChainDoll02.NIF` | Blunt weapon hit | Blunt |
| `Clutter\FightersGuild\TargetHeavy01.NIF` | Blade weapon hit | Blade |
| `Clutter\FightersGuild\TargetHeavy01.NIF` | Blunt weapon hit | Blunt |
| `Clutter\FightersGuild\TargetHay01.NIF` | Player arrow hit | Marksman |
| `Clutter\FightersGuild\TargetHay01.NIF` | Blade weapon hit | Blade |
| `Clutter\FightersGuild\TargetHay01.NIF` | Blunt weapon hit | Blunt |

Unsupported weapons, unrelated statics, actors, and NPC combat are ignored by Real Arena Training's custom training path.

## Progress Multipliers

Each accepted interaction starts from a raw `1.0` skill-progress delta before the object multiplier is applied. A half-percent bonus is treated as an exact half percent, so `0.5%` becomes a `1.005x` multiplier.

| Static model path | Skill | Multiplier |
| --- | --- | --- |
| `Clutter\FightersGuild\TargetHeavy01.NIF` | Blade | `1.5x` |
| `Clutter\FightersGuild\TargetHeavy01.NIF` | Blunt | `0.5x` |
| `Clutter\FightersGuild\TargetHeavy01.NIF` | Blunt, when player base Luck is exactly `10` | `1.0x` |
| chain doll NIFs | Hand to Hand | `1.0x` |
| chain doll NIFs | Blade | `0.5x` |
| chain doll NIFs | Blunt | `1.005x` |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Hand to Hand | `1.005x` |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Blade | `0.96x` |
| `Architecture\Arena\ArenaHeavyBag01.NIF` | Blunt | `0.96x` |
| `Clutter\FightersGuild\TargetHay01.NIF` | Marksman | `1.005x` |
| `Clutter\FightersGuild\TargetHay01.NIF` | Blade | `0.98x` |
| `Clutter\FightersGuild\TargetHay01.NIF` | Blunt | `0.98x` |

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

## Reference Notes

- Full vanilla dummy/base placement inventory: [docs/ArenaDummyInventory.md](docs/ArenaDummyInventory.md)
- Changelog: [CHANGELOG.md](CHANGELOG.md)
