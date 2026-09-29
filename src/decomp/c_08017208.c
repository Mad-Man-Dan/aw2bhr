#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08017208.
 * sub_08017208 @ 0x08017208
 */




/* gUnknown_02023284 and ExpandPipeSeamHpPlane are declared here rather than in a header
 * because nothing else in src/decomp uses them. The two structs are the save
 * block sub_08016F38 (src/decomp/c_08016F38.c) writes; that file carries its
 * own copy of the same declaration. SaveBlkRec is one entry of the
 * map-difference list at 0x0bb8 -- a column, a row and a tile number. */

extern u8 gUnknown_02023284[];
void ExpandPipeSeamHpPlane(u8 *);
struct SaveBlkRec
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
};
struct SaveBlk
{
    /* 0x0000 */ u16 unk0000;
    /* 0x0002 */ u16 unk0002;
    /* 0x0004 */ struct Unk802C57C unk0004;
    /* 0x0008 */ struct Unit unk0008;
    /* 0x0014 */ u8 unk0014[0x140 - 0x14];
    /* 0x0140 */ u8 unk0140[0x48];
    /* 0x0188 */ struct Unit unk0188[4 * 51];
    /* 0x0b18 */ int unk0b18[4];
    /* 0x0b28 */ u8 filler_0b28[0xb98 - 0xb28];
    /* 0x0b98 */ struct Unk03002F08 unk0b98;
    /* 0x0ba0 */ void (*unk0ba0)(void);
    /* 0x0ba4 */ bool8 (*unk0ba4)(void);
    /* 0x0ba8 */ u32 unk0ba8;
    /* 0x0bac */ u8 unk0bac;
    /* 0x0bad */ u8 filler_0bad[1];
    /* 0x0bae */ u16 unk0bae;
    /* 0x0bb0 */ u16 unk0bb0;
    /* 0x0bb2 */ u16 unk0bb2;
    /* 0x0bb4 */ u16 unk0bb4;
    /* 0x0bb6 */ u16 unk0bb6;
    /* 0x0bb8 */ struct SaveBlkRec unk0bb8[(0xd28 - 0xbb8) / 4];
    /* 0x0d28 */ struct Unk02028360 unk0d28[16];
    /* 0x0da8 */ u8 unk0da8[4];
};

/*
 * RestoreBattleSaveState -- load the game state back out of the save block.
 *
 * The reverse of sub_08016F38 (src/decomp/c_08016F38.c), which wrote the block
 * at gUnknown_02000000.
 *
 *   1. gUnknown_030032D8 takes 5 or 0xc depending on the block's .unk0bac
 *      flag. The loose globals, gPlaySt (0x48 bytes) and the unit at
 *      gUnknown_03004490 are copied back out. Two of gUnknown_0200C420's
 *      fields are re-derived from gPlaySt rather than restored.
 *   2. The map's size and scroll position are restored, and the camera
 *      position is recomputed as the scroll position divided by 16.
 *   3. Unless the map ID is one of 0xb4..0xbf, rebuild the map: GetMapArmyCount
 *      and GetMapName for its header word and its name, LoadMapData for its
 *      tiles, then copy the pristine tiles at gUnknown_03003F68 into the live
 *      map and replay the difference list at .unk0bb8 over the top, stopping
 *      at the record whose tile is 0xffff.
 *   4. Copy the five 0x3c-byte records, the four armies' 51 units each and the
 *      sixteen gUnknown_02028360 entries back where sub_08016F38 took them
 *      from, and hand the last four bytes to ExpandPipeSeamHpPlane.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The tile copy splits the byte offset out as its own statement
 *     (`off = idx * 2;`) while the destination stays the array reference
 *     `gMap->tile[idx]`. Folding the multiply into the read, or spelling the
 *     destination as a pointer, swaps which register holds the index and which
 *     holds the constants.
 *   - gUnknown_030032D8 is an if/else with two separate stores and not a `?:`,
 *     because the original loads the symbol's address in both arms.
 *   - `gUnknown_030033EC = v = p->unk0002;` and `map->scrollX = a = p->unk0bb2;`
 *     are single chained statements. Split in two they load the value before
 *     the destination's address; `a` and `b` also keep the values alive for the
 *     later .unk08 and .unk0a stores.
 *   - gUnknown_03003F2C is computed from gUnknown_030033EC read back out of
 *     memory while its neighbours use the local `v`. That is what the original
 *     does: the store to gUnknown_03004084 in between is what forces the
 *     reload.
 *   - `a`, `b` and `v` are three separate locals and the map-ID test is written
 *     out twice. Sharing a local, or merging the two blocks under one test,
 *     changes the register assignment.
 */
