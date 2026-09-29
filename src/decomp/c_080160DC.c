#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080160DC.
 * sub_080160DC @ 0x080160DC
 */

bool8 SlotOp_SetFlag(u8 a)
{
    gUnknown_03002F1C = 1;
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return TRUE;
}
asm(".global sub_080160DC\n.thumb_set sub_080160DC, SlotOp_SetFlag\n");
