#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074320.
 * sub_08074320 @ 0x08074320
 */

#include "proc.h"

/* The seventh member of the RunMapEventsAfterTurnSupply null-guard family, with a caller
 * filter in front: the argument's slot index in gUnits must have
 * neither of bits 6-7 set.
 */
u8 RunMapEventsOnUnitSelected(struct Unk030040D8 *a1)
{
    const struct Unk08074584 *p = GetMapEventTable();
    int r;

    if ((((struct Unit *)a1 - gUnits) & 0xC0) == 0
        && p != 0 && p->unk08 != 0)
    {
        r = RunMapEventRecords(p->unk08, a1, 0);

        if (r != 0)
            Proc_Start(gUnknown_08614268, PROC_TREE_3);

        return r;
    }

    return 0;
}
asm(".global sub_08074320\n.thumb_set sub_08074320, RunMapEventsOnUnitSelected\n");
