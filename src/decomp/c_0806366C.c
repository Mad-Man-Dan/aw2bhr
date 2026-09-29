#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806366C.
 * sub_0806366C @ 0x0806366C
 */

/* The mask is computed BEFORE the guard in the ROM, which is what a local
 * bound outside the `if` looks like -- written inside the branch, agbcc sinks
 * it past the `cmp`. `movs r1, #0x80; lsls r1, #1` is the constant 0x100. */
void sub_0806366C(int a1, int a2)
{
    int t = a2 & 0x3FF;

    if (a1 < 0)
        RegisterFillZero16((void *)(t * 0x20 + 0x06010000), 0x100);
}
