#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018DC0.
 * sub_08018DC0 @ 0x08018DC0
 */

bool8 EventOp_MoveCursorCellBy(s16 a)
{
    gUnknown_030033E4.unk00 += gUnknown_0200C528[a].unk04->unk08;
    gUnknown_030033E4.unk02 += gUnknown_0200C528[a].unk04->unk0a;
    gUnknown_0200C528[a].unk04++;
    return FALSE;
}
asm(".global sub_08018DC0\n.thumb_set sub_08018DC0, EventOp_MoveCursorCellBy\n");
