#ifndef GUARD_DATA_NAMES_H
#define GUARD_DATA_NAMES_H

/* Friendly names for the small accessors over the ROM data tables in
 * include/unknown-globals.h -- the per-unit-type, per-terrain, per-CO and
 * map-header records. Each name was read off this tree's own C once the
 * record's fields had been named from the community Nightmare modules; the
 * field names are what make these bodies one-liners, so the provenance is the
 * module for the FIELD and this tree for the FUNCTION. Every one is a linker
 * alias for an existing sub_XXXXXXXX symbol via .thumb_set, so the old
 * declaration in unknown-functions.h stays and existing callers compile
 * unchanged; this header is only for new code that wants to call them by name.
 */

const u8 * GetTerrainNameGraphic(int a1);
const u8 * GetTerrainNamePalette(int a, int b);
void *GetUnitSpriteFormat(u16 i);
u16 GetCoMusic(u8 a1);
int GetCoPowerStarCost(int a1);
int GetCoPowerCost(int a1);
int GetSuperCoPowerCost(int a1);
int GetTerrainDefense(int a1, s8 a2, u8 a3);
int IsUnitSoldier(struct Unk030040D8 *p);
int IsUnitAir(struct Unk030040D8 *p);

void *GetCoPowerAnimation(int a);
void *GetCoPowerAnimationPalette(int a);
int IsCoPowerAvailable(int a1);
bool8 IsTerrainSilo(s16 a, s16 b);
void CalcRandomWeatherChances(void);
void DrawDaysRemaining(int x, int y);
bool8 IsPlayer1TeamAlive(void);
void DrawOamObject(int a1, int a2, int a3, int a4, int a5);
int RepairUnit(struct Unit *p, u16 a2, u8 a3);

int HasNoDeathRay(void);
int HasNoLaser(void);
int HasNoBlackCannon(void);
int HasNoMinicannon(void);
int HasNoPipeSeams(void);
bool8 HasLivingInventionOfType(int a1);
int CountUnitsOfType(int a1);
bool8 ShouldPromptCountryName(void);

void AiDoNothing(void);
void AiChargeAggressively(void);
void AiMoveWithFrontLine(void);
void AiMoveUpConservatively(void);
void AiProtectHq(void);
void AiConsiderBuildingTCopter(void);
void AiConsiderBuildingApc(void);
void AiConsiderBuildingLander(void);
int GetCampaignMissionId(void);
void ClearArmyCount(void);

bool8 CopUnitCondAlways(void *unit);
u8 CopCondAlways(int mode);
int CopCondStartOfTurn(void);
int CopCondTurnBoundary(int a);
int CopCondUnitsInjured(void);
int CopCondNotSnowing(void);
void LoadMapData(u16 a1);
void DrawWindowBackground(int a1, int a2, int a3, int a4, u16 *a5, int a6);
void EnableCoPowers(void);
void DisableCoPowers(void);

u32 GetCoPowerCharge(int a1);
u8 IsCoPowerReady(int a1);
u8 IsPlayer1CoPowerReady(void);
bool8 HasPlayer1CoPowerCharge(void);
void CampaignMapNoOp(int a);
void StartCoPowerScript(int a, int b, ProcPtr proc);
int CountBuildablePropertiesOfKind(int a1);
u8 PickWeightedAiUnit(u8 a1);

void CacheUnitMovementCosts(int a1);
void StartUnitSparkleEffect(u8 a1, u8 a2, u8 a3);
int GetFirepowerIcon(int a1, int a2);
void DrawUnitAt(u16 x, u16 y);
void AiBeginTurn(void);

void GenerateRandomMap(void);
void InitGameSettings(void);
void LoadGameplayGraphics(int a);
void MatchSetupMoveRuleCursor(void);

void SetupCoSelectLiberation(void);
void SetupCoSelectNeotanksBm(void);
void SetupCoSelectFactoryBlues(void);
void SetupCoSelectNeotanksYc(void);
void SetupCoSelectHuntsEnd(void);
void SetupCoSelectNeotanksGe(void);
void SetupCoSelectGreatSeaBattle(void);
void SetupCoSelectHotPursuit(void);
void SetupCoSelectFinalFront(void);

bool8 HasNoPlayer2Battleship(void);
void AiPickUnitToBuild(void);
void AiCalcBuildPriorities(void);
int AiBuildPropertyList(void);
void AiScanBuildableFacilities(void);

int CountUnitsWithTypeTag(int a1);
u32 GetPlayerSpecialAbilities(int a1);
int GetCoCaptureRate(int a, int b);
void SpendCoPowerCharge(int a1, int a2);
void ActivateCoPower(int a1, int a2, ProcPtr a3);
void FillMapBuffer(u8 *dst, int value);

void FillMovementMap(u8 a);
void AiMarkAffordableUnits(void);
u8 AiSelectBuildUnit(void);
void DrawCoPowerLabel(int x, int y, int i);

void CopUnitResupply(void *p);
void InitUnit(struct Unit *a1, u8 a2);
/* These two take structs that live in their own .c file; forward-declare the
 * tags so the prototypes below do not introduce them inside a parameter list
 * (agbcc warns, and warnings are errors here). */
struct Unk40640Proc;
struct Unk807D800;
void ApplySiloDamage(struct Unk40640Proc *proc);
int IsCoSelectionUnique(struct Unk807D800 *p);
bool8 CanTransportCarry(struct Unit *a1, u8 a2);

int RandRange(int a, int b);
void ClearAllUnits(void);
void FixShorelineAt(int x, int y, int kind);
void FixAllShorelines(void);
int RegisterArmyHqs(void);
int CountProperties(void);
void RecountArmyProperties(void);

void LoadSeaAnimFrame(int a1);
void LoadRiverAnimFrame(int a1);
int CountUnitsByDeployLocation(int a1);
void BlitMapRow(u16 a1, u16 a2, u16 a3, u16 a4);
void BlitMapColumn(u16 a1, u16 a2, u16 a3, u16 a4);

#endif /* GUARD_DATA_NAMES_H */
