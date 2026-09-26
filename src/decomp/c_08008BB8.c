#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08008BB8.
 * EnsureValidTile @ 0x08008BB8
 *
 * Named per aw2bhr-main's src/design.c/design.h ("EnsureValidTile"), called
 * from SetTerrainAt (src/decomp/c_080011F4.c) whenever a cell is set to
 * TERRAIN_SEA. The old sub_XXXXXXXX symbol is kept as a linker alias below
 * so every other unit keeps resolving it unchanged.
 */

void EnsureValidTile(int x, int y)
{
    int v;

    v = GetUnitTypeAt(x, y);

    if (v > 0)
    {
        s8 *costs;
        int idx;
        int c;

        /* The movement chart: the cost of entering each terrain, 32 entries per
         * movement type, so the index is terrain + movementType * 32. A cost of
         * -1 means the unit cannot be there. */
        costs = gUnknown_085D3DD0[1].power[0].movementChart[0];

        idx = gMap->rowOffset[y] + x;
        c = (gMap->terrain[idx] & 0x1f) + gUnknown_085D5ABC[v & 0x3f].movementType * 32;

        if (costs[c] == -1)
            RemoveUnitAt(0, x, y);
    }
}

asm(".global sub_08008BB8\n.thumb_set sub_08008BB8, EnsureValidTile\n");
