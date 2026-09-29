#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080401B4.
 * sub_080401B4 @ 0x080401B4, sub_08040200 @ 0x08040200
 */

#include "proc.h"
/* Hands the proc's entry to StartExplosionEffect with the entry's cell pair and its
 * class tag, then reports the entry's index and refreshes the list.
 *
 * StartExplosionEffect's FOURTH argument is invisible at the call: r3 still holds the
 * proc from the `adds r3, r0, #0` in the prologue, so no instruction sets it
 * up -- the declared `ProcPtr` fourth parameter is what proves it is there.
 *
 * The magic-number chain ends `asr #2` and NOT `asr #8`, so this is the bare
 * pointer subtraction `ent - gUnits` with no `>> 6`: it is the unit
 * index, not the army number that IsIndirectFireUnitArmed and IsDirectFireUnitArmed derive. */
struct Unk401B4Proc
{
    /* 00 */ u8 filler_00[0x4c];
    /* 4c */ struct Unit *unk4c;
};
/* Marks the entry's cell as occupied on two planes of the gUnknown_08499590
 * map, then refreshes. The map header is modelled as a struct for the same
 * reason IsCellOpenForDrop and StartPipeSeamHit need it: the ROM computes every plane
 * address as `(map + K) + idx`, an association that only survives through a
 * COMPONENT_REF.
 *
 * gUnknown_08499590 is RE-READ for the second store (`ldr r2,[r5]` a second
 * time off the same pool register): the intervening `strb` goes through a
 * `u8 *` and kills the non-const pointer global's MEM, so writing the global
 * honestly twice is what reproduces the reload.
 *
 * StartExplosionEffectDefault's third argument is invisible at the call -- r1 still holds this
 * function's own second parameter -- which its declared `ProcPtr` third
 * parameter is what proves. */
void UnitDestroyed_ExplodeAndRemoveUnit(struct Unk401B4Proc *proc)
{
    struct Unit *ent = proc->unk4c;

    StartExplosionEffect(ent->x, ent->y, gUnknown_085D5ABC[ent->type].unitClass, proc);
    DestroyUnitAndCargo(ent - gUnits);
    RebuildMapUnitLayers();
}
asm(".global sub_080401B4\n.thumb_set sub_080401B4, UnitDestroyed_ExplodeAndRemoveUnit\n");

void DestroyLaserOrMinicannon(struct Unk02028360 *ent, ProcPtr a2)
{
    StartExplosionEffectDefault(ent->unk00, ent->unk01, a2);

    gMap->terrain[
        gMap->rowOffset[ent->unk01]
        + ent->unk00] = 1;

    gMap->tile[
        gMap->rowOffset[ent->unk01]
        + ent->unk00] = 4;

    RebuildMapUnitLayers2();
    RemoveInventionRecord((struct Unk3E0D0 *)ent);
    RecountArmyProperties();
}
asm(".global sub_08040200\n.thumb_set sub_08040200, DestroyLaserOrMinicannon\n");
