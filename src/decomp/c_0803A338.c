#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A338.
 * sub_0803A338 @ 0x0803A338
 */

void UnitInfoPanel_Open(void)
{
    TmApplyTsaClipped(gBG2TilemapBuffer, gUnknown_0849D89C->unk00 >> 3, 0, gUnknown_080D4228, 0x8360);
    sub_0801F114();
    InitTilePool(0, (void *)0x06010000, 0x296, 0x15);
    LoadTilePoolGraphic(6);
    LoadTilePoolGraphic(0x23);
    LoadTilePoolGraphic(0x24);
    LoadTilePoolGraphic(0x25);
    LoadTilePoolGraphic(0x26);
    LoadTilePoolGraphic(0x27);
    LoadTilePoolGraphic(0x28);
    LoadTilePoolGraphic(0x29);
    LoadTilePoolGraphic(0x2a);
    LoadTilePoolGraphic(0x3b);
    InitTilePool(3, (void *)0x06010000, 0x1ca, 0x16);
    LoadTilePoolGraphic(0xac);
    LoadTilePoolGraphic(0xad);
    LoadTilePoolGraphic(0xae);
    LoadTilePoolGraphic(0xaf);
    LoadTilePoolGraphic(0xb0);
    LoadTilePoolGraphic(0xb1);
    LoadTilePoolGraphic(0xb2);
    LoadTilePoolGraphic(0xb3);
    LoadTilePoolGraphic(0xb4);
    LoadTilePoolGraphic(0xb5);
    LoadTilePoolGraphic(0xb6);
    LoadTilePoolGraphic(0xb7);
    UnitInfoPanel_LoadPictureDrawMoveAndVision(gUnknown_0849D89C->unk00, gUnknown_0849D89C->unk04);
    sub_0803A2BC(gUnknown_0849D89C->unk00, gUnknown_0849D89C->unk04);
}
asm(".global sub_0803A338\n.thumb_set sub_0803A338, UnitInfoPanel_Open\n");
