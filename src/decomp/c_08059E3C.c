#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059E3C.
 * sub_08059E3C @ 0x08059E3C
 */

#include "map.h"
struct Unk59E3CCell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

/* Collects every cell the current army may act on into the caller's array and
 * terminates it with v = 0xFFFF.  A cell survives four filters: the movement
 * map value must be non-negative, the terrain must be flagged in
 * gUnknown_085767D5, its owner bits must equal gUnknown_03004084, and the
 * +0x376A plane must be non-zero.
 *
 * FOUND BY THE PERMUTER (wave 61), seeded from the wave-77 draft that had got
 * to 4 differing bytes.  TWO ODDITIES BELOW ARE LOAD-BEARING -- each was
 * removed and the match re-checked with trymatch, and each removal lost it:
 *
 *   1. `zero` and the `+= zero` ARE A NO-OP and exist only to shape codegen.
 *      Deleting the pair loses the match.  The earlier draft carried the same
 *      trick as `gUnknown_03003340[y][x] += 0;` at the TOP of the inner loop;
 *      what the permuter changed is that it now sits INSIDE the filters, so it
 *      runs only for surviving cells.  Adding zero is a no-op either way, so
 *      the two spellings are equivalent in behaviour and differ only in the
 *      register allocation they produce.
 *
 *   2. The bare block around the four stores has to stay a block.  It reads as
 *      redundant braces; flattening it loses the match.
 */
void AiListThreatenedProperties(void *a1)
{
    struct Unk59E3CCell *out;
    int zero;
    int x;
    int y;
    int off;

    out = a1;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            zero = 0;

            if (((s8)gUnknown_03003340[y][x]) < 0)
                continue;

            off = gMap->rowOffset[y] + x;

            if (gUnknown_085767D5[gMap->terrain[off] & 0x1f] == 0)
                continue;

            if ((gMap->terrain[off] & 0xe0) != gUnknown_03004084)
                continue;

            if (gMap->unk376A[off] == 0)
                continue;

            do
            {
                out->x = x;
                out->y = y;
                out->v = (s8)gUnknown_03003340[y][x];
                gUnknown_03003340[y][x] += zero;
                out++;
            }
            while (0);
        }
    }

    out->v = 0xFFFF;
}
asm(".global sub_08059E3C\n.thumb_set sub_08059E3C, AiListThreatenedProperties\n");
