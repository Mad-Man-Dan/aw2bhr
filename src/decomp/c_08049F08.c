#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049F08.
 * sub_08049F08 @ 0x08049F08
 */

#include "proc.h"

/* Two parameters: r1 is never written before the `bl`, so the parent handed to
 * Proc_StartBlocking is this function's own second argument passed straight
 * through. */

void StartDefeatFlowProc(int a, ProcPtr parent)
{
    gUnknown_084C3240->unk2e = a;
    Proc_StartBlocking(gUnknown_084C327C, parent);
}
asm(".global sub_08049F08\n.thumb_set sub_08049F08, StartDefeatFlowProc\n");
