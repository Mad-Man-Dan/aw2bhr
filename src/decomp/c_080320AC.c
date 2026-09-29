#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080320AC.
 * sub_080320AC @ 0x080320AC, sub_080320CC @ 0x080320CC
 */

void sub_080320AC(void)
{
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 11, 2);
}

/* The `movs r2, #0` hoisted ahead of the first store is agbcc scheduling the
 * halfword store's constant into a register that survives the pool `ldr` in
 * between; it is not a fourth store. */

void LinkSendLeavePacket(void)
{
    gUnknown_0202575C.unk00 = 0xa8;
    gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
    gUnknown_0202575C.unk02 = 0;

    sub_0802F588(&gUnknown_0202575C, 4);
}
asm(".global sub_080320CC\n.thumb_set sub_080320CC, LinkSendLeavePacket\n");
