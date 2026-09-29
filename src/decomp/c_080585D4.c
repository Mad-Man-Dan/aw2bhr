#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080585D4.
 * sub_080585D4 @ 0x080585D4
 */

/* Weighted count over the whole map: every passable cell (gUnknown_03003340
 * non-negative as an s8) whose terrain byte is accepted by gUnknown_085767D5
 * and which IsTerrainOwnedByUnitsTeam does not reject scores 0x1e if its terrain code is
 * exactly 8 and 1 otherwise.
 *
 * The clean gMap field spelling is byte-exact here: the repeated
 * rowOffset/terrain expressions preserve the original reload shape without a
 * local byte-pointer overlay. */
int AiScoreEnemyPropertiesInReach(void)
{
    int acc;
    int x;
    int y;
    int off;

    acc = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                off = gMap->rowOffset[y] + x;
                if (gUnknown_085767D5[gMap->terrain[off] & 0x1f] != 0
                    && IsTerrainOwnedByUnitsTeam(gUnknown_03003F38, gMap->terrain[off]) == 0)
                {
                    off = gMap->rowOffset[y] + x;
                    if ((gMap->terrain[off] & 0x1f) == 8)
                        acc += 0x1e;
                    else
                        acc += 1;
                }
            }
        }
    }

    return acc;
}
asm(".global sub_080585D4\n.thumb_set sub_080585D4, AiScoreEnemyPropertiesInReach\n");
