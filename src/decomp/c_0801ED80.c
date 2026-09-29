#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801ED80.
 * sub_0801ED80 @ 0x0801ED80, sub_0801EDC0 @ 0x0801EDC0, sub_0801EDF8 @ 0x0801EDF8
 */

/* Repacks its arguments for sub_0801ECE8 and reduces the result to 0 or -1.
 *
 * THE PROLOGUE IS THE INTERESTING PART. `sub sp, #4` before the push, followed
 * by `str r3, [sp,#0x20]` and an immediate reload, is not a spill and not
 * varargs -- agbcc's varargs prologue is `push {r2, r3}`, which was measured
 * and does not appear here. It is a 64-bit parameter straddling the
 * register/stack boundary: the low word arrives in r3, the high word in the
 * caller's first stack slot, and gcc reserves a home slot so the two are
 * contiguous. agbcc annotates the reload `created by
 * thumb_load_double_from_address`. That also accounts for the THIRD stack read
 * at 0x28, which six `int` parameters cannot produce.
 *
 * The 0/-1 tail is written as an if/else and not as `-(x != 0)`: the ROM
 * presets one value and branches over the other, which is the spelling a
 * returned comparison cannot produce. -1 is CSEd with the seventh argument, so
 * both must be written as the same literal. */
int sub_0801ED80(int a, int b, int c, long long d, s16 e)
{
    if (sub_0801ECE8(e, a, b, c, d, -1) != 0)
        return -1;
    else
        return 0;
}

/* Queues a deferred callback, tagging the pointer with bit 31. See the note on
 * RunOrQueueDrawCallback in include/unknown-functions.h, which names this function's two
 * parameters from its own call site.
 *
 * The 64-bit zero is bound to a local so that it is materialised BEFORE the
 * s16 narrowing consumes r1. In the ROM it lands in the callee-saved pair
 * r4/r5, which only happens while r0, r1 and r2 are all still occupied by the
 * two incoming parameters and the copy of `fn`. Folded into the argument list
 * it is created after r1 is free and takes r1/r2 instead. */
int QueueSpriteCallback(void *fn, s16 arg)
{
    long long zero = 0;

    if (sub_0801ECE8(arg, 0, 0, (int)((u32)fn | 0x80000000), zero, -1) != 0)
        return -1;
    else
        return 0;
}
asm(".global sub_0801EDC0\n.thumb_set sub_0801EDC0, QueueSpriteCallback\n");

/* Passes a 64-bit zero through to sub_0801ED80 with its own fourth parameter
 * as the trailing s16. ONE `movs r3, #0` covers both halves of that zero --
 * the low word stays in r3 as the register argument and the same register is
 * stored for the high word -- which is why this one is 4 bytes SHORTER than
 * the two-`int` spelling rather than longer. The 64-bit reading is what makes
 * the sharing legal; two separate `int` zeros give two registers and a wider
 * push. */
int sub_0801EDF8(int a, int b, int c, s16 d)
{
    return sub_0801ED80(a, b, c, 0, d);
}
