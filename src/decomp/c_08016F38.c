#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016F38.
 * CaptureBattleSaveState @ 0x08016F38
 *
 * Requires the pool word to be PLACED on promotion:
 *   "rodata": ["0x0808E550"]   (the -fforce-addr word for &gPlaySt)
 *
 * Not a Xenesis-documented name. The old sub_08016F38 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

struct UnkStruct_02023284
{
    STRUCT_PAD(0x00, 0x3C);
};

struct SavedTileChange
{
    /* 0x00 */ u8 y;
    /* 0x01 */ u8 x;
    /* 0x02 */ u16 tile;
};

struct BattleSaveState
{
    /* 0x0000 */ u16 unk0000;
    /* 0x0002 */ u16 unk0002;
    /* 0x0004 */ struct Unk802C57C unk0004;
    /* 0x0008 */ struct Unit unk0008;
    /* 0x0014 */ struct UnkStruct_02023284 unk0014[5];
    /* 0x0140 */ struct PlaySt playState;
    /* 0x0188 */ struct Unit units[4 * 51];
    /* 0x0b18 */ int unk0b18[4];
    /* 0x0b28 */ u8 filler_0b28[0xb98 - 0xb28];
    /* 0x0b98 */ struct Unk03002F08 unk0b98;
    /* 0x0ba0 */ void (*unk0ba0)(void);
    /* 0x0ba4 */ u8 (*unk0ba4)(void);
    /* 0x0ba8 */ u32 unk0ba8;
    /* 0x0bac */ u8 unk0bac;
    /* 0x0bad */ u8 filler_0bad[1];
    /* 0x0bae */ u16 unk0bae;
    /* 0x0bb0 */ u16 unk0bb0;
    /* 0x0bb2 */ u16 unk0bb2;
    /* 0x0bb4 */ u16 unk0bb4;
    /* 0x0bb6 */ u16 unk0bb6;
    /* 0x0bb8 */ struct SavedTileChange changedTiles[(0xd28 - 0xbb8) / 4];
    /* 0x0d28 */ struct Unk02028360 unk0d28[16];
    /* 0x0da8 */ u8 unk0da8[4];
};

/* Saves the current battle/map state into the gUnknown_02000000 save block,
 * the mirror of sub_08017208 (restore). Map cells are stored as a list of the
 * cells that differ from the map's base layout, terminated by tile 0xffff.
 *
 * Matched by Eebit on decomp.me (scratch A6WrC). It replaces the wave-86
 * spelling, which needed an empty `asm volatile` register pin, a split
 * byte-offset statement and hand-written sub_0808B6E8 calls. The block copies
 * here are struct assignments, which agbcc emits as `bl memcpy`, and memcpy is
 * sub_0808B6E8. */
void CaptureBattleSaveState(u8 enabled)
{
    struct BattleSaveState *save = (struct BattleSaveState *)gUnknown_02000000;
    s16 i;
    s16 j;
    s16 y;
    s16 x;

    if (enabled)
        save->unk0bac = 1;
    else
        save->unk0bac = 0;
    save->unk0ba8 = gUnknown_03001FD4;
    save->unk0004 = gUnknown_030033E4;
    save->unk0000 = gUnknown_03004080;
    save->unk0002 = gUnknown_030033EC;
    save->playState = gPlaySt;
    save->unk0008 = *(struct Unit *)gUnknown_03004490;
    for (i = 0; i < 4; i++)
        save->unk0b18[i] = gUnknown_030033F4[i];
    save->unk0b98 = gUnknown_03002F08;
    save->unk0ba0 = gUnknown_03002F20;
    save->unk0ba4 = gUnknown_03001FF0;

    save->unk0bae = gMap->width;
    save->unk0bb0 = gMap->height;
    save->unk0bb2 = gMap->scrollX;
    save->unk0bb4 = gMap->scrollY;
    save->unk0bb6 = gMap->unk10;

    if ((u8)(gPlaySt.mapID + 0x4c) > 0xb)
    {
        LoadMapData(gPlaySt.mapID);
        i = 0;
        for (y = 0; y < gMap->width; y++)
        {
            for (x = 0; x < gMap->height; x++)
            {
                if (gMap->tile[gMap->rowOffset[x] + y] != gUnknown_03003F68->tile[gMap->rowOffset[x] + y])
                {
                    save->changedTiles[i].tile = gMap->tile[gMap->rowOffset[x] + y];
                    save->changedTiles[i].y = y;
                    save->changedTiles[i].x = x;
                    i++;
                }
            }
        }
        save->changedTiles[i].tile = 0xffff;
        sub_0802481C();
    }
    for (i = 0; i < 5; i++)
        save->unk0014[i] = ((struct UnkStruct_02023284 *)gUnknown_02023284)[i];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 51; j++)
            save->units[i * 51 + j] = gUnknown_02022684[i * 64 + j];
    for (i = 0; i < 16; i++)
        save->unk0d28[i] = gUnknown_02028360[i];
    sub_08045700(save->unk0da8);
}

asm(".global sub_08016F38\n.thumb_set sub_08016F38, CaptureBattleSaveState\n");
