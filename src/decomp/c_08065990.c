#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065990.
 * sub_08065990 @ 0x08065990
 */

#include "hardware.h"

/* Screen setup, and the near-twin of RulesScreen_Init for its first two thirds.
 *
 * The eleven BlendCnt field writes come out as exactly two `ldrb`/`strb`
 * pairs, one per container byte, because agbcc forwards each store into the
 * next statement's load and then drops the dead stores -- so a run of bitfield
 * assignments to one byte is ONE read-modify-write and the chain of masks
 * (`movs #5; rsbs` then `subs #4` then `subs #8` for ~4, ~8, ~0x10) is the CSE
 * of the constants, not something in the source. Read it back field by field
 * in the order the masks apply.
 *
 * `Decompress(sub_0801F49C(), ...)` is a real nesting and not two statements:
 * nothing writes r0 between the two `bl`s, so the second call's first argument
 * can only be the first call's result.
 */

void MatchSetupScreen_Init(void)
{
    sub_080366D0(DefaultVBlankCallback);
    sub_080366C4(DefaultMainLoopCallback);
    SetupBackgrounds(gUnknown_0849D16C);
    ResetWindowShadows();

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target1_enable_bg0 = 0;
    gUnknown_030030E0.bits.target1_enable_bg1 = 1;
    gUnknown_030030E0.bits.target1_enable_bg2 = 0;
    gUnknown_030030E0.bits.target1_enable_bg3 = 0;
    gUnknown_030030E0.bits.target1_enable_obj = 0;
    gUnknown_030030E0.bits.target2_enable_bg0 = 1;
    gUnknown_030030E0.bits.target2_enable_bg1 = 0;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;
    gUnknown_03002020 = 0xf;
    gUnknown_03002B28 = 6;

    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();

    gUnknown_03001FF8 = 0;
    gUnknown_03001418 = 0;
    gUnknown_03002B34 = 0;
    gUnknown_03002F18 = 0;
    gUnknown_030030A0 = 0;
    gUnknown_03001400 = 0;
    gUnknown_0300200C = 0;
    gUnknown_03002000 = 0;

    Decompress(gUnknown_0822FEF0, (void *)((gUnknown_0300251C.bits.chr_block << 14) + 0x06000000));
    Decompress(gUnknown_0822F9AC, gBG3TilemapBuffer);
    ApplyPaletteExt(gUnknown_082344CC, 0x20, 0xa0);
    BG_EnableSyncBG3();
    LoadWindowFrameGraphics((void *)((gUnknown_030030B4.bits.chr_block << 14) + 0x06006C00), 0, 8);
    DrawWindowBackgroundOnBg2(0, 0x10, 0x1e, 4);
    ApplyWindowFramePalette(0, 3);
    Decompress(sub_0801F49C(), (void *)0x06015200);
    MatchSetupInitState();
    sub_080152EC(gUnknown_08580CB4, 3);
    sub_0801F114();

    InitTilePool(1, (void *)0x06010000, 0x298, 0x19);
    LoadTilePoolGraphic(0x3e);
    LoadTilePoolGraphic(0x3f);
    LoadTilePoolGraphic(0x40);
    LoadTilePoolGraphic(0x41);

    InitTilePool(2, (void *)0x06010000, 0x2a8, 0x1a);
    LoadTilePoolGraphic(0x4a);
    LoadTilePoolGraphic(0x4b);
    LoadTilePoolGraphic(0x4c);
    LoadTilePoolGraphic(0x4d);
    LoadTilePoolGraphic(0x4e);
    LoadTilePoolGraphic(0x43);
    LoadTilePoolGraphic(0x44);
    LoadTilePoolGraphic(0x55);
    LoadTilePoolGraphic(0x56);
    LoadTilePoolGraphic(0x57);
    LoadTilePoolGraphic(0x58);
    LoadTilePoolGraphic(0x59);
    LoadTilePoolGraphic(0x5a);
    LoadTilePoolGraphic(0x5b);
    LoadTilePoolGraphic(0x5c);
    LoadTilePoolGraphic(0x5d);
    LoadTilePoolGraphic(0x5e);
    LoadTilePoolGraphic(0x68);

    InitTilePool(5, (void *)0x06010000, 0x2d2, 0x1b);
    LoadTilePoolGraphic(0xbc);
    LoadTilePoolGraphic(0xbd);
    LoadTilePoolGraphic(0xbe);
    LoadTilePoolGraphic(0xbf);
    LoadTilePoolGraphic(0xc0);
    LoadTilePoolGraphic(0xc1);
    LoadTilePoolGraphic(0xc2);
    LoadTilePoolGraphic(0xc3);
    LoadTilePoolGraphic(0xc4);
    LoadTilePoolGraphic(0xc5);
    LoadTilePoolGraphic(0xc6);
    LoadTilePoolGraphic(0xc7);
    LoadTilePoolGraphic(0xc8);
    LoadTilePoolGraphic(0xc9);
    LoadTilePoolGraphic(0xca);
    LoadTilePoolGraphic(0xcb);
    LoadTilePoolGraphic(0xcc);
    LoadTilePoolGraphic(0xcd);
    LoadTilePoolGraphic(0xce);
    LoadTilePoolGraphic(0xcf);
    LoadTilePoolGraphic(0xd0);
    LoadTilePoolGraphic(0xd1);
    LoadTilePoolGraphic(0xd2);
    LoadTilePoolGraphic(0xd3);
    LoadTilePoolGraphic(0xd4);
    LoadTilePoolGraphic(0xd5);
    LoadTilePoolGraphic(0xd6);
    LoadTilePoolGraphic(0xd7);

    sub_080152EC(gUnknown_08580CC4, 3);
    gUnknown_08580934->unk30 = 0;
    gpKeySt->pressed = L_BUTTON;
    MatchSetupSpawnArmyColumns();
    gUnknown_08580934->unk30 = 1;
    gpKeySt->pressed = 0;
}
asm(".global sub_08065990\n.thumb_set sub_08065990, MatchSetupScreen_Init\n");
