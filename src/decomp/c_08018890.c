#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018890.
 * sub_08018890 @ 0x08018890
 */

bool8 EventOp_SetFramePaletteSlot8(s16 a)
{
    gUnknown_03002F08.unk00 = 8;
    LoadArmyObjPalette(gUnknown_030033EC);
    gUnknown_0200C528[a].unk04++;
    return TRUE;
}
asm(".global sub_08018890\n.thumb_set sub_08018890, EventOp_SetFramePaletteSlot8\n");
