#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08081060.
 * sub_08081060 @ 0x08081060
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"

/* WAVE 53 (W53-C). MATCHED. Needs
 * "rodata": ["0x081D9398", "0x081D939C"] in its data/promoted.json entry.
 *
 * Another screen setup out of the same cluster as sub_0807C614.
 *
 * gUnknown_081D9398 and gUnknown_081D939C ARE NOT GLOBALS -- they are this
 * unit's -fforce-addr .rodata words for &gUnknown_0861696C and
 * &gUnknown_03005934, which is why each is reached with TWO `ldr`s. See the
 * note in include/unknown-globals.h.
 *
 * LoadMainMenuSideTileGraphic and LoadMainMenuLabelPlateGraphic each take a SECOND argument that neither
 * callee reads. src/decomp/c_08084580.c defined them without it, because an
 * ignored argument costs the callee nothing; this function is their only
 * caller in the ROM and sets up r1 at all ten call sites, so the caller is the
 * only witness. Both definitions were widened and re-verified byte-identical.
 *
 * The gDispIo byte-1 group is written on both sides of the two window-shadow
 * groups, so agbcc merges it into ONE read-modify-write and the `strb` sinks
 * past both -- the same merge hardware.h already records for SoundRoomMusicPage_Resume.
 * That is automatic; the source order is just the five display enables, then
 * the two shadows, then the four scalars, then the three window enables.
 *
 * THE PALETTE LOOP IS AN ASSOCIATIVITY TELL, and it is worth 6 bytes.
 * `(u16)(0x20 * (i + 0x12))` and `(u16)(0x240 + i * 0x20)` are the same value,
 * but only the first is strength-reduced: loop.c turns the `<< 16` half of the
 * u16 cast into the biv itself (init 0x240 << 16, step 0x20 << 16) and leaves
 * a bare `lsrs r1, r5, #0x10` at the use, which is what the ROM has. Written
 * the other way agbcc folds the constant into the shift and recomputes all
 * five instructions inside the loop. A `u16` variable carried across the loop
 * is a third, also-wrong shape: it truncates at the INCREMENT instead.
 */

void MainMenuCarousel_Init(ProcPtr proc)
{
    int i;

    SetupMenuScreenBgs(proc);
    SyncAllBgTilemaps();

    SetDispEnable(1, 1, 0, 1, 1);

    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 0;
    gUnknown_030030A4.bits.win1_enable_bg2 = 0;
    gUnknown_030030A4.bits.win1_enable_bg3 = 0;
    gUnknown_030030A4.bits.win1_enable_obj = 0;

    gUnknown_030030DC.bits.win0_enable_bg0 = 1;
    gUnknown_030030DC.bits.win0_enable_bg1 = 1;
    gUnknown_030030DC.bits.win0_enable_bg2 = 1;
    gUnknown_030030DC.bits.win0_enable_bg3 = 1;
    gUnknown_030030DC.bits.win0_enable_obj = 1;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0x90;
    gUnknown_030024E4 = 0xf0;
    gUnknown_030020B8 = 0xa0;

    SetWinEnable(1, 1, 0);

    gUnknown_030030E0.bits.effect = 0;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 0;
    gUnknown_03001400 = 0xfff8;

    Decompress(gUnknown_0823A3D4, (void *)((gUnknown_0300251C.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_08239FA4, gBG3TilemapBuffer);
    ApplyPaletteExt((u16 *)gUnknown_0823BDE0, 0, 0x20);

    BG_EnableSyncBG3();

    Decompress(gUnknown_0823BF28, (void *)((gUnknown_030030B4.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0823BE40, gBG2TilemapBuffer);
    ApplyPaletteExt(gUnknown_0823BFD4, 0x20, 0x20);

    BG_EnableSyncBG2();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();

    gUnknown_03005920 = 0;

    LoadMainMenuCentreTileGraphic(gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)]);

    for (i = 0; i < 2; i++)
    {
        LoadMainMenuSideTileGraphic(0, 0x110);
        LoadMainMenuSideTileGraphic(1, 0x150);
        LoadMainMenuSideTileGraphic(2, 0x180);
        LoadMainMenuSideTileGraphic(3, 0x190);
        LoadMainMenuSideTileGraphic(4, 0x1c0);
        LoadMainMenuSideTileGraphic(5, 0x250);
    }

    for (i = 0; i < 2; i++)
    {
        LoadMainMenuLabelPlateGraphic(0, 0x1d0);
        LoadMainMenuLabelPlateGraphic(1, 0x1d6);
        LoadMainMenuLabelPlateGraphic(2, 0x212);
        LoadMainMenuLabelPlateGraphic(3, 0x20c);
    }

    if (IsCampaignCompletionFlagSet(0x20) != 0)
        ApplyPaletteExt(gUnknown_0823DC38, 0x3a0, 0x20);

    MainMenuCarousel_UpdateCompletionFlags();

    for (i = 0; i < 6; i++)
        ApplyPaletteExt(GetMainMenuTilePalette(i), (u16)(0x20 * (i + 0x12)), 0x20);

    ApplyPaletteExt(gUnknown_0823DDB8, 0x340, 0x60);

    LoadCoFullBodyAndPalette(0, 0, 1);
    InitTextTileCache(0);

    ApplyPaletteExt(gUnknown_081320AC, 0x100, 0x20);

    Proc_Start(ProcScr_MainMenuC4, proc);

    if (gUnknown_030058FC != 0)
    {
        ApplyPaletteExt(GetMainMenuTilePalette(gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)]), 0x300, 0x20);
        Proc_Start(gUnknown_08616A40, proc);
    }
}

asm(".global sub_08081060\n.thumb_set sub_08081060, MainMenuCarousel_Init\n");

extern void MainMenuCarousel_StartWheelProc(void);
extern void MainMenuCarousel_Idle(void);

struct ProcCmd CONST_DATA ProcScr_MainMenuC1[] =
{
    PROC_1D(30),
    PROC_CALL(MainMenuCarousel_Init),
    PROC_1E(30),
    PROC_CALL(MainMenuCarousel_StartWheelProc),
    PROC_SLEEP(6),
    PROC_1B(400),
    PROC_REPEAT(MainMenuCarousel_Idle),
    PROC_END,
};

asm(".global gUnknown_08616990\n.set gUnknown_08616990, ProcScr_MainMenuC1\n");
