#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800B61C.
 * sub_0800B61C @ 0x0800B61C
 */

/*
 * GetShoalTile -- choose the tile for the shoreline at (x, y).
 *
 * The nine-bit mask of which cells of the 3 x 3 block around (x, y) are land
 * (bit 8 the top left, bit 0 the bottom right, the centre bit dropped again
 * with `& ~0x10`) indexes gUnknown_084861C4. A negative entry is returned as it
 * stands. Otherwise bits 9 to 14 of the entry name one of nineteen shapes that
 * need a closer look, and that case picks the tile:
 *   - eleven of them ask IsShoalAt about the single neighbour on the side
 *     the shape points at and choose between two tiles; off the edge of the map
 *     counts as the plain one.
 *   - the rest ask GetShoalNeighbourMask about the cell itself, mask its answer and
 *     choose between four tiles. Case 0xA00 has two further tests on the
 *     top-left diagonal.
 * A positive result is finally masked down to nine bits.
 *
 * Case 0x800 tests `x > 0` and then looks at the cell above, where the other
 * up/down cases test `y > 0`. The original does the same.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The cases are listed in the order the original's blocks appear in, which
 *     is not ascending and follows no other obvious rule. The compiler compares
 *     in value order but lays the bodies out in source order, so reordering
 *     them rewrites the whole tail of the function.
 *   - In the eleven two-way cases the call's result goes into `u` first, then
 *     the plain tile is assigned, then `if (u)` overwrites it. Written as
 *     `if (guard && call())` the compiler assigns the two the other way round
 *     and drops a test the original keeps.
 *   - `u` and `t` stay two separate locals although no arm uses both. Sharing
 *     one costs a register copy in each of the eleven two-way cases.
 *   - Case 0xA00's inner chain is an if/else-if, not a `switch`. A three-case
 *     switch is compiled as a balanced comparison tree and tests the wrong
 *     value first.
 */

#define MAP gMap

s16 GetShoalTile(int x, int y)
{
    int mask = 0;
    int t;
    int u;
    int r;

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

    r = gUnknown_084861C4[mask & ~0x10];
    if (r < 0)
        return r;

    switch (r & 0x7e00)
    {
    case 0x400:
        if (x > 0)
        {
            u = IsShoalAt(x - 1, y);
            r = 0xb6;
            if (u)
                r = 0xf3;
        }
        else
            r = 0xb6;
        break;
    case 0x200:
        if (x < MAP->width - 1)
        {
            u = IsShoalAt(x + 1, y);
            r = 0xb7;
            if (u)
                r = 0xf2;
        }
        else
            r = 0xb7;
        break;
    case 0x800:
        if (x > 0)
        {
            u = IsShoalAt(x, y - 1);
            r = 0xb6;
            if (u)
                r = 0xd2;
        }
        else
            r = 0xb6;
        break;
    case 0x4a00:
        if (x < MAP->width - 1)
        {
            u = IsShoalAt(x + 1, y);
            r = 0x8f;
            if (u)
                r = 0x6d;
        }
        else
            r = 0x8f;
        break;
    case 0x4c00:
        if (x > 0)
        {
            u = IsShoalAt(x - 1, y);
            r = 0x8f;
            if (u)
                r = 0x6e;
        }
        else
            r = 0x8f;
        break;
    case 0x5200:
        if (x < MAP->width - 1)
        {
            u = IsShoalAt(x + 1, y);
            r = 0xef;
            if (u)
                r = 0xcd;
        }
        else
            r = 0xef;
        break;
    case 0x5400:
        if (x > 0)
        {
            u = IsShoalAt(x - 1, y);
            r = 0xef;
            if (u)
                r = 0xce;
        }
        else
            r = 0xef;
        break;
    case 0x2c00:
        if (y > 0)
        {
            u = IsShoalAt(x, y - 1);
            r = 0x92;
            if (u)
                r = 0xcf;
        }
        else
            r = 0x92;
        break;
    case 0x3400:
        if (y < MAP->height - 1)
        {
            u = IsShoalAt(x, y + 1);
            r = 0x92;
            if (u)
                r = 0xaf;
        }
        else
            r = 0x92;
        break;
    case 0x2a00:
        if (y > 0)
        {
            u = IsShoalAt(x, y - 1);
            r = 0x93;
            if (u)
                r = 0xd0;
        }
        else
            r = 0x93;
        break;
    case 0x3200:
        if (y < MAP->height - 1)
        {
            u = IsShoalAt(x, y + 1);
            r = 0x93;
            if (u)
                r = 0xb0;
        }
        else
            r = 0x93;
        break;
    case 0x4800:
        t = GetShoalNeighbourMask(x, y) & 7;
        if (t == 6)
            r = 0x10;
        else if (t == 4)
            r = 0x6e;
        else if (t == 2)
            r = 0x6d;
        else
            r = 0x8f;
        break;
    case 0x5000:
        t = GetShoalNeighbourMask(x, y) & 0xe;
        if (t == 6)
            r = 0x50;
        else if (t == 2)
            r = 0xcd;
        else if (t == 4)
            r = 0xce;
        else
            r = 0xef;
        break;
    case 0x2400:
        t = GetShoalNeighbourMask(x, y) & 9;
        if (t == 9)
            r = 0x2e;
        else if (t == 8)
            r = 0xcf;
        else if (t == 1)
            r = 0xaf;
        else
            r = 0x92;
        break;
    case 0x2200:
        t = GetShoalNeighbourMask(x, y) & 9;
        if (t == 9)
            r = 0x32;
        else if (t == 8)
            r = 0xd0;
        else if (t == 1)
            r = 0xb0;
        else
            r = 0x93;
        break;
    case 0xc00:
        t = GetShoalNeighbourMask(x, y);
        if (t == 4)
            r = 0xf3;
        else if (t == 8)
            r = 0xd2;
        else if (t == 0xc)
            r = 0x2f;
        else
            r = 0xb6;
        break;
    case 0xa00:
        t = GetShoalNeighbourMask(x, y);
        if (t == 2)
            r = 0xf2;
        else if (t == 8)
        {
            if (x > 0 && y > 0 && IsTerrainLand(x - 1, y - 1))
                r = 0xd3;
            else
                r = 0xb7;
        }
        else if (t == 0xa)
        {
            if (x > 0 && y > 0 && IsTerrainLand(x - 1, y - 1))
                r = 0x31;
            else
                r = 0xf2;
        }
        else
            r = 0xb7;
        break;
    case 0x1400:
        t = GetShoalNeighbourMask(x, y);
        if (t == 4)
            r = 0x113;
        else if (t == 1)
            r = 0xb2;
        else if (t == 5)
            r = 0x4f;
        else
            r = 0xd6;
        break;
    case 0x1200:
        t = GetShoalNeighbourMask(x, y);
        if (t == 2)
            r = 0x112;
        else if (t == 1)
            r = 0xb3;
        else if (t == 3)
            r = 0x51;
        else
            r = 0xd7;
        break;
    }

    if (r > 0)
        r &= 0x1ff;

    return r;
}
asm(".global sub_0800B61C\n.thumb_set sub_0800B61C, GetShoalTile\n");
