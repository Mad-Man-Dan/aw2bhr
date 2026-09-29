#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075EC4.
 * sub_08075EC4 @ 0x08075EC4
 */

/* MATCHED (wave 38, W38-E). Was parked at 83.0% across waves 35 and 36, both of
 * which read the residual as a register-allocation accident and tried local
 * DECLARATION ORDER against it. It was not an allocation problem at all; it was
 * two source facts, and each one is independently reusable.
 *
 * 1. THE SUM IS A NAMED LOCAL, reused for both tests. The ROM lands both sums in
 *    r2 (`adds r2, r0, r1` twice); two anonymous temporaries land in whatever
 *    register the compare wants and drag `a` out of r3 into r2, which is what
 *    every earlier draft was seeing as "the allocno numbering is wrong". Adding
 *    `c` took this from 83.0% to 89.8% and made the whole head byte-exact --
 *    including `movs r2, #6`, the ldrsh offset scratch, which is only free
 *    because `a` is NOT in r2. The two facts the old notes filed separately
 *    ("`a` in r2 instead of r3" and "the `movs r2` scratch") are ONE fact.
 *
 * 2. THE SECOND TEST NEEDS AN EXPLICIT `else` AND THE OPPOSITE SENSE. The ROM
 *    falls through to `return 0` and branches to `return -1`. Both
 *    `if (c > 0x120) return 0; return -1;` and the same with an explicit `else`
 *    compile to the MIRROR of that, byte-for-byte identical to each other --
 *    so the `else` is not the lever on its own. Only
 *    `if (c <= 0x120) return -1; else return 0;` lays the arms out the ROM's
 *    way. Read the arm ORDER off the ROM (whichever arm sits immediately before
 *    the shared `bx lr` is the one the source names second) rather than reading
 *    the branch condition, which is inverted by the layout.
 *
 * The first test keeps the ordinary convention (`if (c <= 0xE0) return 1;`
 * falls through to the true arm), so the two `if`s in this one function use
 * OPPOSITE spellings for the same shape. That is not a contradiction: the first
 * has no else and is followed by more code, the second terminates the function.
 *
 * The mid-function literal pool at +0x34 falls out of the first `b`; it was
 * never an independent fact. A structurally different attempt that nested the
 * second test inside the first scored 45.5% and moved the pool to the end,
 * which is how the fall-through shape was confirmed.
 *
 * What it computes: an 8-pixel-grid distance test on gUnknown_0202FDFC's
 * +0x04/+0x06 cursor offset. `/ 8` really is a signed divide (the
 * `cmp #0; bge; adds #7` bias is in the ROM), and `b` is measured from y = 0x14.
 */
int GetWorldMapNationPanelSide(void)
{
    int a;
    int b;
    int c;

    a = gUnknown_0202FDFC.unk04 / 8;
    b = 0x14 - gUnknown_0202FDFC.unk06 / 8;

    c = a * a + b * b;
    if (c <= 0xE0)
        return 1;

    c = (0x1E - a) * (0x1E - a) + b * b;
    if (c <= 0x120)
        return -1;
    else
        return 0;
}
asm(".global sub_08075EC4\n.thumb_set sub_08075EC4, GetWorldMapNationPanelSide\n");
