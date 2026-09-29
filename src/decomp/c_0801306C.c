#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801306C.
 * sub_0801306C @ 0x0801306C
 */

/* An on-screen test: the bounds are 0xf0 and 0xa0, i.e. 240x160, widened by
 * half the object's size in both directions.
 *
 * Written as one `||` of four failures rather than four `if (...) return 0;`
 * statements. Both are the same predicate, but the separate-statement form
 * makes agbcc recompute `h + 0xa0` from scratch (`adds r0, r2, #0; adds r0,
 * #0xa0`), where the ROM derives it from the live `h + 0xf0` with
 * `subs r0, #0x50`. The `||` chain keeps that value in one basic block, which
 * is what lets the CSE happen. Measured side by side.
 *
 * The comparisons are spelled with the bound on the LEFT (`h + 0xf0 < x`), not
 * as `x > h + 0xf0` -- the ROM's `cmp r0, r4` has the operands that way round,
 * and swapping them swaps the instruction. `asrs` on the size makes it
 * signed. */
int IsSpriteOnScreen(int x, int y, int size)
{
    int h = size >> 1;

    if (x < -h || h + 0xf0 < x || y < -h || h + 0xa0 < y)
        return 0;
    return 1;
}
asm(".global sub_0801306C\n.thumb_set sub_0801306C, IsSpriteOnScreen\n");
