# AW2E.lua entries that the repo had already named

`src/AW2E.lua` proposes a name for each address it lists. Where the repo had
already given that same address a meaningful (non-`sub_`/`gUnknown_`) name, the
existing name is kept and the Lua name is **not** applied -- renaming would
churn call sites, comments and `.thumb_set` aliases for no gain, and in several
cases the existing name records evidence the Lua name does not.

Everything else in `src/AW2E.lua` is wired in: promoted C functions carry the
Lua name with a `.thumb_set sub_XXXXXXXX, <LuaName>` alias in their own file,
and proc scripts plus still-assembly functions get a `.global`/`.set` debugger
alias in [src/aw2e-names.s](src/aw2e-names.s).

## Open conflicts (Lua name skipped)

| Address | Name in AW2E.lua | Name in the repo | Defined in |
| --- | --- | --- | --- |
| `0x0801153D` | `FadeLoadMap_IDLE_0801153D` | `SomeFade_IDLE_0801153D` | [src/decomp/c_0801153C.c](src/decomp/c_0801153C.c) |
| `0x08034F7D` | `BlockMapStartCoInfo_08034F7D` | `IncrementMapLock` | [src/decomp/c_08034F6C.c](src/decomp/c_08034F6C.c) |
| `0x08034F8D` | `BlockMapStartCoInfo_08034F8D` | `DecrementMapLock` | [src/decomp/c_08034F8C.c](src/decomp/c_08034F8C.c) |
| `0x0803BD6D` | `MainMenu2_GOTO_IF_NO_0803BD6D` | `GetMainMenuLock` | [src/decomp/c_0803BD54.c](src/decomp/c_0803BD54.c) |
| `0x080366A5` | `BlockMapStartCoInfo_080366A5` | `InitMainFrameCallbacks` | [src/decomp/c_080366A4.c](src/decomp/c_080366A4.c) |

### Notes

* **`0x0801153D` -- `FadeLoadMap_IDLE_0801153D` vs `SomeFade_IDLE_0801153D`.**
  These disagree about which proc *owns* the routine, not just what to call it.
  `ProcScr_SomeFade` (`0x0848929C`) and `ProcScr_FadeLoadMap` (`0x084892C4`)
  both list `0x0801153C` as their last `PROC_REPEAT`, so the routine really is
  shared and neither owner-prefixed name is right. Left under `SomeFade_` until
  a name that does not imply a single owner is picked.
* **`0x08034F7D` / `0x08034F8D` / `0x080366A5`.** The repo names describe what
  the functions do (`IncrementMapLock`, `DecrementMapLock`,
  `InitMainFrameCallbacks`) and are shared by more than one caller, so the
  `BlockMapStartCoInfo_`-prefixed Lua names would be narrower than the truth.
  Their sibling `0x08034FD9` had no name yet and *was* wired in as
  `BlockMapStartCoInfo_08034FD9`.
* **`0x0803BD6D` -- `MainMenu2_GOTO_IF_NO_0803BD6D` vs `GetMainMenuLock`.** The
  Lua name is positionally correct: this *is* the `PROC_GOTO_IF_NO` predicate of
  `ProcScr_MainMenu2`. It lost anyway because the function is one third of the
  `gGameLock` accessor set (see [include/lock.h](include/lock.h)), the other two
  thirds are named for the lock, and it has callers outside that script.

## Resolved in favour of the Lua name

These were conflicts; the Lua name won and the repo was renamed to match.

| Address | Was | Now |
| --- | --- | --- |
| `0x084892C4` | `DesignRoomLoad3` / `ProcScr_DesignRoomLoad3` | `FadeLoadMap` / `ProcScr_FadeLoadMap` |
| `0x0801137D` | `DesignRoomLoad3_0801137D` | `FadeLoadMap_0801137D` |
| `0x080114A1` | `DesignRoomLoad3_IDLE_080114A1` | `FadeLoadMap_IDLE_080114A1` |
| `0x08616EFC` | `CoDesignC4` / `ProcScr_CoDesignC4` | `PutFace` / `ProcScr_PutFace` |
| `0x0808A3A1` | `CoDesignC4_IDLE_0808A3A1` | `PutFace_IDLE_0808A3A1` |

`ProcScr_FadeScreenRelated` in [src/decomp/c_080110EC.c](src/decomp/c_080110EC.c)
is a second alias for the same array and was left alone.

## Renamed after reading the code

