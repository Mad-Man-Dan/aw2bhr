#ifndef GUARD_XENESIS_NAMES_H
#define GUARD_XENESIS_NAMES_H

/* Friendly names for functions cross-referenced against Xenesis's AW2
 * Datasheet / Subroutine List (community reverse-engineering notes, kept
 * outside this repository). Each one is a linker alias for an existing
 * sub_XXXXXXXX symbol -- see the defining src/decomp/c_ADDR.c file for the
 * `.thumb_set` directive and the Xenesis citation. The old sub_XXXXXXXX
 * declaration in unknown-functions.h is left untouched so every existing
 * caller keeps compiling unchanged; this header is only for new code that
 * wants to call these by name.
 */

void SubtractPlayerFunds(u16 player, u32 amount);
void AddPlayerFunds(u16 player, u32 amount);
void SubtractUnitFuel(struct Unit *unit, u8 amount);
bool8 IsPlayerAliveAndActive(u8 player);
void AddPlayerIncomeToFunds(void);
int IsCampaignMapUnlocked(u32 mapId);
void SetPlayerCoPowerStatus(int player);
void ClearPlayerCoPowerStatus(int player);
u32 GetNextRandomNumber(void);
int GetLoadedCoPalette(int index);
void SetLoadedCoPalette(int index, u8 value);
int GetCoAttackBonus(int coId, int coPowerState, int unitType);
int GetCoDefenceBonus(int coId, int coPowerState, int unitType);
int GetCoMovementBonus(int coId, int coPowerState, int unitType);
int GetCoRangeBonus(int coId, int coPowerState, int unitType);
int GetUnitCombatClassColumn(int unitType);
u32 TrySpendBattleMapPoints(u32 amount);
struct Unit *CreateUnitAt(s16 a1, s16 a2, s16 a3);
void *BuyUnit(int a1, int a2, int a3);
bool8 IsNeotanksUnlocked(int army);
int IsCampaignCompletionFlagSet(int id);
u32 GetClampedRankScoreSum(u8 player);
u8 GetSpeedScore(u8 player);
u8 GetPowerScore(u8 player);
u8 GetTechScore(u8 player);
void DrawInfoBoxCombobox(void);
void StartSupplyAnimation(struct Unk802C57C *a1, u8 a2, int a3, u8 a4);
bool8 UnitMenu_DiveUsability(void);
void SetDefaultRules(void);
void ResetRulesAfterCampaignMap(void);
void PlayWeatherChangeSound(struct Unk03001470 *p);
void ChangeGameWeather(u8 weatherId);
void LoadWeatherData(void);
void UpdateFuelAmmoGraphics(void);
void CheckSoftResetCombo(void);
int GetRankFromScore(int score);
int GetCampaignScoreRank(int score);
int IsHardCampaignMode(void);
void PlayMusicOrSfx(s16 soundId);
void ActivateMusicOrSoundId(u16 songId);
bool8 HasSupplyAbility(u8 *p);
void DestroyPipeSeam(int x, int y, ProcPtr parent);
int GetUnitBaseMovement(int a);
int GetUnitBaseFiringRange(int a);
int GetUnitBaseCost(int a);
int GetUnitBaseVision(int a);
int GetCoCostBonus(int coId, int coPowerState, int unitType);
int GetCoVisionBonus(int coId, int coPowerState);
int GetCoLuckBonus(int coId, int coPowerState);
int GetPlayerCoLuckBonus(int player);
int GetCoNegativeLuckBonus(int coId, int coPowerState);
int GetPlayerCoNegativeLuckBonus(int player);
int GetCoCounterattackBonus(int coId, int coPowerState);
int GetPlayerCoCounterattackBonus(int player);
int GetUnitCostWithCoBonus(int player, int unitType);
int GetUnitAttackWithCoBonus(int player, int unitType);
int GetUnitDefenceWithCoBonus(int player, int unitType);
int GetUnitMovementWithCoBonus(int player, int unitType);
int GetUnitFiringRangeWithCoBonus(int player, int unitType);
int GetUnitVisionWithCoBonus(int player, int unitType);
int IsUnitDirectForCoPowerAnim(struct Unk030040D8 *p);
int HasMaxRangeGreaterThanOne(struct Unk030040D8 *p);
void HealUnitHyperRepair(void *unit);
void HealUnitHyperUpgrade(void *unit);

#endif // GUARD_XENESIS_NAMES_H
