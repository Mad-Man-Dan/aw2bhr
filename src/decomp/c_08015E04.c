#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015E04.
 * sub_08015E04 @ 0x08015E04, sub_08015E2C @ 0x08015E2C
 */

bool8 SlotOp_Jump(u8 a)
{
    gUnknown_03001470[a].unk04 = *(const void *const *)gUnknown_03001470[a].unk04;
    gUnknown_03001470[a].unk10 = 0;
    gUnknown_03001470[a].unk38 = 0;
    return TRUE;
}
asm(".global sub_08015E04\n.thumb_set sub_08015E04, SlotOp_Jump\n");

bool8 sub_08015E2C(u8 a)
{
    gUnknown_03001470[a].unk04 = *(const void *const *)gUnknown_03001470[a].unk04;
    gUnknown_03001470[a].unk10 = 0;
    gUnknown_03001470[a].unk38 = 0;
    return TRUE;
}
