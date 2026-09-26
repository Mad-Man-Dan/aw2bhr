#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045F24.
 * sub_08045F24 @ 0x08045F24, sub_08045F40 @ 0x08045F40
 */

#include "proc.h"

void sub_08045F24(void)
{
    sub_0803B4DC(0x1c7);
    sub_080130DC(0x14, 0x64, 0x3c, 0);
}

void sub_08045F40(void)
{
    sub_0803B4DC(0x1e1);
    Proc_Start(gUnknown_084B7628, PROC_TREE_3);
}
