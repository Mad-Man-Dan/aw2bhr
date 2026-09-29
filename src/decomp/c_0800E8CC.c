#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800E8CC.
 * sub_0800E8CC @ 0x0800E8CC
 */

#include "map.h"

/* Draft: five-cell vertical wood-neighbour mask. */
int GetForestColumnMask(int x, int y)
{
    int mask = 0;

    if (y > 2 && gMap->terrain[gMap->rowOffset[y - 2] + x] == 4)
        mask |= sub_0800119C(x, y - 2, 4) << 4;
    if (y > 1 && gMap->terrain[gMap->rowOffset[y - 1] + x] == 4)
        mask |= sub_0800119C(x, y - 1, 4) << 3;
    if (gMap->terrain[gMap->rowOffset[y] + x] == 4)
        mask |= sub_0800119C(x, y, 4) << 2;
    if (y < gMap->height - 1 && gMap->terrain[gMap->rowOffset[y + 1] + x] == 4)
        mask |= sub_0800119C(x, y + 1, 4) << 1;
    if (y < gMap->height - 2 && gMap->terrain[gMap->rowOffset[y + 2] + x] == 4)
        mask |= sub_0800119C(x, y + 2, 4);
    return mask;
}
asm(".global sub_0800E8CC\n.thumb_set sub_0800E8CC, GetForestColumnMask\n");
