#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080080F8.
 * RepaintTile @ 0x080080F8
 */

/* Repaints one cell through four independent terrain tests, each a
 * IsTerrainAtCoordsType query on the same (x, y). The first re-reads the cell's own
 * tile through gMap->tile and, when it is one of the two bridge ids, clears it
 * unless the cell below is joinable.
 *
 * GetShoalTile returns s16 -- the `lsls #0x10 / asrs #0x10` before the sign
 * test is agbcc re-narrowing a narrow-returning callee, and the narrowed value
 * in r2 is then handed straight to MakeTileSimple as its third argument.
 *
 * Permuter-derived, so byte-exact but not claimed to be the original source:
 * the cell's tile has to land in r2 rather than r0, which the `v` local plus
 * the split-out sub_080016D0 call achieves. */
void RepaintTile(int x, int y)
{
    s16 height;

    if (IsTerrainAtCoordsType(x, y, 1))
    {
        int off;
        int v;

        off = gMap->rowOffset[y] + x;
        v = gMap->tile[off];

        if (v == 0x43 || v == 3)
        {
            if (!IsTerrainAtCoordsType(x, y + 1, 3))
                MakeTileSimple(x, y, 1);
        }

        v = sub_080016D0(x, y);
        MakeTileSimple(x, y, v);
    }

    if (IsTerrainAtCoordsType(x, y, 3))
        MakeMountain(x, y);

    if (IsTerrainAtCoordsType(x, y, 5))
    {
        MakeTileSimple(x, y, GetRoadTile(x, y));
        MakeTileSimple(x, y, sub_080016D0(x, y));
    }

    if (IsTerrainAtCoordsType(x, y, 0xd))
    {
        s16 w;

        height = GetShoalTile(x, y);
        w = height;

        if (w < 0)
            MakeSeaSafest(x, y);
        else
            MakeTileSimple(x, y, w);
    }
}

asm(".global sub_080080F8\n.thumb_set sub_080080F8, RepaintTile\n");
