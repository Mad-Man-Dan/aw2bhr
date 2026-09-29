#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060424.
 * sub_08060424 @ 0x08060424, sub_0806044C @ 0x0806044C, sub_08060474 @ 0x08060474
 */

#include "proc.h"

void sub_08060424(void)
{
    sub_080425FC(gUnknown_030046C0.unk06);
    gUnknown_03004780 = 2;
    gUnknown_030045D4 = 0;
}

void sub_0806044C(void)
{
    sub_08042634(gUnknown_030046C0.unk06, gUnknown_030046C0.unk07);
    gUnknown_03004780 = 2;
    gUnknown_030045D4 = 0;
}

/* sub_08042C24's fifth parameter is a `ProcPtr` parent, not an int: its
 * promoted definition branches on `(int)parent <= 7` to pick Proc_Start over
 * Proc_StartBlocking, so the small values are tree numbers. The literal 3 here
 * is PROC_TREE_3, and `movs r4, #3` is the same instruction either way. */
void sub_08060474(void)
{
    CommitUnitMove();
    sub_08042C24(gUnknown_030046C0.unk02, gUnknown_030046C0.unk03,
                 gUnknown_030046C0.unk06, gUnknown_030046C0.unk07, PROC_TREE_3);
    gUnknown_030045D4 = 6;
}
