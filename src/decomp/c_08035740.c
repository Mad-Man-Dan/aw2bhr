#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035740.
 * sub_08035740 @ 0x08035740
 */

#include "proc.h"

void sub_08035740(void *a)
{
    ProcPtr proc;

    proc = Proc_Find(ProcScr_SelectUnit);
    if (proc != NULL)
        BeginMoveSlidePath(proc, a);
}
