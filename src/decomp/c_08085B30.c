#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085B30.
 * sub_08085B30 @ 0x08085B30
 */

#include "hardware.h"
#include "proc.h"
/* Sets up the campaign-map list screen: graphics, palettes, the two
 * windows, and the gUnknown_08616CF4 proc.
 *
 * The first two-copy loop is spelled `gUnknown_0200FC50 + (i + 2) * 0x80`
 * and `+ (i + 10) * 0x80`, not `+ 0x100 + i * 0x80` (the first is what
 * matters; the second matches either way). With the constant outside the
 * index, the two CpuFastSet givs trade r6/r7. Same associativity rule as the
 * fifth loop's `(i + 3) * 0x20`. Found by the wave-90 permuter as a local
 * `off = 0x100` added to the base, which is the same thing.
 *
 * Load-bearing, from earlier waves:
 *  - the `register int result asm("r0")` binding of sub_08087248's result
 *    (without it the gUnknown_03002B34 address load moves above the call);
 *  - the volatile store of the CpuSet fill zero;
 *  - the fifth loop as `+ (i + 3) * 0x20`, not `+ 0x60 + i * 0x20`.
 * gUnknown_081D93FC/9400/9404/9408 are this unit's -fforce-addr .rodata
 * words; promotion needs "rodata": ["0x081D93FC", "0x081D9400",
 * "0x081D9404", "0x081D9408"]. */

struct Unk8085B30
{
    /* 00 */ STRUCT_PAD(0x00, 0x4e);
    /* 4e */ u16 unk4e;
};

void sub_08085B30(struct Unk8085B30 *proc)
{
    int i;
    u16 clear[2];

    SetupMenuScreenBgs(proc);
    SyncAllBgTilemaps();

    sub_0801F114();
    InitTilePool(2, (void *)0x06010000, 0xf0, 0x14);

    LoadTilePoolGraphic(0x43);
    LoadTilePoolGraphic(0x44);
    LoadTilePoolGraphic(0x45);
    LoadTilePoolGraphic(0x46);
    LoadTilePoolGraphic(0x52);
    LoadTilePoolGraphic(0x69);

    for (i = 0; i <= 9; i++)
        LoadTilePoolGraphic(i + 0x55);

    InitTilePool(4, (void *)((gUnknown_03002B6C.bits.chr_block << 14) + 0x06000000), 0x370, 9);

    LoadTilePoolGraphic(0xb8);
    LoadTilePoolGraphic(0xb9);
    LoadTilePoolGraphic(0xba);
    LoadTilePoolGraphic(0xbb);

    LoadMapSelectPanelGraphics();
    InitTextTileCache(0);
    StartMapPreviewPalette(6);

    if (gPlaySt.gameMode == 2)
    {
        gUnknown_03005928 = 7;
        BuildMapListForMode(2);
        sub_08086A58(gUnknown_03005900, gUnknown_03005928, 0);
        BuildMapSelectPreviewNow(gUnknown_03005900 + gUnknown_03005930);
    }
    else
    {
        gUnknown_03005928 = 5;

        while (BuildMapListForCategory(gUnknown_0300596C) == 0)
        {
            gUnknown_0300596C--;

            if (gUnknown_0300596C <= 1)
                gUnknown_0300596C = 8;
        }

        sub_08086A58(gUnknown_03005990[gUnknown_0300596C], gUnknown_03005928, 0);
        BuildMapSelectPreviewNow(gUnknown_03005990[gUnknown_0300596C] + gUnknown_03005980);
    }

    gUnknown_0300251C.bits.tm_block = 0x1b;
    gUnknown_03001FE8.bits.size = 2;

    *(volatile u16 *)clear = 0;
    CpuSet(clear, (void *)((gUnknown_03001FE8.bits.tm_block << 11) + 0x06000000), 0x01000800);

    StartScrollingBackdrop(proc);

    StartHeaderBanner(gUnknown_085802CC, gUnknown_0200FC50, 0, 0, 1, 1, (int)proc);

    LoadWindowFrameGraphics((void *)((gUnknown_030030B4.bits.chr_block << 14) + 0x06006C00), 0, 0);

    if (gPlaySt.gameMode == 2)
    {
        sub_0801A368(-1, 4, 0xe, gUnknown_03005928 * 2 + 2, gUnknown_08499580, 0);
        sub_0801A368(0x12, 0, 0xd, 4, gUnknown_08499580, 0);
    }
    else
    {
        sub_0801A368(-1, 8, 0xc, gUnknown_03005928 * 2 + 2, gUnknown_08499580, 0);
        sub_0801A368(-1, 4, 0xc, 4, gUnknown_08499580, 0);
        sub_0801A368(0x12, 0, 0xd, 4, gUnknown_08499580, 0);
    }

    BG_EnableSyncBG2();
    sub_080116E8();

    ApplyPaletteExt(gUnknown_084892EC, 0x220, 0x20);

    LoadTerrainObjTiles(6, 0, (void *)0x06010A00, 1);
    LoadTerrainObjTiles(0xe, 0, (void *)0x06010B00, 1);
    LoadTerrainObjTiles(0xa, 0, (void *)0x06010C00, 1);
    LoadTerrainObjTiles(0xb, 0, (void *)0x06010D00, 1);

    ApplyPaletteExt((u16 *)sub_0802A8AC(6, 0), 0x240, 0x20);

    Decompress(gUnknown_0823E140, gUnknown_0200FC50);

    for (i = 0; i < 2; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + (i + 2) * 0x80, (void *)(0x06010E00 + i * 0x100), 0x20);
        CpuFastSet(gUnknown_0200FC50 + (i + 10) * 0x80, (void *)(0x06010E80 + i * 0x100), 0x20);
    }

    CpuFastSet(gUnknown_0200FE50, (void *)0x06011000, 0x10);
    CpuFastSet(gUnknown_0200FE50 + 0x400, (void *)0x06011040, 0x10);

    ApplyPaletteExt(gUnknown_0823E550, 0x2a0, 0x20);

    Decompress(gUnknown_0823E654, gUnknown_0200FE50 - 0x200);

    for (i = 0; i < 3; i++)
        CpuFastSet(gUnknown_0200FE50 - 0x200 + i * 0x20, (void *)(0x06011100 + i * 0x20), 8);

    for (i = 0; i < 5; i++)
        CpuFastSet(gUnknown_0200FC50 + (i + 3) * 0x20, (void *)(0x06011160 + i * 0x20), 8);

    ApplyPaletteExt(gUnknown_08239DE4, 0x2c0, 0x20);

    proc->unk4e = 0;

    {
        register int result asm("r0");
        result = GetMapPreviewScrollX();
        gUnknown_03002B34 = result;
    }
    gUnknown_03002F18 = GetMapPreviewScrollY();

    MapSelect_SetBlend();

    PlayMusic(0x190);

    Proc_Start(gUnknown_08616CF4, proc);
}
