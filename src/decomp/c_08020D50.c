#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020D50.
 * sub_08020D50 @ 0x08020D50
 */

/* FillUnitAttackRange's signed-coordinate twin against a third overlay writer. Here
 * the guard reads gUnknown_085D5ABC[t].unk0f rather than re-calling
 * GetUnitFiringRangeWithCoBonus, and the ROM keeps the element ADDRESS live across the test to
 * reach .unk0e -- one subscript expression, two members. */
void FillUnitTargetRange(s16 x, s16 y, struct Unit *e)
{
    MarkAttackableCellsInRange(x, y,
                 GetUnitFiringRangeWithCoBonus(((e - gUnits) >> 6) + 1, e->type), 0);
    if (gUnknown_085D5ABC[e->type].maxRange != 1)
        MarkAttackableCellsInRange(x, y, gUnknown_085D5ABC[e->type].minRange - 1, -1);
}
asm(".global sub_08020D50\n.thumb_set sub_08020D50, FillUnitTargetRange\n");
