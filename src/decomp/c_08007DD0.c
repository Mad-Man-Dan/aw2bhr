#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007DD0.
 * sub_08007DD0 @ 0x08007DD0
 */

#define MAP gMap

s16 sub_08007DD0(int x, int y)
{
    int mask = 0;
    int t;

    if (y > 0)
    {
        int ny = y - 1;
        if (x > 0)
            mask |= IsTerrainLand(x - 1, ny) << 8;
        mask |= IsTerrainLand(x, ny) << 7;
        if (x < MAP->width - 1)
            mask |= IsTerrainLand(x + 1, ny) << 6;
    }
    if (x > 0)
        mask |= IsTerrainLand(x - 1, y) << 5;
    mask |= IsTerrainLand(x, y) << 4;
    if (x < MAP->width - 1)
        mask |= IsTerrainLand(x + 1, y) << 3;
    if (y < MAP->height - 1)
    {
        int ny = y + 1;
        if (x > 0)
            mask |= IsTerrainLand(x - 1, ny) << 2;
        mask |= IsTerrainLand(x, ny) << 1;
        if (x < MAP->width - 1)
            mask |= IsTerrainLand(x + 1, ny);
    }

    t = MAP->terrain[MAP->rowOffset[y] + x];
    if (t == 0xd)
        return GetShoalTile(x, y);
    if (t == 2)
        return -1;
    if (t == 0xc)
    {
        if (sub_08008C34(x, y) == 0)
        {
            register s16 *table asm("r1");
            table = gUnknown_08485DC4;
            return table[mask];
        }
        return -1;
    }
    {
        register s16 *table asm("r1");
        table = gUnknown_08485DC4;
        return table[mask];
    }
}

/*
 * sub_08007DD0 (the function above) -- choose the tile shape for (x, y) from
 * which of its neighbours are land.
 *
 * `mask` collects one bit per cell of the 3 x 3 block around (x, y), from
 * IsTerrainLand: bit 8 is the top left, bit 4 the cell itself, bit 0 the
 * bottom right. Cells off the edge of the map stay 0.
 *
 * What happens with the mask depends on the terrain already at (x, y):
 *   0xD  -- ignore it and return GetShoalTile's answer for the cell.
 *   2    -- return -1, meaning no tile.
 *   0xC  -- use the mask only when sub_08008C34 returns 0, else return -1.
 *   any other terrain -- use the mask.
 * Using the mask means returning gUnknown_08485DC4[mask], the shape table.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - Both of the blocks that read the table bind it to a pointer pinned to
 *     r1. That is what puts the load of the table's address and the shift of
 *     the index in the original's order on both return paths.
 */
