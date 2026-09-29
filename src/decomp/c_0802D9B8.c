#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D9B8.
 * sub_0802D9B8 @ 0x0802D9B8
 */

void sub_0802D9B8(struct Unk03001470 *proc)
{
    if (proc->unk20 == proc->unk1e && proc->unk20 > 0)
        proc->unk1e = proc->unk1e - 1;

    if (proc->unk20 == proc->unk1e + 6 && proc->unk1e <= proc->unk22 - 7)
        proc->unk1e = proc->unk1e + 1;

    ClearDeploymentListArea();
    DrawDeploymentList(proc->unk1e);
    RefreshDeploymentUnitInfo(gUnknown_02023830[proc->unk20 * 4]);
}
