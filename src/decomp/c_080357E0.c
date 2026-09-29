#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080357E0.
 * sub_080357E0 @ 0x080357E0, sub_08035810 @ 0x08035810
 */

#include "proc.h"

int sub_080357E0(u16 a, u16 b, u16 c, u16 d, void *e)
{
    ProcPtr proc;

    proc = sub_080355CC(a, b, c, d);
    if (proc == NULL)
        return 0;
    else
    {
        BeginMoveSlidePath(proc, e);
        return (int)proc;
    }
}

void sub_08035810(void)
{
    ProcPtr proc;

    proc = Proc_Find(ProcScr_SelectUnit);
    if (proc != NULL)
        EndMoveSlide(proc);
}
