#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062730.
 * sub_08062730 @ 0x08062730
 */

/* `dist` is a BOUND local, not `dx + dy` at the compare: the ROM keeps the sum
 * in r7 across both calls, and leaving the two halves separate costs an r8
 * spill for gUnknown_030033EC (+4 bytes, measured).
 *
 * The tail is `if (dist > cost) return 0; else return 1;` and not the `<=`
 * form. Both are the same 24-byte `movs`/`b`/`movs` shape, but agbcc emits the
 * `<=` spelling with `ble` reaching the 1-arm, while the ROM has `bgt` reaching
 * the 0-arm -- i.e. the source's if-body is the ZERO. */
int AiIsEnemyThreatWindowNearUnit(struct Unit *a, struct Unit *b)
{
    int dx;
    int dy;
    int dist;
    int cost;

    dx = a->x - b->x;
    if (dx < 0)
        dx = -dx;

    dy = a->y - b->y;
    if (dy < 0)
        dy = -dy;

    dist = dx + dy;

    if (GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, b->type) == 1)
        cost = GetUnitMovementBudget(a) + GetUnitMovementBudget(b) + 1;
    else
        cost = GetUnitMovementBudget(a) + GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, b->type);

    if (dist > cost)
        return 0;
    else
        return 1;
}
asm(".global sub_08062730\n.thumb_set sub_08062730, AiIsEnemyThreatWindowNearUnit\n");
