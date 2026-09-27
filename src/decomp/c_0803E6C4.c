#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E6C4.
 * ScanUnitsBelowStrip @ 0x0803E6C4
 *
 * Not a Xenesis-documented name. The old sub_0803E6C4 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"

/* For each row from y + 3 to the bottom of the map, looks at the three cells
 * starting at column x. Every cell holding a unit is passed to sub_0803E560
 * (column, row, the unit byte, a3), unless the owning player's teamColor
 * (player slot = (unit byte >> 6) + 1) is 5.
 *
 * Measured spelling notes (parked since wave 38 at 156/160, -4):
 * - All map reads through gMap's members, the first height check included.
 *   Naming gMap lets -fforce-addr emit its own address word; the draft spelled
 *   that first check through gUnknown_080912FC and was 4 bytes short.
 * - `left = x;` copied into a local before the loop is what orders the
 *   prologue's parameter moves (a3's copy before x's move to sl). Found by
 *   decomp-permuter; 97.5% without it.
 */
void ScanUnitsBelowStrip(int x, int y, int a3)
{
    int row;
    int left;
    int i;
    int t;

    left = x;
    row = y + 3;
    if (row < gMap->height)
    {
        do
        {
            for (i = 0; i <= 2; i++)
            {
                if (gMap->unit[gMap->rowOffset[row] + (left + i)] != 0)
                {
                    t = gMap->unit[gMap->rowOffset[row] + (left + i)];
                    if (gPlayers[(t >> 6) + 1].teamColor != 5)
                        sub_0803E560(left + i, row, t, a3);
                }
            }
            row++;
        } while (row < gMap->height);
    }
}

asm(".global sub_0803E6C4\n.thumb_set sub_0803E6C4, ScanUnitsBelowStrip\n");
