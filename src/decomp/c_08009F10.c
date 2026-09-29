#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009F10.
 * MakeRiver @ 0x08009F10
 */

#define MAP gMap

int MakeRiver(int x, int y)
{
    int a;
    int b;

    a = sub_0800A6AC(x, y);
    if (a < 0)
    {
fail:
        return 0;
    }

    if (a != 0)
    {
        b = sub_0800A95C(x, y);
        if (b != 0)
        {
            if (b <= 0)
                goto fail;
            if ((a & 0xfe00) == 0x4000)
            {
                SetTerrainAt(x, y, 2);
                MakeTileSimple(x, y, b);
            }
            if ((a & 0xfe00) == 0x2000)
            {
                SetTerrainAt(x, y, 2);
                MakeTileSimple(x, y, b);
            }
            if ((a & 0xfe00) == 0)
            {
                SetTerrainAt(x, y, 2);
                MakeTileSimple(x, y, b);
            }
            goto placed;
        }
    }
    else
    {
        if (sub_0800A6AC(x, y) < 0)
            goto fail;
        a = sub_0800A884(x, y);
        if (a == 0)
            goto fail;
        b = sub_0800A95C(x, y);
        if (b != 0)
            a = b;
        if (a <= 0)
            goto fail;
    }

    SetTerrainAt(x, y, 2);
    MakeTileSimple(x, y, a);

placed:
    sub_0800A588(x, y);
    if (CountRiverNeighbours(x, y))
        sub_0800A098(x, y);

    if (y > 0)
    {
        int ny = y - 1;
        if (IsTerrainAtCoordsType(x, ny, 1))
        {
            MakeTileSimple(x, ny, 1);
            MakeForestSimple(x, ny);
            if (MAP->terrain[MAP->rowOffset[y] + x] == 3)
                sub_0800AF74(x, y);
        }
    }
    if (y < MAP->height - 1)
    {
        int ny = y + 1;
        if (IsTerrainAtCoordsType(x, ny, 1))
            MakeTile2(x, ny, 1);
    }

    RepaintNeighbours(x, y);
    sub_0800EC20(x, y);
    return 1;
}

asm(".global sub_08009F10\n.thumb_set sub_08009F10, MakeRiver\n");
