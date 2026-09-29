#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080211DC.
 * sub_080211DC @ 0x080211DC
 */

void StampUnitVision(u8 a1, s8 a2)
{
    struct Unit *e;
    struct Map *map;
    int bonus;

    e = &gUnits[a1];
    bonus = 0;

    if (e->type == 0)
        return;

    if ((e->flags & 6) == 2)
        return;

    if (e->type <= 2)
    {
        map = gMap;

        if ((map->terrain[map->rowOffset[e->y] + e->x] & 0x1f) == 3)
            bonus = 3;
    }

    StampVisionByPlaneMask(e->x, e->y,
                 bonus + GetUnitVisionWithCoBonus(((e - gUnits) >> 6) + 1, e->type),
                 gPlayers[(a1 >> 6) + 1].turnState,
                 a2, (a1 >> 6) + 1);
}
asm(".global sub_080211DC\n.thumb_set sub_080211DC, StampUnitVision\n");
