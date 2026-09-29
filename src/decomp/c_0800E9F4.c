#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800E9F4.
 * sub_0800E9F4 @ 0x0800E9F4
 */

#include "map.h"
#define TILE(xx, yy) (gMap->tile[gMap->rowOffset[(yy)] + (xx)])
#define TILE_IS_PAIR(t, first) ((u16)((t) - (first)) <= 1)

/* Identifies which corner of a 2x2 tile block (0x24/0x25, 0x27 over
 * 0x64/0x65, 0x67) the tile at (x, y) is:
 * 1 = top-left (0x24/0x25), 2 = top-right (0x27), 3 = bottom-left
 * (0x64/0x65), 0 if the surrounding tiles do not form the block.
 *
 * Every neighbour must be written as TILE(x + dx, y + dy), which expands to
 * `rowOffset[y + dy] + (x + dx)`. Spelled `rowOffset[..] + dx + x`, fold
 * reassociates and CSEs the column sums, and the function is 8 bytes short. */
int GetForestBlockCorner(int x, int y)
{
    int result = 0;

    switch (TILE(x, y))
    {
    case 0x24:
    case 0x25:
        if (TILE(x + 1, y) == 0x27
         && TILE_IS_PAIR(TILE(x, y + 1), 0x64)
         && TILE(x + 1, y + 1) == 0x67)
            result = 1;
        break;
    case 0x27:
        if (TILE_IS_PAIR(TILE(x - 1, y), 0x24)
         && TILE_IS_PAIR(TILE(x - 1, y + 1), 0x64)
         && TILE(x, y + 1) == 0x67)
            result = 2;
        break;
    case 0x64:
    case 0x65:
        if (TILE_IS_PAIR(TILE(x, y - 1), 0x24)
         && TILE(x + 1, y - 1) == 0x27
         && TILE(x + 1, y) == 0x67)
            result = 3;
        break;
    }
    return result;
}
asm(".global sub_0800E9F4\n.thumb_set sub_0800E9F4, GetForestBlockCorner\n");
