#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077140.
 * sub_08077140 @ 0x08077140, sub_08077180 @ 0x08077180
 */

/* A 6x6 tilemap fill into a 32-wide map: the store index steps by 32 per row
 * while the tile number steps by 6, so the two strides in the one statement
 * are real. Both counters are u8 and both tests are unsigned (`bls`), which is
 * what forces the `lsls #0x18 / lsrs #0x18` re-narrowing each iteration.
 * The tile value is computed into a temp BEFORE the store address -- authoring
 * the store as one statement emits the address first and swaps the two givs.
 * PutTileBlock6x4 is this function with a 4-row outer bound. */
void PutTileBlock6x6(u16 *dest, u16 base, int pal)
{
    u8 i, j;
    int val;

    for (i = 0; i < 6; i++)
        for (j = 0; j < 6; j++)
        {
            val = base + j + i * 6 + (pal << 12);
            dest[i * 32 + j] = val;
        }
}
asm(".global sub_08077140\n.thumb_set sub_08077140, PutTileBlock6x6\n");

/* PutTileBlock6x6 with a 4-row outer bound; identical otherwise. */
void PutTileBlock6x4(u16 *dest, u16 base, int pal)
{
    u8 i, j;
    int val;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 6; j++)
        {
            val = base + j + i * 6 + (pal << 12);
            dest[i * 32 + j] = val;
        }
}
asm(".global sub_08077180\n.thumb_set sub_08077180, PutTileBlock6x4\n");
