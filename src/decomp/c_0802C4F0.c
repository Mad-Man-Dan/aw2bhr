#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C4F0.
 * sub_0802C4F0 @ 0x0802C4F0
 */

#include "proc.h"

void sub_0802C4F0(ProcPtr proc)
{
    struct Unk03003338 *p;

    if (FindSlotScript((s32)gUnknown_0849A00C) != -1)
        return;

    if (gUnknown_03003F40 < 0)
    {
        DecrementMapLock();
        Proc_End(proc);
        sub_0802D558();
    }
    else
    {
        p = GetAttackTargetRecord(gUnknown_03003F40);

        if (p->unk02 == 0)
            sub_080425E0(p->unk00);
        else
            sub_08042618(p->unk04, p->unk06);

        Proc_Break(proc);
    }
}
