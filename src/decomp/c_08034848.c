#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034848.
 * sub_08034848 @ 0x08034848, sub_0803486C @ 0x0803486C, sub_08034890 @ 0x08034890
 */

/* The 1 written to unk0c, unk02 and unk09 is one CSEd constant that has to
 * survive two calls, which is why the ROM parks it in r5 behind
 * `push {r4, r5, lr}` -- plain literals reproduce it. */
void InitVersusPlayState(void)
{
    gPlaySt.bgmOn = 1;
    gPlaySt.gameMode = 3;
    gPlaySt.mapID = 1;
    SetDefaultRules();
    sub_08034838();
    gPlaySt.animOpts = 1;
}
asm(".global sub_08034848\n.thumb_set sub_08034848, InitVersusPlayState\n");

/* TWO statements, not `a = b = v`. The chained form hoists all three pool words
 * ahead of the loads and comes out with the stores swapped; written as two
 * statements CSE keeps the loaded word in r1 and the second address load lands
 * between the two `str`s, which is the ROM's order. */
void InitRecordListPointersAndTerrainTable(void)
{
    gUnknown_03003338 = gUnknown_0849FE74[0];
    gUnknown_03003F20 = gUnknown_03003338;
    LoadTileTerrainTable();
}
asm(".global sub_0803486C\n.thumb_set sub_0803486C, InitRecordListPointersAndTerrainTable\n");

void InitMapGameState(void)
{
    InitGameSettings();
    LoadMapIntoGMap(gPlaySt.mapID);
    InitNewMapState();
    RecountPropertiesIncomeAndAiFacilities();
    StartArmyTurn();
}
asm(".global sub_08034890\n.thumb_set sub_08034890, InitMapGameState\n");
