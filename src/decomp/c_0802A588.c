#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A588.
 * sub_0802A588 @ 0x0802A588
 */

#include "proc.h"
/* The handler for the proc StartUnitDestroy starts. `adds r2, r0, #0` in the
 * prologue is not a spill: it is the THIRD argument of StartExplosionEffectDefault being
 * staged. StartExplosionEffectDefault's whole body is `adds r3,r2,#0; movs r2,#0;
 * bl StartExplosionEffect`, i.e. it forwards r2 into StartExplosionEffect's `ProcPtr` parent
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

void UnitDestroy_Execute(struct Unk2A588Proc *proc)
{
    struct Unit *unit = proc->unk4c;

    StartExplosionEffectDefault(unit->x, unit->y, proc);
    DestroyUnitAndCargo(unit - gUnits);
    RebuildMapUnitLayers();
}
asm(".global sub_0802A588\n.thumb_set sub_0802A588, UnitDestroy_Execute\n");