void RestoreBattleSaveState(void)
{
    struct SaveBlk *p = (struct SaveBlk *)gUnknown_02000000;
    struct Map *map;
    s16 i, j, x, y;
    int idx, v, a, b, off;

    if (p->unk0bac)
        gUnknown_030032D8 = 5;
    else
        gUnknown_030032D8 = 0xc;
    gUnknown_03001FD4 = p->unk0ba8;
    gUnknown_030033E4 = p->unk0004;
    gUnknown_03004080 = p->unk0000;
    gUnknown_030033EC = v = p->unk0002;
    gUnknown_03004084 = v * 0x20;
    gUnknown_03003F2C = (gUnknown_030033EC - 1) * 0x40;
    gUnknown_03004480 = v;
    sub_0808B6E8(&gPlaySt, p->unk0140, 0x48);
    *(struct Unit *)gUnknown_03004490 = p->unk0008;
    gUnknown_0200C420.unk0e = gPlaySt.animOpts;
    gUnknown_0200C420.unk14 = (gPlaySt.bgmOn == 0);
    for (i = 0; i < 4; i++)
        gUnknown_030033F4[i] = p->unk0b18[i];
    gUnknown_03002F08 = p->unk0b98;
    gUnknown_03002F20 = p->unk0ba0;
    gUnknown_03001FF0 = p->unk0ba4;
    map = gMap;
    map->width = p->unk0bae;
    map->height = p->unk0bb0;
    map->scrollX = a = p->unk0bb2;
    map->scrollY = b = p->unk0bb4;
    map->camX = map->scrollX / 16;
    map->camY = map->scrollY / 16;
    map->unk08 = a;
    map->unk0a = b;
    map->unk10 = p->unk0bb6;
    if (gPlaySt.mapID < 0xb4 || gPlaySt.mapID > 0xbf)
    {
        gMap->unk4233 = GetMapArmyCount(gPlaySt.mapID);
        CopyString(gMap->unk421a,
                     GetMapName(gPlaySt.mapID));
        LoadMapData(gPlaySt.mapID);
        InitMapRowOffsets();
        for (y = 0; y < gMap->width; y++)
        {
            for (x = 0; x < gMap->height; x++)
            {
                idx = gMap->rowOffset[x] + y;
                off = idx * 2;
                gMap->tile[idx] =
                    *(u16 *)((u8 *)gUnknown_03003F68 + off + 2);
            }
        }
        sub_0802481C();
    }
    if (gPlaySt.mapID < 0xb4 || gPlaySt.mapID > 0xbf)
    {
        for (i = 0; p->unk0bb8[i].unk02 != 0xffff; i++)
        {
            gMap->tile[
                gMap->rowOffset[p->unk0bb8[i].unk01]
                + p->unk0bb8[i].unk00] = p->unk0bb8[i].unk02;
        }
    }
    for (i = 0; i < 5; i++)
        sub_0808B6E8(&gUnknown_02023284[i * 0x3c], (u8 *)p + i * 0x3c + 0x14, 0x3c);
    for (i = 0; i < 4; i++)
        for (j = 0; j < 51; j++)
            gUnknown_02022684[i * 64 + j] = p->unk0188[i * 51 + j];
    for (i = 0; i < 16; i++)
        gUnknown_02028360[i] = p->unk0d28[i];
    ExpandPipeSeamHpPlane(p->unk0da8);
}
asm(".global sub_08017208\n.thumb_set sub_08017208, RestoreBattleSaveState\n");
