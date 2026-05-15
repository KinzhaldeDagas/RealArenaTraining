# Changelog

All notable RealArenaTraining changes are tracked here.

## 1.0.0 - 2026-05-15

Initial public release.

### Added

- Added xOBSE plugin support for Oblivion 1.2.0416.
- Added custom skill progress for decoded arena practice statics only:
  - `ArenaHeavyBag01`
  - `ChainDollArena01`
  - live Arena Bloodworks `ChainDoll01`
  - `ArenaDummy1`
  - `TargetHay01`
- Added base-form and NIF/model-path gates so any placement of the supported statics can train, while unrelated statics do not.
- Added melee skill selection from the player's equipped weapon:
  - no weapon grants Hand to Hand where supported
  - one-handed or two-handed blade weapons grant Blade
  - one-handed or two-handed blunt weapons grant Blunt
- Added player-fired arrow support for `TargetHay01` Marksman training.
- Added per-target progress multipliers:
  - `ArenaDummy1`: Blade `1.5x`; Blunt `0.5x`, or `1.0x` when player base Luck is exactly `10`
  - `ChainDollArena01` / `ChainDoll01`: Hand to Hand `1.0x`; Blade `0.5x`; Blunt `1.005x`
  - `ArenaHeavyBag01`: Hand to Hand `1.005x`; Blade `0.96x`; Blunt `0.96x`
  - `TargetHay01`: Marksman `1.005x`; Blade `0.98x`; Blunt `0.98x`
- Added capped diagnostic logging for accepted training, rejected target/skill combinations, ignored candidates, and hook installation.
- Added accepted-training log fields for base progress, multiplier, applied raw progress, before/after skill progress, required progress, and skill level before/after.
