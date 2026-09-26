#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C0CC.
 * sub_0802C0CC @ 0x0802C0CC, sub_0802C0D8 @ 0x0802C0D8
 */

/* A selector wrapper: sub_0802BFD0 takes an `int` (bare `adds r6, r0, #0`
 * across four calls) and this pins it to 0. `pop {r0}; bx r0`, so void.
 */

void sub_0802C0CC(void)
{
    sub_0802BFD0(0);
}

/* A selector wrapper: sub_0802BFD0 takes an `int` (bare `adds r6, r0, #0`
 * across four calls) and this pins it to 1. `pop {r0}; bx r0`, so void.
 */

void sub_0802C0D8(void)
{
    sub_0802BFD0(1);
}
