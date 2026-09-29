#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AC20.
 * sub_0808AC20 @ 0x0808AC20
 */

void FlashTimerIntr(void)
{
    int v;

    if (gUnknown_03000F72 != 0)
    {
        /* `v` must be `int`, and the narrowing must sit on the COMPARISON.
         * A `u16 v` narrows at the assignment instead (`lsl #16; lsr #16`
         * before the `strh`), and `--gUnknown_03000F72 == 0` re-reads the
         * volatile for the test -- a third `ldrh` the ROM does not have. */
        v = gUnknown_03000F72 - 1;
        gUnknown_03000F72 = v;

        if ((u16)v == 0)
            gUnknown_03000F74 = 1;
    }
}
asm(".global sub_0808AC20\n.thumb_set sub_0808AC20, FlashTimerIntr\n");
