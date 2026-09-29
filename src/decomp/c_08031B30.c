#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031B30.
 * sub_08031B30 @ 0x08031B30
 */

/* Every access here is a volatile member of struct Unk0849B01C, which is what
 * emits the dead `ldrh` of the very address being stored to, twice.
 *
 * Wave 30, W30-A -- the BRANCH POLARITY is the whole difference and it is not
 * free to choose. `if (unk212 == 0) return 1; <copy>; return 0;` and the same
 * thing with an explicit `else` BOTH compile to `beq` with the copy inline and
 * the `movs r0,#1` out of line, because do_jump rewrites `x == 0` by swapping
 * its two labels. Only the `!= 0` spelling below leaves the compare
 * un-swapped, and it is also what splits the literal pool in two the way the
 * ROM does: three words before the copy block, two after `bx lr`. */

int LinkWaitReadyBarrier(void)
{
    gUnknown_0849B01C->unk06 = 0x9abc;

    if (gUnknown_0849B01C->unk212 != 0)
    {
        gUnknown_0300410C = gUnknown_030040CC;
        gUnknown_0849B01C->unk212 = 0;

        return 0;
    }

    return 1;
}
asm(".global sub_08031B30\n.thumb_set sub_08031B30, LinkWaitReadyBarrier\n");
