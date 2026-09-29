#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F0AC.
 * sub_0801F0AC @ 0x0801F0AC, sub_0801F0C8 @ 0x0801F0C8, sub_0801F0E0 @ 0x0801F0E0, sub_0801F0FC @ 0x0801F0FC
 */

/* Family F070, second member -- see sub_0801F050. */

void sub_0801F0AC(void)
{
    if (gUnknown_03001FE0)
        SyncLoOam();
    else
        sub_0801EFF4();
}

/* Family F071, second member -- see sub_0801F050. Its callee sub_0801EFA8 is
 * the F069 member matched in the same batch. */

void sub_0801F0C8(void)
{
    if (!gUnknown_03001FE0)
        sub_0801EFA8();
}

/* Family F070, third member -- see sub_0801F050. */

void sub_0801F0E0(void)
{
    if (gUnknown_03001FE0)
        SyncHiOam();
    else
        sub_0801EFD8();
}

/* Family F071, third member -- see sub_0801F050. */

void sub_0801F0FC(void)
{
    if (!gUnknown_03001FE0)
        sub_0801555C();
}
