#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805BD40.
 * sub_0805BD40 @ 0x0805BD40
 */

int AiConsiderSecondCargoDropNeighbour(int x, int y, int t, int id, s16 *out)
{
    s8 *costs;
    int idx;
    int c;

    if (x < 0)
        return 0;
    if (y < 0)
        return 0;

    if (x >= gMap->width)
        return 0;
    if (y >= gMap->height)
        return 0;

    if (id == gUnknown_030045C8)
        return 0;

    idx = gMap->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
        return 0;

    costs = gUnknown_085D3DD0[1].power[0].movementChart[0];

    c = (gMap->terrain[idx] & 0x1f)
        + gUnknown_085D5ABC[t].movementType * 32;

    if (costs[c] != -1)
    {
        out[0] = x;
        out[1] = y;
        return 1;
    }

    return 0;
}
asm(".global sub_0805BD40\n.thumb_set sub_0805BD40, AiConsiderSecondCargoDropNeighbour\n");
