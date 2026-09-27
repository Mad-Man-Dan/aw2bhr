#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D558.
 * CompactMapArmies @ 0x0803D558
 *
 * Not a Xenesis-documented name. The old sub_0803D558 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

#include "map.h"

/* Scans the map for HQ tiles (terrain type 8 in the low five bits; the army
 * that owns a tile is in the top three bits) and marks each army that has
 * one. It gives up once it has seen four HQs. Otherwise it lists the armies
 * present in order from slot 1, gives every tile of those armies owner bits
 * 0x20, and hands the list to gPlayers[].teamColor.
 *
 * Measured spelling notes (parked since wave 55 at 352/352 94.3% or 95.5%/+4):
 * - Both scans read the map through gMap's members. The struct form gives the
 *   ROM's (base + 0x417A) + y*2 grouping. The draft also had one scan through
 *   `(struct Map *)gUnknown_08499590`, which is the same address but a second
 *   pool word.
 * - The recolour step is split into statements: `type = terrain & 0x1f;` then
 *   `owner = terrain >> 5;`, a `bits = ownerBits;` pointer and the cell index
 *   in `i`. Written as one expression, the second loop pass sees a short
 *   enough inner loop (24 real insns, threshold ~26 with -dL) that it hoists
 *   the 0x417A constant: +4 bytes. The split keeps the ROM's in-loop constant.
 *   The mask before the shift is what orders `movs #0x1f` ahead of `lsrs`.
 * - `i = x;` before the scan-1 read is load-bearing (90.1% without it).
 */
void CompactMapArmies(void)
{
    u8 armies[8];
    u8 ownerBits[8];
    int i;
    int x;
    int y;
    u8 hqCount;
    u8 n;
    u8 terrain;
    u8 owner;
    u8 type;
    u8 *bits;

    for (i = 0; i <= 4; i++)
    {
        armies[i] = 0;
        ownerBits[i] = 0;
    }

    hqCount = 0;
    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            i = x;
            terrain = gMap->terrain[gMap->rowOffset[y] + i];
            if ((terrain & 0x1f) == 8)
            {
                armies[terrain >> 5] = 1;
                if (++hqCount == 4)
                    return;
            }
        }
    }

    n = 1;
    for (i = 0; i <= 4; i++)
    {
        if (armies[i] != 0)
        {
            armies[i] = 0;
            armies[n++] = i;
        }
    }

    for (i = 0; i <= 4; i++)
    {
        if (armies[i] != 0)
            ownerBits[armies[i]] = 0x20;
    }

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            terrain = gMap->terrain[gMap->rowOffset[y] + x];
            type = terrain & 0x1f;
            owner = terrain >> 5;
            bits = ownerBits;
            i = gMap->rowOffset[y] + x;
            gMap->terrain[i] = type + bits[owner];
        }
    }

    for (i = 0; i <= 4; i++)
        gPlayers[i].teamColor = armies[i];
}

asm(".global sub_0803D558\n.thumb_set sub_0803D558, CompactMapArmies\n");
