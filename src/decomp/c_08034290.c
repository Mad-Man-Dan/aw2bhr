#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034290.
 * sub_08034290 @ 0x08034290, sub_080342BC @ 0x080342BC
 */

#include "proc.h"

void sub_08034290(void)
{
    Proc_EndEach(gUnknown_0849BB80);
    Proc_EndEach(ProcScr_PutFace);
    Proc_EndEach(gUnknown_0849BB68);
    SetVCountInterruptHandler(0);
}

/* sub_08034290 without the leading gUnknown_0849BB80 teardown. */
void sub_080342BC(void)
{
    Proc_EndEach(ProcScr_PutFace);
    Proc_EndEach(gUnknown_0849BB68);
    SetVCountInterruptHandler(0);
}
