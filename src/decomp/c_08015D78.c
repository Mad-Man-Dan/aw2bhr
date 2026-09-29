#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015D78.
 * sub_08015D78 @ 0x08015D78, sub_08015DA0 @ 0x08015DA0
 */

bool8 SlotOp_SetUpdateCallback(u8 a)
{
    gUnknown_03001470[a].unk08 = *(const u32 *)gUnknown_03001470[a].unk04;
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08015D78\n.thumb_set sub_08015D78, SlotOp_SetUpdateCallback\n");

bool8 SlotOp_Wait(u8 a)
{
    gUnknown_03001470[a].unk10 = ((const u16 *)gUnknown_03001470[a].unk04)[2];
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08015DA0\n.thumb_set sub_08015DA0, SlotOp_Wait\n");
