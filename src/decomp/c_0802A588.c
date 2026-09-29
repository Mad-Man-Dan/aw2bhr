#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A588.
 * sub_0802A588 @ 0x0802A588
 */

#include "proc.h"
/* The handler for the proc sub_0802A54C starts. `adds r2, r0, #0` in the
 * prologue is not a spill: it is the THIRD argument of sub_0803FECC being
 * staged. sub_0803FECC's whole body is `adds r3,r2,#0; movs r2,#0;
 * bl sub_0803FF48`, i.e. it forwards r2 into sub_0803FF48's `ProcPtr` parent
 * slot -- so this proc passes itself as the parent, and nothing else explains
 * the copy.
 *
 * The `lsls/adds` cascade ending in `rsbs; asrs #2` is agbcc's division by 12
 * for a pointer difference; write it as `unit - gUnits` and it falls
 * out. That difference is also what proves the +0x4c member's type. */

struct Unk2A588Proc
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ struct Unit *unk4c;
};

void sub_0802A588(struct Unk2A588Proc *proc)
{
    struct Unit *unit = proc->unk4c;

    sub_0803FECC(unit->x, unit->y, proc);
    DestroyUnitAndCargo(unit - gUnits);
    RebuildMapUnitLayers();
}
