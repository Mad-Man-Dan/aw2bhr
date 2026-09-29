#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080849C8.
 * sub_080849C8 @ 0x080849C8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"

/* WAVE 71 (W71-F): MATCHED configured, 520/520. A saved-parent alias fixed
 * to r9 and used at all three parent call sites reproduces the ROM allocation.
 * The four expected .rodata symbol-name differences resolve identically.
 *
 * PARKED at 48.3% / +8 bytes, wave 54 (W54-D). The score is POSITIONAL and
 * misleading: every statement, every constant, every pool word and the loop
 * are right, and the entire residual is WHICH REGISTER `parent` lands in.
 *
 * ROM: `mov r9, r0` -- the parameter goes straight into a high register and the
 * three -fforce-addr word ADDRESSES take r5/r7/r8, all three `ldr`d in the
 * loop preheader. Here: `adds r7, r0, #0` -- `parent` wins the low callee-saved
 * r7, so the 0x081D93E4 word is pushed to r8 and the 0x081D93E8 word is created
 * late in r9, which costs one extra `mov rLOW, rHIGH` before its `ldr` and one
 * extra pool slot. +8 bytes, all of it in the last three statements.
 *
 * SETTLED (each read out of baserom.gba, and all four are -fforce-addr pool
 * words, NOT objects -- do not declare a gUnknown_081D93xx for any of them):
 *   0x081D93DC -> 0x08499598  gPlayers (already `struct PlayerStruct *`)
 *   0x081D93E0 -> 0x03003FC0  gPlaySt
 *   0x081D93E4 -> 0x08616BE4  the Proc_Start script
 *   0x081D93E8 -> 0x08043591  AnimateCoPowerStatusPalette, THUMB bit set
 * The loop's `+0x56` off a 0x3c-stride element is `[i + 1].unk1a`, i.e. armies
 * 1..n, not an out-of-range field.
 *
 * RULED OUT by compile_probe: binding the script and the function pointer to
 * locals before the loop. It does move `parent` out of r7 -- but into sl, with
 * a THIRD high register saved -- and, worse, it DEFEATS -fforce-addr: agbcc
 * then emits plain `.word ProcScr_CoInfo` / `.word AnimateCoPowerStatusPalette` pool words
 * where the ROM has the double indirection through 0x081D93E4/E8. The naming-
 * the-symbol-directly spelling below is the one that reproduces those.
 * NOT tried: decomp-permuter. This is exactly its case (same instructions,
 * same order, wrong registers) and is the first thing to try on this function.
 */

void CoInfoScreen_LoadGraphics(ProcPtr parent)
{
    int i;
    register ProcPtr savedParent asm("r9") = parent;

    SetupMenuScreenBgs(savedParent);
    gDispIo.disp_ct.bg1_enable = 0;
    StartScrollingBackdrop(savedParent);
    LoadCoInfoUnitSheet(0, gUnknown_030033EC);
    LoadWindowFrameGraphics((void *)(gUnknown_030030B4.bits.chr_block * 0x4000 + 0x06006C00),
                 gUnknown_08616B1C[gPlayers[gUnknown_030033EC].teamColor], 0);
    CoInfoScreen_LoadBg2Backdrop();
    LoadCoFullBodyAndPalette(gPlayers[gUnknown_030033EC].co, 0xB6 * 2, 5);
    LoadCoMiniPortrait(gPlayers[gUnknown_030033EC].co, (void *)0x06015700, 0x16);
    sub_08043B44(8);
    LoadCoNameGraphic(gPlayers[gUnknown_030033EC].co, 0xAB * 4);
    sub_0801F114();
    InitTilePool(0, (void *)0x06010000, 0xB1 * 4, 0x12);
    InitTilePool(1, (void *)0x06010000, 0xB3 * 4, 0x13);
    InitTilePool(2, (void *)0x06010000, 0xB7 * 4, 0x14);
    LoadTilePoolGraphic(0x13);
    LoadTilePoolGraphic(0x14);

    for (i = 0;
         i < (gPlaySt.gameMode == 2 ? GetMapArmyCount(gPlaySt.mapID)
                                           : GetLoadedMapArmyCount());
         i++)
        LoadTilePoolGraphic(gPlayers[i + 1].teamColor + 0x3D);

    LoadTilePoolGraphic(0x9B);
    LoadTilePoolGraphic(0x9C);
    LoadTilePoolGraphic(0x9D);
    LoadTilePoolGraphic(0x9E);
    LoadTilePoolGraphic(0x9F);
    LoadTilePoolGraphic(0xA0);
    LoadTilePoolGraphic(0xA1);
    LoadTilePoolGraphic(0xA2);
    LoadTilePoolGraphic(0xA3);
    LoadTilePoolGraphic(0xA4);
    LoadTilePoolGraphic(0xA5);
    LoadTilePoolGraphic(0xA6);
    LoadTilePoolGraphic(0xA7);
    LoadTilePoolGraphic(0x93);
    LoadTilePoolGraphic(0x94);
    LoadTilePoolGraphic(0x43);
    LoadTilePoolGraphic(0x44);
    LoadTilePoolGraphic(0x50);
    LoadTilePoolGraphic(0x95);
    LoadTilePoolGraphic(0x96);
    LoadTilePoolGraphic(0x97);
    LoadTilePoolGraphic(0x98);
    LoadTilePoolGraphic(0x99);
    LoadTilePoolGraphic(0x9A);
    LoadTilePoolGraphic(0x67);
    LoadTilePoolGraphic(0x92);

    Proc_Start(ProcScr_CoInfo, savedParent);
    AddVBlankHook((void *)AnimateCoPowerStatusPalette);
}

asm(".global sub_080849C8\n.thumb_set sub_080849C8, CoInfoScreen_LoadGraphics\n");
