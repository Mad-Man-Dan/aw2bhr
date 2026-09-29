#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C4F0.
 * sub_0802C4F0 @ 0x0802C4F0
 */

#include "proc.h"

void AttackTargetSelect_Resolve(ProcPtr proc)
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
            StartRecordedUnitAttack(p->unk00);
        else
            StartRecordedInventionAttack(p->unk04, p->unk06);

        Proc_Break(proc);
    }
}
asm(".global sub_0802C4F0\n.thumb_set sub_0802C4F0, AttackTargetSelect_Resolve\n");
