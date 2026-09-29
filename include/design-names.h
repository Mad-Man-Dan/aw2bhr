#ifndef GUARD_DESIGN_NAMES_H
#define GUARD_DESIGN_NAMES_H

/* Friendly names for the map-editing / design-room family and its
 * neighbours. Two provenances, and the defining src/decomp/c_ADDR.c file
 * says which applies to each: most are cross-referenced against SRR_AW2's
 * Definitions.s (Veslyquix/SRR_AW2, the same author's randomizer), and the
 * rest were named by reading this tree's own C where SRR's label did not
 * match the body. Every one is a linker alias for an existing sub_XXXXXXXX
 * symbol via .thumb_set, so the old declaration in unknown-functions.h is
 * left untouched and existing callers compile unchanged; this header is
 * only for new code that wants to call them by name. */


int MakeRiver(int x, int y);
void MakeMountain(int x, int y);
void MakeForest(int x, int y);
void MakeForestSimple(int x, int y);
void MakeRoad(int x, int y);
void MakeSea(int x, int y);
void MakeSeaSafe(int x, int y);
void MakeSeaSafest(int x, int y);
void MakeBridge(int x, int y);

void MakePipe(int x, int y);
void MakeSeam(int x, int y);
int GetSeamType(int x, int y);
int MakeReefSafe(int x, int y);
void MakeProperty(int x, int y, int t);
void MakeTile(void);
void MakeTile2(int x, int y, int v);
void DesignRoomSelectItem(int a1);
void RunMapStateMachine(void);

void RepaintTile(int x, int y);
void RepaintTileRight(int x, int y);
int RemoveUnitAt(int mode, int x, int y);
void CopyString(u8 *dst, const u8 *src);
void MarkDefeatedArmies(void);
void FinalizeMatchResult(void);

/* Design Room editor and tiler functions, named from reading the matched
 * C. Each is an alias of its sub_XXXXXXXX symbol, like the names above. */
void DesignRoomSetMode(u16);
void DesignRoomRunMode(void);
void DesignRoomEnterPaintMode(void);
void DesignRoomMode_Start(void);
void DesignRoomMode_Paint(void);
void DesignRoomPickUnderCursor(void);
void DesignRoomProc_Loop(void);
void DesignRoomProc_Init(struct Unk03001470 *proc);
void StartDesignRoom(int);
int DesignRoomHandleCursorInput(void);
int DesignRoomFindItemIndex(int);
void DesignRoomUpdateSelectionPanel(void);
void DesignRoomDrawTerrainIcon(int, int, int, int, int, int, int);
void DesignRoomDrawUnitIcon(int, int, int, int, int, int, int);
void DesignRoomDrawTerrainName(int, int, int, int, int, int);
void DesignRoomDrawUnitName(int, int, int, int, int, int);
void DesignRoomDrawTerrainRing(void);
void DesignRoomDrawUnitRing(void);
void DesignRoomLoadTerrainNamePalettes(void);
void DesignRoomLoadGraphics(void);
void DesignRoomDrawUi(void);
void DesignRoomDrawPropertyCounts(void);
void DesignRoomResetArmyPanels(void);
void DesignRoomUpdateArmyPanels(int, int);
void DesignRoomUpdateArmyPanel(int, int, int);
void DesignRoomDrawCoordBox(void);
void DesignRoomUpdateCoordBox(void);
void DesignRoomStartCoordBox(void);
void DesignRoomShowCoordBox(void);
void DesignRoomHideCoordBox(void);
void DesignRoomNewMap(int);
void StampTerrainBlob(int, int, int, int, int);
void DesignRoomMenu_EnterPaintMode(void);
void DesignRoomMenu_CloseToPaint(void);
void DesignRoomMenu_CloseToMainMenu(void);
void DesignRoomOpenMainMenu(void);
void DesignRoomStartNameEntry(void);
void DesignRoomOnNameEntryDone(void);
void DesignRoomMenu_CloseToMainMenu2(void);
void DesignRoomMenu_NewMap(int);
void DesignRoomMenu_NewMapSea(void);
void DesignRoomMenu_NewMapPlain(void);
void DesignRoomMenu_NewMapMountain(void);
void DesignRoomMenu_NewMapForest(void);
void DesignRoomMenu_NewMapRandom(void);
void DesignRoomMode_Menu(void);
void DesignRoomLoadFromSlot(void);
void DesignRoomMenu_PickSlot0(int a, int b, u8 c);
void DesignRoomMenu_PickSlot1(int a, int b, u8 c);
void DesignRoomMenu_PickSlot2(int a, int b, u8 c);
void DesignRoomSaveToSlot(void);
void DesignRoomClearName(void);
int DesignRoomRefreshSlotFlag(int);
void DesignRoomShowSlotPreview0(int, int, int);
void DesignRoomShowSlotPreview1(int, int, int);
void DesignRoomShowSlotPreview2(int, int, int);
void DesignRoomDrawHelpPage1(void);
void DesignRoomDrawHelpPage2(void);
void DesignRoomHelp_Loop(void);
void DesignRoomMode_Ring(void);
int DesignRoomGetPreviousRingIndex(void);
void DesignRoomSaveSelection(void);
void DesignRoomBuildRing(int, int);
void DesignRoomSetUnitArmy(s8);
void DesignRoomBuildItemList(int, int);
void RepaintNeighbours(int, int);
void DesignRoomCountArmyUnits(void);
int DesignRoomPlaceUnitAtCursor(void);
int GetLandNeighbourMask(int, int);
int CanPlaceBridgeAt(int, int);
int IsPlainRiverAt(int, int);
int CountRiverNeighbours(int, int);
int CountRiverOrBridgeNeighbours(int, int);
int CountLandOnSide(int, int, int);
int CanPlaceRiverAt(int, int);
int IsShoalAt(int, int);
int GetShoalNeighbourMask(int, int);
s16 GetShoalTile(int, int);
int MakeShoal(int, int);
void RepaintShoalNeighbours(int, int);
int CanPlaceReefAt(int, int);
int IsSeaAt(int, int);
void AddPropertyRecord(int, int, int);
void RemovePropertyAt(int, int);
int GetArmyHq(int, int *, int *);
void SetArmyHq(int, int, int);
void ClearArmyHq(int);
u32 CountPropertiesOfType(int);
int GetArmyColorSetIndex(void);
int MakeForestBlocks(int, int);
int GetForestColumnMask(int, int);
int GetForestBlockCorner(int, int);
void MakeForestBlock2x2(int, int);
void MakeForestBlock3x3(int, int);
int IsRoadOrBridgeAt(int, int);
int IsRoadOrHorizontalBridgeAt(int, int);
int IsRoadOrVerticalBridgeAt(int, int);
int GetRoadTile(int, int);
int GetPipeConnectionAt(int, int, int);
int GetPipeTile(int, int, int);
void RepaintPipesAround(int, int);
void DesignRoomShowTilePanel(void);
void DesignRoomHideTilePanel(void);

#endif /* GUARD_DESIGN_NAMES_H */
