#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059A0C.
 * sub_08059A0C @ 0x08059A0C
 */

/* The c_0804151C.c list builder with a different predicate: walks every map
 * cell, and for each passable one whose terrain code is accepted by
 * gUnknown_085767D5 and whose unit id IsTerrainOwnedByUnitsTeam does not reject against
 * gUnknown_03003F38, appends {x, y, terrain} to the caller's buffer. Terminated
 * by a 0xFFFF value halfword; returns the element count as a pointer
 * difference against the buffer's START, which is kept in sl.
 *
 * The start pointer is a SECOND local copied from `out`, not a second read of
 * the parameter: the ROM does `adds r4,r0,#0` then `mov sl,r4`, where
 * `start = (struct Unk59A0CCell *)a1` alongside `out` gives `mov sl,r0` then
 * `mov r4,sl` -- the two instructions in the other order. That was the entire
 * residual at 98.7%.
 *
 * Cell addressing is BuildBoardableTransportList's idiom -- p, then t, then rows, then off,
 * then cells, each its own local -- which is what keeps 0x417A and 0x1432 in
 * pool words rather than folding either into a load displacement. The offset
 * here is 0x1432, not BuildBoardableTransportList's 0x12.
 *
 * `cells[off]` is written TWICE rather than bound to a local: the ROM loads the
 * byte twice off one CSE'd address (`ldrb r1,[r2]` before the table lookup and
 * again as the second argument), which is the discriminator against
 * BuildBoardableTransportList's `cell` local.
 */

struct Unk59A0CCell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};

int AiListEnemyPropertyCells(void *a1)
{
    struct Unk59A0CCell *out;
    struct Unk59A0CCell *start;
    int off;
    int x;
    int y;

    out = (struct Unk59A0CCell *)a1;
    start = out;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if ((s8)gUnknown_03003340[y][x] >= 0)
            {
                off = gMap->rowOffset[y] + x;
                if (gUnknown_085767D5[gMap->terrain[off] & 0x1f] != 0
                    && IsTerrainOwnedByUnitsTeam(gUnknown_03003F38, gMap->terrain[off]) != 1)
                {
                    out->x = x;
                    out->y = y;
                    out->v = (s8)gUnknown_03003340[y][x];
                    out++;
                }
            }
        }
    }

    out->v = 0xFFFF;
    return out - start;
}
asm(".global sub_08059A0C\n.thumb_set sub_08059A0C, AiListEnemyPropertyCells\n");
