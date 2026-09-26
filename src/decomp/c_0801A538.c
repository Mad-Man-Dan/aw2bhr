#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A538.
 * sub_0801A538 @ 0x0801A538
 */

/*
 * sub_0801A538 -- run sub_080199F8, then sub_08024584.
 *
 * The four parameters are never used. They are declared because the callers set
 * all four argument registers immediately before the call, and sub_08019DA8
 * (src/decomp/c_08019D0C.c) sets them to four different non-zero constants,
 * which nothing but four arguments explains. They cost nothing: the first call
 * overwrites the first argument register anyway. `int` is the weakest type that
 * fits the constants seen.
 *
 * Two statements and not `sub_08024584(sub_080199F8())`: the second callee takes
 * no arguments, so there is nothing for the first call's result to reach. See
 * the F005 block in include/unknown-functions.h for the other eighteen wrappers
 * of this shape.
 */

void sub_0801A538(int a, int b, int c, int d)
{
    sub_080199F8();
    sub_08024584();
}
