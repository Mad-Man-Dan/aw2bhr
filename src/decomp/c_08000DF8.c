#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08000DF8.
 * sub_08000DF8 @ 0x08000DF8
 */

/*
 * sub_08000DF8 -- reset the map view, and generate a fresh map when a1 is 0.
 *
 * Clears gUnknown_030032D8 and calls LoadTileTerrainTable. When a1 is 0 it also zeroes
 * the map's scroll and camera position and calls GenerateRandomMap, so a
 * non-zero a1 keeps the map that is already loaded. Either way it then moves
 * both cursors (gUnknown_030033E4, gUnknown_030033E0) to 0,0 and calls
 * InitCursorInfoPanelPosition.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - The six clears go through one local `map` pointer. Written as
 *     `gMap->field = 0` six times, the compiler has to assume a store could
 *     change gMap itself and reloads the pointer before every store.
 */

void sub_08000DF8(int a1)
{
    struct Map *map;

    gUnknown_030032D8 = 0;
    LoadTileTerrainTable();

    if (a1 == 0)
    {
        map = gMap;
        map->scrollX = 0;
        map->scrollY = 0;
        map->unk08 = 0;
        map->unk0a = 0;
        map->camX = 0;
        map->camY = 0;
        GenerateRandomMap();
    }

    gUnknown_030033E4.unk00 = 0;
    gUnknown_030033E4.unk02 = 0;
    gUnknown_030033E0.unk00 = 0;
    gUnknown_030033E0.unk02 = 0;
    InitCursorInfoPanelPosition();
}
