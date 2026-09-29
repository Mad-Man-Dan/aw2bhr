#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08024058.
 * sub_08024058 @ 0x08024058
 */

void CapturePropertyAt(s16 a1, s16 a2)
{
    struct Map *map;
    int idx;
    u8 v;

    map = gMap;
    idx = map->rowOffset[a2] + a1;
    v = (map->terrain[idx] & 0x1f) + gUnknown_03004084;
    map->terrain[idx] = v;

    SetPropertyTileForOwner(a1, a2, v & 0xe0);
    RebuildMapUnitLayers2();
    RecountArmyProperties();
    RecountArmyIncome();
}
asm(".global sub_08024058\n.thumb_set sub_08024058, CapturePropertyAt\n");
