#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A538.
 * sub_0801A538 @ 0x0801A538
 */

/*
 * DisableWindow0AndResetMapLayers -- run DisableWindow0, then SetMapLayersDefault.
 *
 * The four parameters are never used. They are declared because the callers set
 * all four argument registers immediately before the call, and Menu_OnEndReleaseMapLock
 * (src/decomp/c_08019D0C.c) sets them to four different non-zero constants,
 * which nothing but four arguments explains. They cost nothing: the first call
 * overwrites the first argument register anyway. `int` is the weakest type that
 * fits the constants seen.
 *
 * Two statements and not `SetMapLayersDefault(DisableWindow0())`: the second callee takes
 * no arguments, so there is nothing for the first call's result to reach. See
 * the F005 block in include/unknown-functions.h for the other eighteen wrappers
 * of this shape.
 */

void DisableWindow0AndResetMapLayers(int a, int b, int c, int d)
{
    DisableWindow0();
    SetMapLayersDefault();
}
asm(".global sub_0801A538\n.thumb_set sub_0801A538, DisableWindow0AndResetMapLayers\n");
