#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D0B4.
 * sub_0802D0B4 @ 0x0802D0B4
 */

/* See src/decomp/c_0802D064.c: same shape, sub_08042864 instead of
 * sub_08042650 and command id 7. */

void sub_0802D0B4(void)
{
    LockUnitSelection();
    CloseTopMenu();
    sub_0802C57C();
    sub_08042864();
    sub_080424FC();
    sub_0802C594();

    if (gPlaySt.savingEnabled != 0)
        sub_08034534(7, gUnknown_03003F38, 0, 0);
}
