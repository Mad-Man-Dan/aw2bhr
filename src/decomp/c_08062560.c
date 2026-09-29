#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062560.
 * sub_08062560 @ 0x08062560
 */

void AiAddArmyThreat(u16 a1, u8 a2)
{
    struct Unit *e;
    int u;
    int x;
    int y;
    int off;

    gUnknown_03004480 = a1;
    for (u = (a1 - 1) * 0x40; u < (a1 - 1) * 0x40 + 0x40; u++) {
        e = &gUnits[u];
        if (e->type == 0)
            continue;
        if ((gUnknown_085D5ABC[e->type].unk1c & a2) == 0)
            continue;
        if ((u8)(e->flags & 8) != 0)
            continue;
        if ((u8)AiIsEnemyThreatWindowNearUnit((struct Unit *)gUnknown_030040D8, e) == 0)
            continue;
        if (e->type == 0x18 && (u8)IsUnitVisibleToCurrentTeam(u) == 0)
            continue;
        if ((u8)IsCellVisibleToArmy(gUnknown_03004480, e->x, e->y) == 0)
            continue;
        if (GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, e->type) == 1) {
            gUnknown_030013EC(e->x, e->y, e->type,
                              GetUnitMovementWithCoBonus(gUnknown_030033EC, e->type), -1);
            MapMarkHalo(0x79);
        } else {
            gUnknown_030013EC(e->x, e->y, 0x10,
                              GetUnitFiringRangeWithCoBonus(gUnknown_030033EC, e->type), 0);
        }
        for (y = 0; y < gMap->height; y++) {
            for (x = 0; x < gMap->width; x++) {
                if ((s8)gUnknown_03003340[y][x] >= 0) {
                    off = gMap->rowOffset[y] + x;
                    gMap->dangerMask[off] |= gUnknown_085D5ABC[e->type].unk1c;
                }
            }
        }
    }
}
asm(".global sub_08062560\n.thumb_set sub_08062560, AiAddArmyThreat\n");
