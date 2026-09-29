#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08009310.
 * sub_08009310 @ 0x08009310
 */

#include "map.h"

/* Whether the cell at (x, y) can join an adjacent bridge or coast tile.
 * Terrain 0xC counts as a join on its own, and so does any horizontal or
 * vertical neighbour of that terrain.
 *
 * The shared success/fail labels are load-bearing: plain returns give
 * each exit its own tail and the function comes out 8 bytes short.
 * `n` holds both sub_08008D70's result and the vertical-neighbour flag,
 * which is what puts both in r6. The right neighbour needs its own
 * index local, not `left` reused, to get the ROM's `ldrh r1`. */
int CanPlaceBridgeAt(int x, int y)
{
    int terrain = gMap->terrain[gMap->rowOffset[y] + x];
    int direction;
    int n;
    int left;
    int right;

    if (terrain == 0xC)
        goto success;

    if (terrain == 7 || terrain == 0xD || terrain == 0x13)
    {
        switch (GetLandNeighbourMask(x, y))
        {
        case 1: case 2: case 4: case 8:
            sub_08008CB8(x, y);
            goto success;
        case 6: case 7: case 9: case 11: case 13: case 14:
            goto success;
        case 3: case 5: case 10: case 12:
            goto fail;
        default:
            n = sub_08008D70(x, y);
            if (n > 0) goto success;
            goto fail;
        }
    }

    if (!IsPlainRiverAt(x, y))
        goto fail;

    n = 0;
    switch (sub_08008CB8(x, y))
    {
    case 0: case 8: case 9:
        direction = GetLandNeighbourMask(x, y);
        if (direction & 6)
            goto success;
        if (x > 0 && (left = gMap->rowOffset[y] - 1, gMap->terrain[left + x]) == 0xC)
            goto success;
        if (x >= gMap->width - 1)
            break;
        if ((right = gMap->rowOffset[y] + 1, gMap->terrain[right + x]) != 0xC)
            break;
        goto success;
    case 2: case 4: case 6:
        direction = GetLandNeighbourMask(x, y);
        if (direction & 9)
            goto success;
        if ((y > 0 && gMap->terrain[gMap->rowOffset[y - 1] + x] == 0xC)
            || (y < gMap->height - 1 &&
            gMap->terrain[gMap->rowOffset[y + 1] + x] == 0xC))
            n = 0x16;
        break;
    }
    if (n <= 0) goto fail;
    goto success;
success: return 1;
fail: return 0;
}
asm(".global sub_08009310\n.thumb_set sub_08009310, CanPlaceBridgeAt\n");
