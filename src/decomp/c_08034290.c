#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034290.
 * sub_08034290 @ 0x08034290, sub_080342BC @ 0x080342BC
 */

#include "proc.h"

void LinkC3_EndScreenProcs(void)
{
    Proc_EndEach(gUnknown_0849BB80);
    Proc_EndEach(ProcScr_PutFace);
    Proc_EndEach(gUnknown_0849BB68);
    SetVCountInterruptHandler(0);
}
asm(".global sub_08034290\n.thumb_set sub_08034290, LinkC3_EndScreenProcs\n");

/* LinkC3_EndScreenProcs without the leading gUnknown_0849BB80 teardown. */
void sub_080342BC(void)
{
    Proc_EndEach(ProcScr_PutFace);
    Proc_EndEach(gUnknown_0849BB68);
    SetVCountInterruptHandler(0);
}
