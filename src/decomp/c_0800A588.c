#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800A588.
 * sub_0800A588 @ 0x0800A588, sub_0800A6AC @ 0x0800A6AC, sub_0800A798 @ 0x0800A798, sub_0800A884 @ 0x0800A884, sub_0800A95C @ 0x0800A95C
 */

/*
 * sub_0800A588 -- work out the tile for each of the four cells next to (x, y)
 * again.
 *
 * A neighbour is only looked at when it is on the map and IsPlainRiverAt accepts
 * it. sub_0800A95C then classifies it: a negative answer means the cell is
 * wrong and sub_08007F68 repairs it; a positive one is the new tile, which is
 * stored, and sub_0800A098 then tidies that cell's own neighbours. Zero leaves
 * the cell alone.
 *
 * The other four functions in this file all build the same nine-bit mask of the
 * 3 x 3 block around (x, y) -- bit 8 the top left, bit 4 the centre, bit 0 the
 * bottom right, cells off the map left at 0 -- and look it up in a table:
 *   sub_0800A6AC: land bits with the centre forced to 1, table
 *     gUnknown_08486BC4; -1 for a cell that is not land or that sub_080094EC
 *     rejects.
 *   sub_0800A798: the same table, but only for a water cell, and the centre bit
 *     is set only when sub_080094EC returns 0; -1 otherwise.
 *   sub_0800A884: land bits with the centre forced, table gUnknown_084867C4.
 *   sub_0800A95C: bits from sub_08009918 rather than IsTerrainLand and no
 *     centre bit at all, table gUnknown_08486BC4.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - sub_0800A95C returns a plain `int`. Declared s16, its callers would have
 *     to sign-extend the result, and the original does not.
 *   - Each mask is built by writing the nine terms out, and each neighbour is
 *     its own block with its own `n`, rather than as a loop.
 */
#define MAP gMap

void sub_0800A588(int x, int y)
{
    if (y > 0)
    {
        int n = y - 1;
        if (IsPlainRiverAt(x, n))
        {
            int v = sub_0800A95C(x, n);
            if (v < 0)
                sub_08007F68(x, n, 1);
            else if (v > 0)
            {
                MakeTileSimple(x, n, v);
                sub_0800A098(x, n);
            }
        }
    }

    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (IsPlainRiverAt(x, n))
        {
            int v = sub_0800A95C(x, n);
            if (v < 0)
                sub_08007F68(x, n, 1);
            else if (v > 0)
            {
                MakeTileSimple(x, n, v);
                sub_0800A098(x, n);
            }
        }
    }

    if (x > 0)
    {
        int n = x - 1;
        if (IsPlainRiverAt(n, y))
        {
            int v = sub_0800A95C(n, y);
            if (v < 0)
                sub_08007F68(n, y, 1);
            else if (v > 0)
            {
                MakeTileSimple(n, y, v);
                sub_0800A098(n, y);
            }
        }
    }

    if (x < MAP->width - 1)
    {
        int n = x + 1;
        if (IsPlainRiverAt(n, y))
        {
            int v = sub_0800A95C(n, y);
            if (v < 0)
                sub_08007F68(n, y, 1);
            else if (v > 0)
            {
                MakeTileSimple(n, y, v);
                sub_0800A098(n, y);
            }
        }
    }
}

int sub_0800A6AC(int x, int y)
{
    int m;

    if (sub_080094EC(x, y) || IsTerrainLand(x, y) == 0)
        return -1;

    m = 0;

    if (y > 0)
    {
        int n = y - 1;
        if (x > 0)
            m = IsTerrainLand(x - 1, n) << 8;
        m |= IsTerrainLand(x, n) << 7;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n) << 6;
    }
    if (x > 0)
        m |= IsTerrainLand(x - 1, y) << 5;
    m |= 0x10;
    if (x < MAP->width - 1)
        m |= IsTerrainLand(x + 1, y) << 3;
    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (x > 0)
            m |= IsTerrainLand(x - 1, n) << 2;
        m |= IsTerrainLand(x, n) << 1;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n);
    }

    return gUnknown_08486BC4[m];
}

int sub_0800A798(int x, int y)
{
    int m;

    if (IsTerrainNotWater(x, y) == 0)
        return -1;

    m = 0;

    if (y > 0)
    {
        int n = y - 1;
        if (x > 0)
            m = IsTerrainLand(x - 1, n) << 8;
        m |= IsTerrainLand(x, n) << 7;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n) << 6;
    }
    if (x > 0)
        m |= IsTerrainLand(x - 1, y) << 5;
    if (sub_080094EC(x, y) == 0)
        m |= 0x10;
    if (x < MAP->width - 1)
        m |= IsTerrainLand(x + 1, y) << 3;
    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (x > 0)
            m |= IsTerrainLand(x - 1, n) << 2;
        m |= IsTerrainLand(x, n) << 1;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n);
    }

    return gUnknown_08486BC4[m];
}

int sub_0800A884(int x, int y)
{
    int m = 0;

    if (y > 0)
    {
        int n = y - 1;
        if (x > 0)
            m = IsTerrainLand(x - 1, n) << 8;
        m |= IsTerrainLand(x, n) << 7;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n) << 6;
    }
    if (x > 0)
        m |= IsTerrainLand(x - 1, y) << 5;
    m |= 0x10;
    if (x < MAP->width - 1)
        m |= IsTerrainLand(x + 1, y) << 3;
    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (x > 0)
            m |= IsTerrainLand(x - 1, n) << 2;
        m |= IsTerrainLand(x, n) << 1;
        if (x < MAP->width - 1)
            m |= IsTerrainLand(x + 1, n);
    }

    return gUnknown_084867C4[m];
}

int sub_0800A95C(int x, int y)
{
    int m = 0;

    if (y > 0)
    {
        int ny = y - 1;
        if (x > 0)
            m = sub_08009918(x - 1, ny) << 8;
        m |= sub_08009918(x, ny) << 7;
        if (x < MAP->width - 1)
            m |= sub_08009918(x + 1, ny) << 6;
    }
    if (x > 0)
        m |= sub_08009918(x - 1, y) << 5;
    if (x < MAP->width - 1)
        m |= sub_08009918(x + 1, y) << 3;
    if (y < MAP->height - 1)
    {
        int ny = y + 1;
        if (x > 0)
            m |= sub_08009918(x - 1, ny) << 2;
        m |= sub_08009918(x, ny) << 1;
        if (x < MAP->width - 1)
            m |= sub_08009918(x + 1, ny);
    }

    return gUnknown_08486BC4[m];
}
