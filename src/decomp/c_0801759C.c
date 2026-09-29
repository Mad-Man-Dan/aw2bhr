#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801759C.
 * sub_0801759C @ 0x0801759C
 */

/* Rebuilds the whole terrain plane of the gUnknown_08499590 map from its tile
 * plane, then kicks the three subsystems that cache it and re-selects the
 * current cursor target.
 *
 * Per cell: `idx = rowOffset[y] + x` -- the same 0x417A row-offset table the
 * rest of this map's readers use -- then
 * `terrain[idx] = gUnknown_0849959C[tile[idx]]`, i.e. the u16 tile id at
 * +0x0A22 indexes a global byte table and the result lands in the +0x1432
 * byte plane at the SAME index.
 *
 * The local struct cast is the wave-34 spelling recorded on gUnknown_08499590
 * in include/unknown-globals.h: only a COMPONENT_REF keeps the ROM's
 * `(map + K) + idx` association: plain `gUnknown_08499590[0x1432 + idx]` folds
 * K into the load displacement, and `*(u16 *)(gUnknown_08499590 + 0x417A +
 * y * 2)` reassociates the constant outward past the variable. The tag is
 * file-local and its 0x0A22 member splits what c_08041EA8.c's layout calls
 * `unit`; both spellings describe the same bytes and rescale nothing.
 *
 * Both counters are plain `int`: neither the increment nor the exit test
 * carries a narrowing, and both bounds are `ldrh`ed and compared SIGNED. */

void RebuildTerrainPlaneFromTiles(void)
{
    int x;
    int y;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            int idx;

            idx = gMap->rowOffset[y] + x;
            gMap->terrain[idx] =
                gUnknown_0849959C[gMap->tile[idx]];
        }
    }

    RecountPropertiesIncomeAndAiFacilities();
    sub_08023348();
    RebuildMapUnitLayers2();
    ApplyArmyWindowFramePalette(gUnknown_030033EC);
}
asm(".global sub_0801759C\n.thumb_set sub_0801759C, RebuildTerrainPlaneFromTiles\n");
