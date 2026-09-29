#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020020.
 * sub_08020020 @ 0x08020020
 */

/* Grows the movement-range overlay by one cell: for every cell that is neither
 * negative nor already 1, any negative orthogonal neighbour that exists becomes
 * 1. Each neighbour's own sign is tested BEFORE its edge test -- that order is
 * the ROM's and it is what puts the `cmp rX,#0` after the `ldrsb`.
 *
 * gUnknown_03003340 is declared `u8 *[]` and every read here is `ldrsb`, so the
 * (s8 *) cast is on THIS function's access, not on the global: the declaration
 * is left alone per the shared-type rule. FillMovementMap, which fills the same
 * table, stores a u8 -- the two are consistent, the plane just carries a
 * negative "unreached" sentinel that only this reader looks at. */
#define MAP gMap
#define ROW(n) ((s8 *)gUnknown_03003340[n])

void MapMarkHaloOneUnused(void)
{
    int x, y;

    for (y = 0; y < MAP->height; y++)
    {
        for (x = 0; x < MAP->width; x++)
        {
            if (ROW(y)[x] < 0 || ROW(y)[x] == 1)
                continue;
            if (ROW(y)[x - 1] < 0 && x != 0)
                ROW(y)[x - 1] = 1;
            if (ROW(y)[x + 1] < 0 && x != MAP->width - 1)
                ROW(y)[x + 1] = 1;
            if (ROW(y - 1)[x] < 0 && y != 0)
                ROW(y - 1)[x] = 1;
            if (ROW(y + 1)[x] < 0 && y != MAP->height - 1)
                ROW(y + 1)[x] = 1;
        }
    }
}
asm(".global sub_08020020\n.thumb_set sub_08020020, MapMarkHaloOneUnused\n");
