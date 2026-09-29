#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B3E0.
 * sub_0804B3E0 @ 0x0804B3E0
 */

/* One side's fade-out step: while [0] is armed it walks [1] up through
 * gUnknown_085644C8 and writes the entry into the side's second scroll origin,
 * disarming both halves once the counter passes 4.
 *
 * THE ROW IS A BOUND POINTER. `gUnknown_02028E5C` is declared `u16 [][2]`, and
 * naming `gUnknown_02028E5C[a][0]` / `[a][1]` builds TWO bases (`adds r0, r2,
 * #2` for the second), where the ROM keeps one base and a `#2` displacement.
 * The row pointer is what makes those one address.
 *
 * `v = 0` is hoisted above the test rather than living in the else arm: the
 * else stores that same register into both halves of the row, so the zero is
 * one value used three times. */
void StepPanelBounce(u16 a)
{
    u16 v;
    u16 *row;

    row = gUnknown_02028E5C[a];

    if (row[0] == 1)
    {
        v = 0;
        if (row[1] <= 4)
        {
            row[1]++;
            v = gUnknown_085644C8[row[1]];
        }
        else
        {
            row[1] = 0;
            row[0] = 0;
        }
        *gUnknown_084C3F78[a] = v;
    }
}
asm(".global sub_0804B3E0\n.thumb_set sub_0804B3E0, StepPanelBounce\n");
