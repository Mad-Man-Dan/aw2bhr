#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800C124.
 * sub_0800C124 @ 0x0800C124
 */

/* Exact configured match. Repaints the cell at (x, y) after a change: if it is
 * a road tile, record its tile in gActiveMap->cursorTerrain and repaint; else
 * switch on its terrain byte. All map reads go straight through
 * gMap->tile[] / gMap->terrain[] / gMap->rowOffset[]; `q` is bound once in the
 * true arm. */
void sub_0800C124(int x, int y)
{
    struct ActiveMap *q;

    if (IsTerrainNotWater(x, y))
    {
        if (sub_08008C34(x, y))
            return;
        if (GetPropertyKindAt(x, y))
            RemovePropertyAt(x, y);
        q = gActiveMap;
        q->cursorTerrain = gMap->tile[gMap->rowOffset[y] + x];
        sub_0800EC20(x, y);
        MakeTileSimple(x, y, 0x2A);
        SetTerrainAt(x, y, 7);
        RepaintNeighbours(x, y);
    }
    else
    {
        switch (gMap->terrain[gMap->rowOffset[y] + x])
        {
        case TERRAIN_SEA:
            MakeSeaSafest(x, y);
            if (gMap->tile[gMap->rowOffset[y] + x] != 0x2A)
            {
                MakeTileSimple(x, y, 0x2A);
                SetTerrainAt(x, y, 7);
            }
            break;

        case TERRAIN_SHOAL:
            break;

        case TERRAIN_REEF:
            break;
        }
    }
    EnsureValidTile(x, y);
}
