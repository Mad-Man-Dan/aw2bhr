#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801220C.
 * sub_0801220C @ 0x0801220C
 */

/* Resets the text/dialogue cursor state: seed the pair through SetKeyRepeatTiming,
 * snapshot gUnknown_03000044 into its neighbour, then clear four more
 * halfwords. Every store is `strh` through its own pool word, so these are six
 * separate scalars and not one aggregate -- the same reading the
 * gUnknown_03000044 / gUnknown_03000046 note already carries. */
void sub_0801220C(void)
{
    SetKeyRepeatTiming(0x14, 6);

    gUnknown_03000042 = gUnknown_03000044;
    gUnknown_03000040 = 0;
    gUnknown_03002F04 = 0;
    gUnknown_030024C8 = 0;
    gUnknown_030030B0 = 0;
}
