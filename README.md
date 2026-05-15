# RealArenaTraining

RealArenaTraining 1.0.0 is an xOBSE plugin for Oblivion 1.2.0416.

It grants player skill use only for decoded arena practice statics identified by base form and matching NIF model path:

- `ArenaHeavyBag01` and the decoded chain-doll statics (`ChainDollArena01`, plus the live Arena Bloodworks `ChainDoll01`): melee hits grant the currently used melee skill.
  - No equipped weapon: Hand to Hand.
  - Blade one-handed or two-handed weapon: Blade.
  - Blunt one-handed or two-handed weapon: Blunt.
- `ArenaDummy1`: Blade or Blunt weapon hits grant that weapon skill.
- `TargetHay01`: arrows fired by the player grant Marksman; Blade or Blunt weapon hits grant that melee skill.

Current per-interaction progress multipliers, with `0.5%` treated as an exact half percent:

- `ArenaDummy1`: Blade `1.5x`; Blunt `0.5x`, or `1.0x` when the player's base Luck is exactly `10`.
- Chain-doll targets: Blade `0.5x`; Blunt `1.005x`; Hand to Hand `1.0x`.
- `ArenaHeavyBag01`: Blade and Blunt `0.96x`; Hand to Hand `1.005x`.
- `TargetHay01`: Blade and Blunt `0.98x`; Marksman `1.005x`.

Training log entries include `base`, `multiplier`, and `applied` so in-game traces can confirm the exact raw progress delta passed into Oblivion's decoded skill-progress routine.

The plugin uses the copied xOBSE/ezobse tree in `ezobse` and builds from:

```text
ezobse\obse_plugins\realarenatraining\RealArenaTraining.sln
```

The Release DLL is emitted to:

```text
ezobse\obse_plugins\realarenatraining\Builds\Release\RealArenaTraining.dll
```

A packaged copy is also emitted under:

```text
package\Data\OBSE\Plugins\
```

Install the DLL under:

```text
Oblivion\Data\OBSE\Plugins\
```

The IDA decoding notes and current feature-completion estimate are in:

```text
ida_decode\RealArenaTraining_IDA_notes.md
```
