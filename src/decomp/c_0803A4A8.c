#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A4A8.
 * sub_0803A4A8 @ 0x0803A4A8
 */

void sub_0803A4A8(void)
{
    TmApplyTsaClipped(gBG2TilemapBuffer, gUnknown_0849D89C->unk00 >> 3, 0, gUnknown_080D4228, 0x8360);
    sub_08013AD4(2);
    UnitInfoPanel_LoadPictureDrawMoveAndVision(gUnknown_0849D89C->unk00, gUnknown_0849D89C->unk04);
    sub_08013AD4(0);
    SetMapLayerPrioritiesDefault();
    StartTextBox((gUnknown_0849D89C->unk00 >> 3) + 1, 0xa, gBG0TilemapBuffer,
                 gUnknown_0849E398[gUnknown_081BA068[gUnknown_0849D89C->unk04->type] - 1][0],
                 0x8000, 0xf8)->unk3a = 1;
}
