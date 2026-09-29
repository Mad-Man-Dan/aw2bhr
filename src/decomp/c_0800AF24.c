#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800AF24.
 * sub_0800AF24 @ 0x0800AF24, sub_0800AF74 @ 0x0800AF74, sub_0800AFCC @ 0x0800AFCC
 */

#define MAP gMap

void sub_0800AF24(int x, int y)
{
    if (y > 0)
    {
        int n = y - 1;
        if (IsTerrainAtCoordsType(x, n, 3))
            MakeMountain(x, n);
    }

    if (y < MAP->height - 1)
    {
        int n = y + 1;
        if (IsTerrainAtCoordsType(x, n, 3))
            MakeMountain(x, n);
    }
}

void sub_0800AF74(int x, int y)
{
    if (IsTerrainNotWaterOrRiver(x, y) == 0)
        SetTerrainAt(x, y, 1);

    SetTerrainAt(x, y, 3);
    MakeMountain(x, y);
    sub_0800A588(x, y);
    sub_0800ABD0(x, y);
    RepaintNeighbours(x, y);
    sub_0800BEE4(x, y);
    sub_0800EC20(x, y);
}

int sub_0800AFCC(int x, int y)
{
    switch (MAP->terrain[MAP->rowOffset[y] + x] & 0x1f)
    {
    case TERRAIN_MOUNTAIN:
    case TERRAIN_WOOD:
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_AIRPORT:
    case TERRAIN_PORT:
    case TERRAIN_BASE:
        return 1;
    }

    return 0;
}
