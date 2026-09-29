#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A54C.
 * sub_0802A54C @ 0x0802A54C
 */

#include "proc.h"
/* Starts the gUnknown_0849A198 proc for a live, non-flag-8 unit and parks the
 * unit in the new proc's +0x4c. The two guards are separate `if`s rather than
 * one `&&` only in spelling -- both jump to the same exit and neither has a
 * body, so this is the short-circuit shape either way.
 *
 * The +0x4c member is a `struct Unit *` on UnitDestroy_Execute's evidence, not
 * a guess: that handler reads the slot back, subtracts gUnits from
 * it and divides the difference by 12 to recover the unit index. The proc
 * record itself is not otherwise modelled, so its struct stays local to the
 * file per the header rules. */

struct Unk2A54CProc
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ struct Unit *unk4c;
};

void StartUnitDestroy(struct Unit *unit, ProcPtr parent)
{
    struct Unk2A54CProc *proc;

    if (unit->type == 0)
        return;

    if (unit->flags & 8)
        return;

    proc = Proc_Start(gUnknown_0849A198, parent);
    proc->unk4c = unit;
}
asm(".global sub_0802A54C\n.thumb_set sub_0802A54C, StartUnitDestroy\n");
