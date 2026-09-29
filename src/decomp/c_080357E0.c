#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080357E0.
 * sub_080357E0 @ 0x080357E0, sub_08035810 @ 0x08035810
 */

#include "proc.h"

int CreateMoveSlideWithPath(u16 a, u16 b, u16 c, u16 d, void *e)
{
    ProcPtr proc;

    proc = CreateMoveSlide(a, b, c, d);
    if (proc == NULL)
        return 0;
    else
    {
        BeginMoveSlidePath(proc, e);
        return (int)proc;
    }
}
asm(".global sub_080357E0\n.thumb_set sub_080357E0, CreateMoveSlideWithPath\n");

void EndActiveMoveSlide(void)
{
    ProcPtr proc;

    proc = Proc_Find(ProcScr_SelectUnit);
    if (proc != NULL)
        EndMoveSlide(proc);
}
asm(".global sub_08035810\n.thumb_set sub_08035810, EndActiveMoveSlide\n");
