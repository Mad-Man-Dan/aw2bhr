#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080201E0.
 * sub_080201E0 @ 0x080201E0
 */

/* FillUnitAttackRange's signed-coordinate twin against a different overlay writer.
 * The second GetUnitFiringRangeWithCoBonus call is spelled out again rather than bound to a
 * local: the ROM recomputes the whole army-number chain and re-issues the call
 * before comparing the result against 1. */
void PaintUnitAttackRange(s16 x, s16 y, struct Unit *e)
{
    MapSetInRangeSigned(x, y,
                 GetUnitFiringRangeWithCoBonus(((e - gUnits) >> 6) + 1, e->type), 1);
    if (GetUnitFiringRangeWithCoBonus(((e - gUnits) >> 6) + 1, e->type) != 1)
        MapSetInRangeSigned(x, y, gUnknown_085D5ABC[e->type].minRange - 1, -1);
}
asm(".global sub_080201E0\n.thumb_set sub_080201E0, PaintUnitAttackRange\n");
