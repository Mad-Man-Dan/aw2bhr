#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800BA9C.
 * sub_0800BA9C @ 0x0800BA9C, sub_0800BB2C @ 0x0800BB2C
 */

#define MAP gMap

int MakeShoal(int x, int y)
{
    int t;
    int v;

    if (sub_0800B528(x, y) < 0)
        return 0;

    MakeSeaSafest(x, y);
    t = MAP->terrain[MAP->rowOffset[y] + x];
    SetTerrainAt(x, y, 0xd);

    v = GetShoalTile(x, y);

    if (v < 0)
    {
        SetTerrainAt(x, y, t);
        return 0;
    }

    MakeTileSimple(x, y, v);

    if (IsTerrainAtCoordsType(x, y, 7) == 0)
        RepaintShoalNeighbours(x, y);

    return 1;
}
asm(".global sub_0800BA9C\n.thumb_set sub_0800BA9C, MakeShoal\n");

void RepaintShoalNeighbours(int x, int y)
{
    int k = 0x2a;

    if (y > 0)
    {
        int n = y - 1;
        if (IsShoalAt(x, n))
        {
            int v = GetShoalTile(x, n);
            if (v < 0)
            {
                SetTerrainAt(x, n, 7);
                MakeTileSimple(x, n, k);
            }
            else
                MakeTileSimple(x, n, v);
        }
    }

    if (x > 0)
    {
        int n = x - 1;
        if (IsShoalAt(n, y))
        {
            int v = GetShoalTile(n, y);
            if (v < 0)
            {
                SetTerrainAt(n, y, 7);
                MakeTileSimple(n, y, k);
            }
            else
                MakeTileSimple(n, y, v);
        }
    }

    if (x < MAP->width - 1)
    {
        int n = x + 1;
        if (IsShoalAt(n, y))
        {
            int v = GetShoalTile(n, y);
            if (v < 0)
            {
                SetTerrainAt(n, y, 7);
                MakeTileSimple(n, y, k);
            }
            else
                MakeTileSimple(n, y, v);
        }
    }

    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (IsShoalAt(x, n))
        {
            int v = GetShoalTile(x, n);
            if (v < 0)
            {
                SetTerrainAt(x, n, 7);
                MakeTileSimple(x, n, k);
            }
            else
                MakeTileSimple(x, n, v);
        }
    }
}
asm(".global sub_0800BB2C\n.thumb_set sub_0800BB2C, RepaintShoalNeighbours\n");