These names (AW2E.lua labels or earlier house names) did not match what the
matched C does, so the repo was renamed. `src/AW2E.lua` keeps the original
labels. The `sub_XXXXXXXX` alias of each function is unchanged.

| Address | Was | Now |
| --- | --- | --- |
| `0x08071CF4` | `FadeToBlack_OnInit` | `FadeToBlack_OnInitUnused` |
| `0x08071D70` | `FadeToCommon_OnLoop` | `FadeToCommon_OnLoopUnused` |
| `0x08071DB4` | `FadeFromBlack_OnInit` | `FadeFromBlack_OnInitUnused` |
| `0x08071E40` | `FadeFromCommon_OnLoop` | `FadeFromCommon_OnLoopUnused` |
| `0x08011054` | `FadePalBlack_08011055` | `FadeToBlack_OnInit` |
| `0x080110A4` | `FadePalBlack_IDLE_080110A5` | `FadeToCommon_OnLoop` |
| `0x080110EC` | `DesignRoomFadeIn_080110ED` | `FadeFromBlack_OnInit` |
| `0x0801113C` | `DesignRoomFadeIn_IDLE_0801113D` | `FadeFromCommon_OnLoop` |
| `0x08019260` | `WM_ConfirmExit_WHILE_08019261` | `IsAnyEventScriptRunning` |
| `0x0801C754` | `WaitForLaser_IDLE_0801C755` | `APProc_OnUpdate` |
| `0x0801C780` | `WaitForLaser_CB_0801C781` | `APProc_OnEnd` |
| `0x0802481C` | `WarRoomScroll_CB_0802481D` | `FreeMapLoadBuffer` |
| `0x08024ABC` | `CalcDamage` | `SelectBattleWeapon` |
| `0x0802CC90` | `CanShowSubMenuItem` | `UnitMenu_DiveUsability` |
| `0x08036384` | `SelectUnit_IDLE_08036385` | `MoveSlide_Loop` |
| `0x080363D0` | `SelectUnit_CB_080363D1` | `MoveSlide_OnEnd` |
| `0x08039188` | `DrawMarkerSprites` | `DrawMovePathArrow` |
| `0x0803B628` | `CampaignIntro_WHILE_0803B629` | `IsMusicFadeActive` |
| `0x0803E388` | `CountLivingInventionsOfType` | `HasLivingInventionOfType` |
| `0x08042C9C` | `GetCoPriceMultiplier` | `GetUnitCostWithCoBonus` |
| `0x080489CC` | `BattleMaps_080489CD` | `ShopScreen_Init` |
| `0x08049360` | `BattleMaps_IDLE_08049361` | `ShopScreen_Loop` |
| `0x0806A454` | `MainMenu2_0806A455` | `StartIntroSequence` |
| `0x0806F710` | `SoundRoom_0806F711` | `StartSoundRoomBlocking` |
| `0x08072CE4` | `MainMenu_PutSelectModeSprite_IDLE_08072CE5` | `HeaderBanner_PopInLoop` |
| `0x08072F04` | `MainMenu_PutSelectModeSprite_IDLE_08072F05` | `HeaderBanner_HoldLoop` |
| `0x080733B8` | `EndCoSelect_080733B9` | `EndHeaderBanner` |
| `0x080750A4` | `WM_MoveScope_080750A5` | `DifficultyStars_Init` |
| `0x0807519C` | `WM_MoveScope_IDLE_0807519D` | `DifficultyStars_SpawnLoop` |
| `0x08081060` | `MainMenuC1_08081061` | `MainMenuCarousel_Init` |
| `0x0808135C` | `MainMenuC2_0808135D` | `MainMenuCarouselWheel_Init` |
| `0x080815C0` | `MainMenuC2_IDLE_080815C1` | `MainMenuCarouselWheel_TilesSlideInLoop` |
| `0x0808177C` | `MainMenuC2_IDLE_0808177D` | `MainMenuCarouselWheel_CentreTilePopLoop` |
| `0x080819A0` | `MainMenuC2_IDLE_080819A1` | `MainMenuCarouselWheel_LabelSlideInLoop` |
| `0x08081D30` | `MainMenuC2_IDLE_08081D31` | `MainMenuCarouselWheel_InputLoop` |
| `0x08084544` | `MainMenuC4_IDLE_08084545` | `MainMenuCarouselBg_Loop` |
| `0x080849BC` | `StartCoInfoScreen_080849BD` | `CoInfoScreen_ResetPage` |
| `0x080849C8` | `StartCoInfoScreen_080849C9` | `CoInfoScreen_LoadGraphics` |
| `0x08084BD4` | `CoInfo_08084BD5` | `CoInfoScreen_Init` |
| `0x08085AF4` | `MainMenu_08085AF5` | `ResetMapSelectState` |
| `0x08085F94` | `PutMapPropertiesPreview_08085F95` | `MapSelectList_Init` |
| `0x0808603C` | `PutMapPropertiesPreview_IDLE_0808603D` | `MapSelectList_InputLoop` |
| `0x08086058` | `PutMapPropertiesPreview_IDLE_08086059` | `MapSelectList_DrawLoop` |
| `0x08086D98` | `WarRoomScroll_08086D99` | `MapSelectPreview_LoadMap` |
| `0x08086DF4` | `WarRoomScroll_08086DF5` | `MapSelectPreview_FillTilemap` |
| `0x0808789C` | `PutEnemyCoMinimug_0808789D` | `EnemyCoMinimugs_Init` |
| `0x080878A8` | `PutEnemyCoMinimug_IDLE_080878A9` | `EnemyCoMinimugs_Loop` |
| `0x080879B0` | `PreviewMapRecords_CB_080879B1` | `PreviewMapRecords_OnEnd` |
| `0x080879D8` | `PreviewMapRecords_080879D9` | `PreviewMapRecords_Init` |
| `0x08087A10` | `PreviewMapRecords_IDLE_08087A11` | `PreviewMapRecords_Loop` |
| `0x08087C94` | `CoDesignC1_08087C95` | `CoDesignRoot_Init` |
| `0x08088004` | `CoDesignC1_08088005` | `CoDesignRoot_StartEditor` |
| `0x08088044` | `CoDesignC2_08088045` | `CoDesignEditor_Init` |
| `0x080880BC` | `CoDesignC2_IDLE_080880BD` | `CoDesignEditor_IntroLoop` |
| `0x0808844C` | `CoDesignC2_IDLE_0808844D` | `CoDesignEditor_Loop` |
| `0x0808A3A0` | `PutFace_IDLE_0808A3A1` | `CoDesignBg_Loop` |
| `0x0808A6CC` | `CampaignIntro_0808A6CD` | `CampaignIntro_Init` |
| `0x0808A820` | `CampaignIntro_0808A821` | `CampaignIntro_StartPrologueText` |
| `0x0808A82C` | `CampaignIntro_IDLE_0808A82D` | `CampaignIntro_WaitForTextBoxes` |
| `0x0808A844` | `CampaignIntro_IDLE_0808A845` | `CampaignIntro_WaitForButtonA` |
| `0x0808A884` | `CampaignIntro_IDLE_0808A885` | `CampaignIntro_WaitForSkip` |

