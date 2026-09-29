#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080176A8.
 * sub_080176A8 @ 0x080176A8
 */

/* A saturating bump. One `ldrh` serves both the test and the increment, so the
 * global is a plain (non-volatile) u16 -- a volatile one would keep two loads.
 */
void IncrementDayCount(void)
{
    if (gUnknown_03004080 < 0x3e7)
        gUnknown_03004080++;
}
asm(".global sub_080176A8\n.thumb_set sub_080176A8, IncrementDayCount\n");
