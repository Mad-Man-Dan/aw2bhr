#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018D90.
 * sub_08018D90 @ 0x08018D90
 */

bool8 EventOp_SetCursorCell(s16 a)
{
    gUnknown_030033E4.unk00 = gUnknown_0200C528[a].unk04->unk08;
    gUnknown_030033E4.unk02 = gUnknown_0200C528[a].unk04->unk0a;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018D90\n.thumb_set sub_08018D90, EventOp_SetCursorCell\n");