## Renamed after reading the code (second review)

These names (AW2E.lua labels or earlier house names) did not match what the
matched C does, so the repo was renamed. `src/AW2E.lua` keeps the original
labels. The `sub_XXXXXXXX` alias of each function is unchanged.

| Address | Was | Now |
| --- | --- | --- |
| `0x08000ccc` | `SetSelectedTile` | `DesignRoomSelectItem` |
| `0x0800164c` | `IsTerrainWater` | `IsTerrainNotWater` |
| `0x0800168c` | `IsTerrainWaterOrRiver` | `IsTerrainNotWaterOrRiver` |
| `0x08011298` | `SomeFade_08011299` | `WipeToBlack_Init` |
| `0x0801137c` | `FadeLoadMap_0801137D` | `WipeFromBlack_Init` |
| `0x080113ec` | `SomeFade_IDLE_080113ED` | `WipeToBlack_Loop` |
| `0x080114a0` | `FadeLoadMap_IDLE_080114A1` | `WipeFromBlack_Loop` |
| `0x0801153c` | `SomeFade_IDLE_0801153D` | `Wipe_End` |
| `0x080116e8` | `StartCoInfoScreen_080116E9` | `LoadGlyphSpriteTiles` |
| `0x08011b18` | `WM_ConfirmExit_08011B19` | `ClearVBlankHooks` |
| `0x08014878` | `EndCoInfoScreen_08014879` | `EndAllTextBoxes` |
| `0x080191b0` | `MainMenu_080191B1` | `ResetEventScriptsAndUiState` |
| `0x08020edc` | `AddValueInRange` | `StampVisionDisc` |
| `0x08028568` | `FinalizeBattleResult` | `FinalizeMatchResult` |
| `0x080345c8` | `MapMainIdle` | `RunMapStateMachine` |
| `0x08034838` | `MainMenu2_08034839` | `ClearSavingEnabled` |
| `0x0803b83c` | `BattleMaps_0803B83D` | `ReturnToMainMenu` |
| `0x0803d48c` | `MainMenu2_0803D48D` | `RefreshDesignRoomSlotDirectory` |
| `0x0803d960` | `BattleMaps_0803D961` | `StartProfileSaveScreen` |
| `0x08044b08` | `AnimateUnitCreation` | `StartUnitSparkleEffect` |
| `0x08048fd8` | `BattleMaps_IDLE_08048FD9` | `ShopScreen_PickGreeting` |
| `0x080490bc` | `BattleMaps_IDLE_080490BD` | `ShopScreen_WaitGreetingThenSlide` |
| `0x08049170` | `BattleMaps_08049171` | `ShopList_ResetRise` |
| `0x08049178` | `BattleMaps_IDLE_08049179` | `ShopList_RiseStep` |
| `0x08049928` | `BattleMaps_IDLE_08049929` | `ShopScreen_WaitMessageEnd` |
| `0x08049b14` | `BattleMaps_08049B15` | `StartShopDrawScript` |
| `0x08049b28` | `BattleMaps_08049B29` | `ShopScreen_EndMessageScripts` |
| `0x08049b70` | `BattleMaps_08049B71` | `ShopScreen_AddScrollBackgroundHook` |
| `0x08049b80` | `BattleMaps_08049B81` | `ShopScreen_UpdateMainMenuLock` |
| `0x08061868` | `RunAiTurn` | `AiBeginTurn` |
| `0x08066bf4` | `HandleRulesMenuInput` | `MatchSetupMoveRuleCursor` |
| `0x08071e80` | `FadeToWhite_OnInit` | `FadeToWhite_OnInitUnused` |
| `0x08071eb8` | `FadeFromWhite_OnInit` | `FadeFromWhite_OnInitUnused` |
| `0x08072cac` | `MainMenu_PutSelectModeSprite_08072CAD` | `HeaderBanner_SnapIfTimerNegative` |
| `0x08072e70` | `MainMenu_PutSelectModeSprite_IDLE_08072E71` | `HeaderBanner_SlideToTargetLoop` |
| `0x08072f70` | `MainMenu_PutSelectModeSprite_IDLE_08072F71` | `HeaderBanner_FlyOutLoop` |
| `0x080730d0` | `MainMenu_PutSelectModeSprite_IDLE_080730D1` | `HeaderBanner_FlyInLoop` |
| `0x080750c0` | `WM_MoveScope_IDLE_080750C1` | `DifficultyStars_PopInLoop` |
| `0x08075248` | `WM_MoveScope_IDLE_08075249` | `DifficultyStars_HoldLoop` |
| `0x080760b4` | `WM_Listener_080760B5` | `WorldMapNationPanel_Init` |
| `0x0807610c` | `WM_Listener_0807610D` | `WorldMapNationPanel_Setup` |
| `0x0807614c` | `WM_Listener_IDLE_0807614D` | `WorldMapNationPanel_WatchLoop` |
| `0x080761c8` | `WM_Listener_IDLE_080761C9` | `WorldMapNationPanel_SlideInLoop` |
| `0x08076298` | `WM_Listener_IDLE_08076299` | `WorldMapNationPanel_SlideOutLoop` |
| `0x08078e04` | `EndCoSelect_08078E05` | `EndScrollingBackdrop` |
| `0x08080f3c` | `MainMenu_08080F3D` | `MainMenuCarousel_ResetSelection` |
| `0x08081334` | `MainMenuC1_08081335` | `MainMenuCarousel_StartWheelProc` |
| `0x08081358` | `MainMenuC1_IDLE_08081359` | `MainMenuCarousel_Idle` |
| `0x08084bd0` | `StartCoInfoScreen_IDLE_08084BD1` | `CoInfoScreen_Idle` |
| `0x08085f90` | `PreviewMap_IDLE_08085F91` | `MapSelect_Idle` |
| `0x08086db4` | `WarRoomScroll_08086DB5` | `MapSelectPreview_RunOverlayRoutine3` |
| `0x08086dd4` | `WarRoomScroll_08086DD5` | `MapSelectPreview_RunOverlayRoutine4` |
| `0x08088040` | `CoDesignC1_IDLE_08088041` | `CoDesignRoot_Idle` |
