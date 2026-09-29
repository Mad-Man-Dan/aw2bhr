#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BC5C.
 * sub_0802BC5C @ 0x0802BC5C
 */

/* The index needs its own binding local. Written `p[gUnknown_030033EC - 1]`
 * the -1 folds into the address as a trailing `subs r0, r0, #2` after the
 * scaling, where the ROM subtracts 1 from the index first. */
void DrawDayAndFundsBarForCurrentArmy(void)
{
    const s16 *p = gUnknown_08090A98;
    int i = gUnknown_030033EC - 1;

    DrawDayAndFundsBar(p[i]);
}
asm(".global sub_0802BC5C\n.thumb_set sub_0802BC5C, DrawDayAndFundsBarForCurrentArmy\n");
