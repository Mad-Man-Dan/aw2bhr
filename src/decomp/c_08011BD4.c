#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011BD4.
 * sub_08011BD4 @ 0x08011BD4
 */

/* Sums the unk08 halfword of every live gUnknown_0200B3B4 queue entry, walking
 * the queue BACKWARDS from gUnknown_03002F30 - 1. The count global is
 * `volatile u16` (c_08011BC4.c returns it as s16), which is why it is loaded
 * twice: once for the `== 0` early return and once for the loop init. Those
 * are two separate source constructs, not one -- the early return exits with
 * `movs r0,#0` while the zero-trip loop falls through the `(s16)sum` tail.
 *
 * `sum` is u16 (the `lsls #0x10; lsrs #0x10` at every accumulate) and the
 * return re-signs it, so the return type is s16.
 *
 * THE ACCUMULATE MUST GO THROUGH A LOCAL. `sum += gUnknown_0200B3B4[i].unk08;`
 * emits `adds r1, r2, r1` where the ROM has `adds r1, r1, r2` -- the two
 * operands of the commutative add swap places, a different encoding and the
 * function's only differing bytes. Because unk08 is `volatile u16`, fold may
 * not reorder the read past the accumulator, so the operand order is fixed by
 * the source and gcc canonicalises the direct form the other way. Reading it
 * into a plain local first (or, equivalently, an `int` accumulator with an
 * explicit `(u16)` cast) restores the ROM's order; changing the order of the
 * `+` operands in the source does NOT -- that was measured both ways. */
s16 GetCopyQueuePendingSize(void)
{
    u16 sum;
    u16 x;
    s16 i;

    sum = 0;

    if (gUnknown_03002F30 == 0)
        return 0;

    for (i = gUnknown_03002F30 - 1; i >= 0; i--)
    {
        x = gUnknown_0200B3B4[i].unk08;
        sum += x;
    }

    return sum;
}
asm(".global sub_08011BD4\n.thumb_set sub_08011BD4, GetCopyQueuePendingSize\n");
