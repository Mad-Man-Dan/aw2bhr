#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803E6C4.
 * sub_0803E6C4 @ 0x0803E6C4
 */

#include "map.h"

/*
 * sub_0803E6C4 -- run sub_0803E560 on every unit in a 3-column strip below a
 * point.
 *
 * Scans map columns a1 .. a1+2, from row a2+3 down to the bottom edge of the
 * map. For each tile that holds a unit, the unit's army is the top two bits of
 * its map byte; if that army's player (gPlayers[army + 1]) does not have
 * teamColor 5, it calls sub_0803E560(x, y, unit byte, a3). What teamColor 5
 * marks is not known yet.
 *
 * Why the C looks odd:
 *   - `col = a1;` is a separate first statement. Using a1 directly makes the
 *     compiler copy the parameters in a different order at entry.
 */

void sub_0803E6C4(int a1, int a2, int a3)
{
    int row;
    int i;
    int t;
    int col;

    col = a1;
    row = a2 + 3;
    if (row < ((struct Map *)gUnknown_08499590)->height)
    {
        do
        {
            for (i = 0; i <= 2; i++)
            {
                if (((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[row] + (col + i)] != 0)
                {
                    t = ((struct Map *)gUnknown_08499590)->unit[((struct Map *)gUnknown_08499590)->rowOffset[row] + (col + i)];
                    if (gPlayers[(t >> 6) + 1].teamColor != 5)
                        sub_0803E560(col + i, row, t, a3);
                }
            }
            row++;
        } while (row < ((struct Map *)gUnknown_08499590)->height);
    }
}
