# Arena Dummy Inventory

Generated from `C:\src\OblivionPlugin\output\groups\STAT.ndjson.gz` and `CELL.ndjson.gz` parsed from `Oblivion.esm`.

## Scope

This inventory covers the vanilla training-dummy family relevant to RealArenaTraining investigation: heavy bags, chain dolls, heavy targets, hay targets, and the Arena-specific dummy base. RealArenaTraining now trains every static whose base model path matches a row marked `targeted=yes`; the listed base forms are the decoded vanilla bases that use those NIF paths.

| Base form | EDID | Model | Bounds radius | Targeted by model path | Placement count |
| --- | --- | --- | ---: | --- | ---: |
| `0x00159450` | `ArenaHeavyBag01` | `Architecture\Arena\ArenaHeavyBag01.NIF` | 121.018 | yes | 5 |
| `0x00027FD8` | `ChainDollArena01` | `Architecture\Arena\ChainDollArena01.NIF` | 165.825 | yes | 4 |
| `0x00000E06` | `ChainDoll01` | `Clutter\FightersGuild\ChainDoll01.NIF` | 168.263 | yes | 14 |
| `0x000521F3` | `ChainDoll02` | `Clutter\FightersGuild\ChainDoll02.NIF` | 121.954 | yes | 5 |
| `0x000693D7` | `ArenaDummy1` | `Clutter\FightersGuild\TargetHeavy01.NIF` | 212.570 | yes | 4 |
| `0x00000CED` | `TargetHeavy01` | `Clutter\FightersGuild\TargetHeavy01.NIF` | 212.570 | yes | 24 |
| `0x00000D45` | `TargetHay01` | `Clutter\FightersGuild\TargetHay01.NIF` | 95.747 | yes | 34 |

## Model Fingerprints

Runtime matching uses exact normalized model-path fingerprints for static base forms. `ArenaDummy1` and generic `TargetHeavy01` resolve to the same model path and identical NIF bytes, so they intentionally share the same `TargetHeavy01.NIF` Blade/Blunt behavior. The SHA-256 values below are useful as offline evidence for the vanilla BSA payloads.

| EDID | Normalized model path | FNV-1a32 path hash | Path length | BSA | BSA offset | Stored bytes | Unpacked bytes | NIF SHA-256 |
| --- | --- | ---: | ---: | --- | ---: | ---: | ---: | --- |
| `ArenaHeavyBag01` | `Architecture\Arena\ArenaHeavyBag01.NIF` | `0x90A29F11` | 38 | `Oblivion - Meshes.bsa` | 560116569 | 36090 | 105914 | `5c1b5343de36a1961620677421005cc42eaf716036a4ae7af71fc940d48b0d0b` |
| `ChainDollArena01` | `Architecture\Arena\ChainDollArena01.NIF` | `0xD6B42764` | 39 | `Oblivion - Meshes.bsa` | 537479930 | 192099 | 853326 | `ff22643bd372cf0a8a2d47aed524839c76e1b7b2605d648c3d71f907f618a900` |
| `ChainDoll01` | `Clutter\FightersGuild\ChainDoll01.NIF` | `0x4970EBAF` | 37 | `Oblivion - Meshes.bsa` | 535605385 | 238606 | 731806 | `083e59b3ddc9fd409bda9e1e497bd4aeda1a5ab2588a3bc879d0acfa77dacfa6` |
| `ChainDoll02` | `Clutter\FightersGuild\ChainDoll02.NIF` | `0x8432F02E` | 37 | `Oblivion - Meshes.bsa` | 559157020 | 214237 | 626017 | `60965ec63381b3d3e2bda118080bcc1cba475ac7ca5810dfc4c8f00363ab3e30` |
| `ArenaDummy1` | `Clutter\FightersGuild\TargetHeavy01.NIF` | `0x5D13CC8B` | 39 | `Oblivion - Meshes.bsa` | 526592912 | 110884 | 420986 | `09b2fad132c18a43cd23bc68163085adcac8ef5d5503585fd03ea20f8d16617b` |
| `TargetHeavy01` | `Clutter\FightersGuild\TargetHeavy01.NIF` | `0x5D13CC8B` | 39 | `Oblivion - Meshes.bsa` | 526592912 | 110884 | 420986 | `09b2fad132c18a43cd23bc68163085adcac8ef5d5503585fd03ea20f8d16617b` |
| `TargetHay01` | `Clutter\FightersGuild\TargetHay01.NIF` | `0xD1FB87BE` | 37 | `Oblivion - Meshes.bsa` | 577596818 | 143469 | 340145 | `c9304150560f78f1d249bbda2bcb575df40eee1f956c23be67e6b043a3576461` |

