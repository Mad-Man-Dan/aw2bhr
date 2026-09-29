#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800A098.
 * sub_0800A098 @ 0x0800A098, sub_0800A2EC @ 0x0800A2EC
 */

/*
 * sub_0800A098 -- repair the cell on each side of (x, y) after its tile changed.
 *
 * The tile now at (x, y) says which pair of neighbours can be wrong: 0x39 the
 * two horizontal ones, 0x18 the two vertical ones. A neighbour is only touched
 * when IsTerrainNotWater says it is not water, (x, y) still reads that same tile,
 * and CountLandOnSide has nothing against that direction (0 left, 1 right, 2 up,
 * 4 down). It is then made sea -- terrain 2 with a fixed tile, 0x11D, 0xFD,
 * 0xFC or 0x11C, one per direction. Finally, if the terrain in the cell
 * diagonally beyond it reads 0xD, MakeShoal is called on that cell.
 *
 * The diagonal reads have no bounds check: with y == 0 the 0x39 arms read the
 * row table one entry before its start, and the 0x18 arms can reach column -1.
 * The original does the same.
 *
 * sub_0800A2EC below is a separate sweep: every one of the four neighbours that
 * IsPlainRiverAt accepts is set to terrain 1 with tile 1.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The diagonal index is written `(t = <row> -+ 1, t + x)`, so the 1 is
 *     added to the row and x afterwards. As one expression the compiler
 *     regroups it, either folding the 1 into the terrain array's own offset or
 *     sharing it with an `x - 1` computed elsewhere in the function. Binding
 *     the row in a statement of its own instead lifts the whole row load above
 *     the two calls that must come first.
 *   - `t` is declared inside each arm so that each one is short-lived.
 */
#define MAP gMap

void sub_0800A098(int x, int y)
{
    int c = MAP->tile[MAP->rowOffset[y] + x];

    if (c == 0x39)
    {
        if (x > 0)
        {
            int nx = x - 1;
            if (IsTerrainNotWater(nx, y) == 0
             && MAP->tile[MAP->rowOffset[y] + x] == 0x39
             && CountLandOnSide(nx, y, 0) == 0)
            {
                int t;
                SetTerrainAt(nx, y, 2);
                MakeTileSimple(nx, y, 0x11d);
                if (MAP->terrain[(t = MAP->rowOffset[y - 1] - 1, t + x)] == 0xd)
                    MakeShoal(nx, y - 1);
            }
        }
        if (x < MAP->width - 1)
        {
            int nx = x + 1;
            if (IsTerrainNotWater(nx, y) == 0
             && MAP->tile[MAP->rowOffset[y] + x] == 0x39
             && CountLandOnSide(nx, y, 1) == 0)
            {
                int t;
                SetTerrainAt(nx, y, 2);
                MakeTileSimple(nx, y, 0xfd);
                if (MAP->terrain[(t = MAP->rowOffset[y - 1] + 1, t + x)] == 0xd)
                    MakeShoal(nx, y - 1);
            }
        }
    }
    if (c == 0x18)
    {
        if (y > 0)
        {
            int ny = y - 1;
            if (IsTerrainNotWater(x, ny) == 0
             && MAP->tile[MAP->rowOffset[y] + x] == 0x18
             && CountLandOnSide(x, ny, 2) == 0)
            {
                int t;
                SetTerrainAt(x, ny, 2);
                MakeTileSimple(x, ny, 0xfc);
                if (MAP->terrain[(t = MAP->rowOffset[ny] - 1, t + x)] == 0xd)
                    MakeShoal(x - 1, ny);
            }
        }
        if (y < MAP->height - 1)
        {
            int ny = y + 1;
            if (IsTerrainNotWater(x, ny) == 0
             && MAP->tile[MAP->rowOffset[y] + x] == 0x18
             && CountLandOnSide(x, ny, 4) == 0)
            {
                int t;
                SetTerrainAt(x, ny, 2);
                MakeTileSimple(x, ny, 0x11c);
                if (MAP->terrain[(t = MAP->rowOffset[ny] - 1, t + x)] == 0xd)
                    MakeShoal(x - 1, ny);
            }
        }
    }
}

void sub_0800A2EC(int x, int y)
{
    if (y > 0)
    {
        int n = y - 1;
        if (IsPlainRiverAt(x, n))
        {
            SetTerrainAt(x, n, 1);
            MakeTileSimple(x, n, 1);
        }
    }

    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (IsPlainRiverAt(x, n))
        {
            SetTerrainAt(x, n, 1);
            MakeTileSimple(x, n, 1);
        }
    }

    if (x > 0)
    {
        int n = x - 1;
        if (IsPlainRiverAt(n, y))
        {
            SetTerrainAt(n, y, 1);
            MakeTileSimple(n, y, 1);
        }
    }

    if (x < MAP->width - 1)
    {
        int n = x + 1;
        if (IsPlainRiverAt(n, y))
        {
            SetTerrainAt(n, y, 1);
            MakeTileSimple(n, y, 1);
        }
    }
}