## Placement Counts

- `ArenaHeavyBag01` (`0x00159450`): 5
- `ChainDollArena01` (`0x00027FD8`): 4
- `ChainDoll01` (`0x00000E06`): 14
- `ChainDoll02` (`0x000521F3`): 5
- `ArenaDummy1` (`0x000693D7`): 4
- `TargetHeavy01` (`0x00000CED`): 24
- `TargetHay01` (`0x00000D45`): 34

## Placed References

| Base EDID | Base form | Ref form | Ref EDID | Cell form | Cell EDID | Cell name | Kind | Scale | Position XYZ | Rotation XYZ | Flags |
| --- | --- | --- | --- | --- | --- | --- | --- | ---: | --- | --- | --- |
| `ArenaHeavyBag01` | `0x00159450` | `0x0005263A` | `` | `0x00010332` | `XPCann05` | Cann, Arena Substratum | Interior | 1.000 | 1302.797, -1511.197, 4795.376 | 0.000, -0.000, -1.571 |  |
| `ArenaHeavyBag01` | `0x00159450` | `0x0005266C` | `` | `0x00010332` | `XPCann05` | Cann, Arena Substratum | Interior | 1.000 | 3393.464, 2981.697, 4795.381 | 0.000, -0.000, -1.571 |  |
| `ArenaHeavyBag01` | `0x00159450` | `0x00159451` | `HeavyBagRef` | `0x00037868` | `ICArenaBloodworks` | Arena Bloodworks | Interior | 1.000 | 1850.008, 1606.998, 6296.463 | 0.000, 0.000, 0.000 | persistent |
| `ArenaHeavyBag01` | `0x00159450` | `0x000661E5` | `JGhastaBag2` | `0x000661C9` | `BrumaJGhastasHouseTrainingRoom` | Secret Training Room | Interior | 1.000 | 2275.134, 1985.225, 7061.045 | 0.000, 0.000, 0.000 | persistent |
| `ArenaHeavyBag01` | `0x00159450` | `0x000661E7` | `JGhastaBag1` | `0x000661C9` | `BrumaJGhastasHouseTrainingRoom` | Secret Training Room | Interior | 1.000 | 2280.668, 2146.208, 7059.398 | 0.000, 0.000, 0.000 | persistent |
| `ChainDollArena01` | `0x00027FD8` | `0x0006F45A` | `` | `0x00015E38` | `FortWariel` | Fort Wariel | Interior | 1.040 | -1433.486, -236.456, 317.804 | 0.000, -0.000, -3.142 |  |
| `ChainDollArena01` | `0x00027FD8` | `0x0006562C` | `` | `0x0003379A` | `FortBlueBlood02` | Fort Blueblood Halls | Interior | 1.050 | -1663.424, 2479.049, -560.806 | 0.000, -0.000, -3.142 |  |
| `ChainDollArena01` | `0x00027FD8` | `0x0006560C` | `` | `0x0003C2CD` | `FortBlueBlood` | Fort Blueblood | Interior | 1.020 | 2035.945, 3917.196, 208.777 | 0.000, -0.000, -1.571 |  |
| `ChainDollArena01` | `0x00027FD8` | `0x000C7C2C` | `` | `0x000C7B37` | `FortBlueBlood02Backup` | Fort Blueblood Halls | Interior | 1.050 | -1663.424, 2479.049, -560.806 | 0.000, -0.000, -3.142 |  |
| `ChainDoll01` | `0x00000E06` | `0x00023DA3` | `` | `0x000008C1` | `WarehouseMisc` | Misc Clutter Warehouse | Interior | 1.000 | 2682.166, 2435.062, 34.526 | 0.000, -0.000, 0.000 |  |
| `ChainDoll01` | `0x00000E06` | `0x000A316A` | `BladesDummy1` | `0x00000ADC` | `CloudRulerTempleArmory` | Temple Armory | Interior | 1.000 | -317.955, 41.556, 38.382 | 0.000, -0.000, -0.890 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x000A3B4D` | `` | `0x00006917` | `CheydinhalMagesGuildBasement` | Mages Guild Basement | Interior | 1.000 | -19.878, -771.872, 40.238 | 0.000, -0.000, -3.083 |  |
| `ChainDoll01` | `0x00000E06` | `0x000084FC` | `` | `0x0000691B` | `CheydinhalFightersGuildBasement` | Cheydinhal Fighters Guild Basement | Interior | 1.000 | 25.245, -468.287, -416.227 | 0.000, -0.000, -3.142 |  |
| `ChainDoll01` | `0x00000E06` | `0x0005264A` | `` | `0x00010332` | `XPCann05` | Cann, Arena Substratum | Interior | 1.000 | 612.509, 1610.003, 4709.410 | 0.000, -0.000, -0.671 |  |
| `ChainDoll01` | `0x00000E06` | `0x0001DB3F` | `ChorrolFGDummyREF` | `0x0001C93F` | `ChorrolFightersGuildBasement` | Chorrol Fighters Guild Basement | Interior | 1.000 | 25.245, -468.287, -416.227 | 0.000, -0.000, -3.142 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x0002F70A` | `SkingradPracticeDummyREF` | `0x0002D155` | `SkingradFightersGuildBasement` | Fighters Guild Basement | Interior | 1.000 | 362.016, 27.778, -92.035 | 0.000, -0.000, 1.571 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x000A4B3C` | `` | `0x0002DB9A` | `BrumaCastleBarracks` | Castle Bruma Barracks | Interior | 1.000 | 382.984, 1537.014, -410.590 | 0.000, -0.000, 1.550 |  |
| `ChainDoll01` | `0x00000E06` | `0x00037F72` | `ICArenaDummy` | `0x00037868` | `ICArenaBloodworks` | Arena Bloodworks | Interior | 1.000 | 1492.586, 1998.990, 6220.541 | 0.000, -0.000, -1.790 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x0006EF35` | `` | `0x0003F153` | `TestTraps` |  | Interior | 1.000 | 1554.199, 2423.589, 6334.106 | 0.000, 0.000, 0.000 |  |
| `ChainDoll01` | `0x00000E06` | `0x0004CF73` | `DarkSancDummy2` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 1.000 | 3146.311, 1474.124, 6549.112 | 0.000, -0.000, 1.600 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x0004CF95` | `DarkSancDummy1` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 1.000 | 2954.032, 1024.778, 6549.410 | 0.000, -0.000, -0.033 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x000661E6` | `JGhastaDummy1` | `0x000661C9` | `BrumaJGhastasHouseTrainingRoom` | Secret Training Room | Interior | 1.000 | 2540.818, 2092.198, 6997.410 | 0.000, -0.000, 1.460 | persistent |
| `ChainDoll01` | `0x00000E06` | `0x00094A9F` | `` | `0x00092E8F` | `WhiteStallionLodgeInterior` | White Stallion Lodge | Interior | 1.000 | -0.800, 258.402, 47.458 | 0.000, -0.000, -1.542 |  |
| `ChainDoll02` | `0x000521F3` | `0x00067403` | `` | `0x0000A2B9` | `BravilFightersGuildBasement` | Bravil Fighters Guild Basement | Interior | 1.000 | 112.132, -1007.232, 57.456 | 0.000, -0.000, -3.142 |  |
| `ChainDoll02` | `0x000521F3` | `0x000CBA05` | `` | `0x00015C3A` | `SquanderedMine` | Squandered Mine | Interior | 1.130 | 4211.309, -351.906, -168.602 | 0.000, 0.000, 0.000 |  |
| `ChainDoll02` | `0x000521F3` | `0x000521F0` | `` | `0x000302ED` | `BrumaFightersGuild` | Bruma Fighters Guild | Interior | 1.000 | -1191.063, 1632.870, 33.239 | 0.000, -0.000, -1.571 |  |
| `ChainDoll02` | `0x000521F3` | `0x000496D3` | `` | `0x000308CB` | `AnvilFightersGuild` | Fighters Guild | Interior | 1.000 | 157.671, 8.854, -96.382 | 0.000, -0.000, -1.571 |  |
| `ChainDoll02` | `0x000521F3` | `0x0006EF36` | `` | `0x0003F153` | `TestTraps` |  | Interior | 1.000 | 1689.444, 2423.435, 6329.277 | 0.000, 0.000, 0.000 |  |
| `ArenaDummy1` | `0x000693D7` | `0x0005263B` | `` | `0x00010332` | `XPCann05` | Cann, Arena Substratum | Interior | 1.000 | 1608.252, -1718.978, 4672.000 | 0.000, -0.000, -2.327 |  |
| `ArenaDummy1` | `0x000693D7` | `0x0005266D` | `` | `0x00010332` | `XPCann05` | Cann, Arena Substratum | Interior | 1.000 | 3026.847, 3185.896, 4672.000 | 0.000, -0.000, 1.573 |  |
| `ArenaDummy1` | `0x000693D7` | `0x0000CF08` | `` | `0x000302E4` | `BrumaArnorasHouse` | Arnora's House | Interior | 0.750 | 17.062, 131.574, -359.231 | 0.000, -0.000, 1.571 | persistent |
| `ArenaDummy1` | `0x000693D7` | `0x00132A90` | `AgronakDummyRef` | `0x00037868` | `ICArenaBloodworks` | Arena Bloodworks | Interior | 1.000 | 2270.917, 2389.613, 6153.998 | 0.003, -0.000, -3.023 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0002ABF9` | `` | `0x00000820` | `SkingradTownGuardHouse` | Skingrad Town Guard House | Interior | 1.000 | -6.227, -981.758, 90.418 | 0.000, -0.000, -3.125 |  |
| `TargetHeavy01` | `0x00000CED` | `0x00023D78` | `` | `0x000008C1` | `WarehouseMisc` | Misc Clutter Warehouse | Interior | 1.000 | 2520.371, 2446.431, -35.213 | 0.000, -0.000, 1.571 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0003CC3A` | `` | `0x00007946` | `AnvilCastleBarracks` | Castle Barracks | Interior | 0.900 | 368.605, -194.258, 221.826 | 0.000, -0.000, -2.700 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0009C796` | `` | `0x0000BFB6` | `AnvilTownGuardBarracks` | City Watch Barracks | Interior | 1.000 | 2144.063, 2087.682, 7137.395 | 0.000, -0.000, -3.133 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0008E21A` | `SEJayredShootArrowsTargetRef` | `0x00011F94` | `SEPasswallJayredsTent` | Jayred's House | Interior | 0.700 | -878.401, -23.344, -265.559 | 0.000, 0.000, 0.000 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x00057F92` | `` | `0x000158B1` | `SEDeepwallowBeeleisHouse` | Beelei's House | Interior | 1.000 | 391.164, 437.688, -137.444 | 0.000, -0.000, -1.600 |  |
| `TargetHeavy01` | `0x00000CED` | `0x000573F8` | `` | `0x000173AD` | `ICImperialLegionWatchTowerNE` | Northeast Watch Tower | Interior | 0.650 | -476.139, 612.026, -200.142 | 0.000, -0.000, 0.700 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0002DFE1` | `` | `0x0002C162` | `ICMarketDistrictSlashNSmash` | Slash 'N Smash | Interior | 0.780 | -257.269, 186.070, -2.055 | 0.000, -0.000, 1.600 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0005C604` | `AUPracticeTarget1` | `0x0002C174` | `ICArcaneUniversityPracticeRooms` | Practice Rooms | Interior | 0.720 | -226.944, 121.891, -124.638 | 0.000, -0.000, 0.000 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0005C605` | `AUPracticeRoomTarget2` | `0x0002C174` | `ICArcaneUniversityPracticeRooms` | Practice Rooms | Interior | 0.720 | 491.098, 113.476, -124.638 | 0.000, -0.000, -3.083 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0006BC3F` | `` | `0x0002C174` | `ICArcaneUniversityPracticeRooms` | Practice Rooms | Interior | 0.720 | -129.872, -548.246, -124.638 | 0.000, -0.000, -1.571 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0009CB03` | `` | `0x0002C1C1` | `ICElvenGardensAdrianDecaniusHouse` | Adrian Decanius' House | Interior | 0.680 | 183.115, 20.172, -142.490 | 0.000, -0.000, -1.571 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0000528A` | `SkingradArcheryREF` | `0x0002D155` | `SkingradFightersGuildBasement` | Fighters Guild Basement | Interior | 1.000 | 564.001, 476.212, -114.102 | 0.000, -0.000, 1.571 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x000521EF` | `` | `0x000302ED` | `BrumaFightersGuild` | Bruma Fighters Guild | Interior | 1.000 | -901.079, 1982.375, -20.485 | 0.000, -0.000, 1.571 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0003CA9F` | `FGLeyawiinTarget` | `0x0003E0E5` | `LeyawiinFightersGuildTraining` | Fighter's Guild 2nd Floor East | Interior | 1.000 | 3231.611, -472.531, 149.328 | 0.000, 0.000, 0.000 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0009CDF6` | `` | `0x00047C9A` | `ICElvenGardensGuardHouseBasement` | Guard House Basement | Interior | 0.910 | 1744.788, 2402.514, 6961.671 | 0.000, 0.000, 0.000 |  |
| `TargetHeavy01` | `0x00000CED` | `0x000A28C1` | `` | `0x00049AF0` | `ICMarketDistrictAFightingChanceBasement` | A Fighting Chance Basement | Interior | 0.630 | 1554.676, 2493.819, 7527.772 | 0.000, -0.000, 1.571 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0004CF96` | `MraajDarSpellTarget` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 1.000 | 2060.005, 969.317, 6488.596 | 0.000, -0.000, -1.583 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x00057950` | `` | `0x000512F6` | `ICImperialLegionWatchTowerSWTopGuardQuarters` | Southwest Watch Barracks | Interior | 0.700 | 2.556, 843.984, 1582.097 | 0.000, -0.000, 1.400 |  |
| `TargetHeavy01` | `0x00000CED` | `0x000577CE` | `` | `0x0005397E` | `ICImperialLegionWatchTowerSCaptainsQuarters` | South Watch Captain's Quarters | Interior | 0.570 | -43.921, -768.671, 297.481 | 0.000, -0.000, 1.383 |  |
| `TargetHeavy01` | `0x00000CED` | `0x0005C61C` | `AUPracticeRoomsUpstairsTarget02` | `0x000855A9` | `ICArcaneUniversityPracticeRoomsUpstairs` | Practice Rooms Upper Level  | Interior | 1.000 | 1944.735, 2458.083, 7188.549 | 0.000, -0.000, 1.600 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0005C61E` | `AUPracticeRoomsUpstairsTarget03` | `0x000855A9` | `ICArcaneUniversityPracticeRoomsUpstairs` | Practice Rooms Upper Level  | Interior | 1.000 | 2212.849, 2458.843, 7188.549 | 0.000, -0.000, 1.600 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0005C620` | `AUPracticeRoomsUpstairsTarget01` | `0x000855A9` | `ICArcaneUniversityPracticeRoomsUpstairs` | Practice Rooms Upper Level  | Interior | 1.000 | 1704.746, 2456.827, 7188.549 | 0.000, -0.000, 1.600 | persistent |
| `TargetHeavy01` | `0x00000CED` | `0x0005C621` | `AUPracticeRoomsUpstairsTarget04` | `0x000855A9` | `ICArcaneUniversityPracticeRoomsUpstairs` | Practice Rooms Upper Level  | Interior | 1.000 | 2217.134, 1700.044, 7188.549 | 0.000, -0.000, -1.583 | persistent |
| `TargetHay01` | `0x00000D45` | `0x00023D77` | `` | `0x000008C1` | `WarehouseMisc` | Misc Clutter Warehouse | Interior | 1.000 | 2404.563, 2383.552, -34.392 | 0.000, 0.000, 0.000 |  |
| `TargetHay01` | `0x00000D45` | `0x000954CC` | `` | `0x0000691B` | `CheydinhalFightersGuildBasement` | Cheydinhal Fighters Guild Basement | Interior | 1.000 | 335.770, -464.567, -472.811 | 0.000, -0.000, 1.771 |  |
| `TargetHay01` | `0x00000D45` | `0x000954CD` | `` | `0x0000691B` | `CheydinhalFightersGuildBasement` | Cheydinhal Fighters Guild Basement | Interior | 1.000 | 339.780, -344.430, -472.811 | 0.000, -0.000, 1.371 |  |
| `TargetHay01` | `0x00000D45` | `0x0003CC37` | `` | `0x00007946` | `AnvilCastleBarracks` | Castle Barracks | Interior | 1.000 | 391.642, 105.953, 180.779 | 0.000, -0.000, 1.500 |  |
| `TargetHay01` | `0x00000D45` | `0x0003CC3B` | `` | `0x00007946` | `AnvilCastleBarracks` | Castle Barracks | Interior | 1.000 | 387.908, -33.966, 180.779 | 0.000, -0.000, 1.600 |  |
| `TargetHay01` | `0x00000D45` | `0x0009C795` | `` | `0x0000BFB6` | `AnvilTownGuardBarracks` | City Watch Barracks | Interior | 1.000 | 1956.121, 2243.352, 7112.654 | 0.000, -0.000, -0.550 |  |
| `TargetHay01` | `0x00000D45` | `0x00057778` | `` | `0x000173AB` | `ICImperialLegionWatchTowerS` | South Watch Tower | Interior | 1.000 | 217.352, -339.840, -79.161 | 0.000, -0.000, -2.600 |  |
| `TargetHay01` | `0x00000D45` | `0x00057779` | `` | `0x000173AB` | `ICImperialLegionWatchTowerS` | South Watch Tower | Interior | 1.000 | 105.601, -147.951, -79.161 | 0.000, -0.000, -1.700 |  |
| `TargetHay01` | `0x00000D45` | `0x000577FF` | `` | `0x000173AC` | `ICImperialLegionWatchTowerSE` | Southeast Watch Tower | Interior | 1.000 | -549.515, 351.508, 166.373 | 0.000, -0.000, 1.400 |  |
| `TargetHay01` | `0x00000D45` | `0x00057800` | `` | `0x000173AC` | `ICImperialLegionWatchTowerSE` | Southeast Watch Tower | Interior | 1.000 | -665.986, 542.642, 166.373 | 0.000, -0.000, 0.700 |  |
| `TargetHay01` | `0x00000D45` | `0x0007C24F` | `` | `0x0002C170` | `ICMarketDistrictAFightingChance` | A Fighting Chance | Interior | 1.000 | 216.975, 13.431, -15.486 | 0.000, -0.000, -0.400 |  |
| `TargetHay01` | `0x00000D45` | `0x000A2876` | `` | `0x0002C170` | `ICMarketDistrictAFightingChance` | A Fighting Chance | Interior | 1.000 | 456.742, 14.387, -15.486 | 0.000, -0.000, -0.300 |  |
| `TargetHay01` | `0x00000D45` | `0x0002897A` | `` | `0x0002C52F` | `KvatchCastleRooms` | Great Hall | Interior | 0.870 | 2111.196, 1485.968, -30.604 | 0.000, -0.000, -2.600 |  |
| `TargetHay01` | `0x00000D45` | `0x000A2000` | `` | `0x000302ED` | `BrumaFightersGuild` | Bruma Fighters Guild | Interior | 1.000 | -1302.354, 1949.252, -35.970 | 0.000, -0.000, -1.125 |  |
| `TargetHay01` | `0x00000D45` | `0x0009182A` | `` | `0x00030420` | `CheydinhalOrumHouse` | Magrum gra-Orum's House | Interior | 1.000 | 540.823, -142.350, -210.134 | 0.000, -0.000, 2.700 |  |
| `TargetHay01` | `0x00000D45` | `0x00051D8F` | `` | `0x000308CB` | `AnvilFightersGuild` | Fighters Guild | Interior | 1.000 | -108.624, -82.393, -155.562 | 0.000, -0.000, -1.600 |  |
| `TargetHay01` | `0x00000D45` | `0x00051D90` | `` | `0x000308CB` | `AnvilFightersGuild` | Fighters Guild | Interior | 1.000 | -110.181, 78.799, -155.562 | 0.000, -0.000, -1.600 |  |
| `TargetHay01` | `0x00000D45` | `0x000378EC` | `ArenaICTarget` | `0x00037868` | `ICArenaBloodworks` | Arena Bloodworks | Interior | 1.000 | 2484.647, 2078.089, 6149.886 | 0.000, -0.000, 1.150 | persistent |
| `TargetHay01` | `0x00000D45` | `0x0003CAA0` | `` | `0x0003E0E5` | `LeyawiinFightersGuildTraining` | Fighter's Guild 2nd Floor East | Interior | 1.000 | 3201.662, -680.630, 95.428 | 0.000, -0.000, -1.571 |  |
| `TargetHay01` | `0x00000D45` | `0x0009CDF2` | `` | `0x00047C9A` | `ICElvenGardensGuardHouseBasement` | Guard House Basement | Interior | 1.000 | 1842.541, 1828.105, 6960.874 | 0.000, -0.000, 3.100 |  |
| `TargetHay01` | `0x00000D45` | `0x0009CDF3` | `` | `0x00047C9A` | `ICElvenGardensGuardHouseBasement` | Guard House Basement | Interior | 1.000 | 2154.699, 1902.059, 6960.874 | 0.000, -0.000, 1.700 |  |
| `TargetHay01` | `0x00000D45` | `0x0002E00B` | `` | `0x00049E28` | `ICMarketDistrictSlashNSmashBasement` | Slash 'N Smash Basement | Interior | 1.000 | -236.087, 119.922, 733.348 | 0.000, -0.000, -0.500 |  |
| `TargetHay01` | `0x00000D45` | `0x0007C1DD` | `` | `0x00049E28` | `ICMarketDistrictSlashNSmashBasement` | Slash 'N Smash Basement | Interior | 1.000 | -65.122, -197.233, 733.348 | 0.000, -0.000, -1.900 |  |
| `TargetHay01` | `0x00000D45` | `0x0007C201` | `` | `0x00049E2F` | `ICMarketDistrictTheCopiousCoinpurseBasement` | The Copious Coinpurse Basement | Interior | 1.000 | 277.690, -131.770, 2030.501 | 0.000, -0.000, 1.800 |  |
| `TargetHay01` | `0x00000D45` | `0x0018BCE5` | `` | `0x0004BF32` | `ICWaterfrontDarelothsHouseBasement` | Dareloth's Basement | Interior | 1.000 | 2112.432, 58.385, 9184.000 | 0.000, -0.000, -3.142 |  |
| `TargetHay01` | `0x00000D45` | `0x0004CF71` | `DarkSancTarget1` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 1.040 | 2530.460, 950.309, 6481.353 | 0.000, -0.000, -2.983 | persistent |
| `TargetHay01` | `0x00000D45` | `0x0004CF8F` | `` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 0.800 | 2630.749, 979.226, 6461.177 | 0.000, -0.000, 2.800 |  |
| `TargetHay01` | `0x00000D45` | `0x0004CF90` | `` | `0x0004CF47` | `CheydinhalSanctuaryTrainingRoom` | Dark Brotherhood Training Room | Interior | 0.800 | 2430.388, 984.958, 6461.177 | 0.000, -0.000, -2.983 |  |
| `TargetHay01` | `0x00000D45` | `0x0005747E` | `` | `0x00051055` | `ICImperialLegionWatchTowerNGuardQuarters` | North Watch Barracks | Interior | 1.000 | -228.027, 671.198, 462.222 | 0.000, -0.000, -1.200 |  |
| `TargetHay01` | `0x00000D45` | `0x00051C63` | `` | `0x00051B90` | `BravilFightersGuild2ndFloor` | Bravil Fighter's Guild Second Floor | Interior | 1.000 | 669.266, 1595.460, 6246.282 | 0.000, -0.000, -0.700 |  |
| `TargetHay01` | `0x00000D45` | `0x00094AA2` | `` | `0x00092E8F` | `WhiteStallionLodgeInterior` | White Stallion Lodge | Interior | 1.000 | -265.827, 304.347, -24.201 | 0.000, -0.000, -1.071 |  |
| `TargetHay01` | `0x00000D45` | `0x00094AA3` | `` | `0x00092E8F` | `WhiteStallionLodgeInterior` | White Stallion Lodge | Interior | 1.000 | 334.019, -19.020, -22.855 | 0.000, -0.000, 1.300 |  |
| `TargetHay01` | `0x00000D45` | `0x00150F37` | `` | `0x000B985E` | `ICPalaceCrypt` | Palace Basement | Interior | 1.000 | -187.397, -286.359, -178.392 | 0.000, -0.000, 0.898 |  |
| `TargetHay01` | `0x00000D45` | `0x00150F38` | `` | `0x000B985E` | `ICPalaceCrypt` | Palace Basement | Interior | 1.000 | -61.316, -277.471, -178.392 | 0.000, -0.000, 2.418 |  |

## Runtime Identification Recommendation

- Use exact normalized model-path fingerprints for static forms: lowercase ASCII, `/` normalized to `\`, then FNV-1a32 plus path length.
- Treat every static using the same decoded training NIF as the same training target kind. `ArenaDummy1` and generic `TargetHeavy01` therefore both use `TargetHeavy01.NIF` Blade/Blunt behavior.
- Treat NIF SHA-256 as an offline audit value, not the runtime key, unless a future pass decodes and uses Oblivion archive I/O for BSA-backed resources. The vanilla NIFs above are packed in `Oblivion - Meshes.bsa`, and no loose files exist under `Data\Meshes` in the checked install.
