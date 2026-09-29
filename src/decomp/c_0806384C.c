#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806384C.
 * sub_0806384C @ 0x0806384C
 */

/* A rectangular halfword blit into a 32-wide tilemap. `src` points at a header
 * whose low byte is the row width; the halfwords follow it, and each is biased
 * by `add` on the way out.
 *
 * Two things settle the register allocation, and wave 46 measured both:
 *  - The ROM reuses the `dst` PARAMETER as the row cursor. That is what forces
 *    the `adds r4, r0, #0` prologue copy: dst is then one pseudo live across
 *    the whole outer loop, so it takes a callee-saved register instead of
 *    staying in r0. A separate `row` local leaves dst in r0 (86 bytes, no
 *    prologue copy) and a `dst += x` in-place update coalesces base into dst.
 *  - `j` must be DECLARED BEFORE `i`. Declaration order sets the pseudo
 *    numbers, and the allocno tie-break follows them: with `i` first the two
 *    counters land in r3/r1, with `j` first they land in r1/r3 as the ROM has
 *    them. Same instruction stream either way. */
void UnusedBlitTilemapRectWithBias(u16 *dst, const u16 *src, u16 x, u16 y, u16 h, u16 add)
{
    u16 *base;
    u16 j;
    u16 i;
    u32 w;

    w = *(const u8 *)src;
    base = (dst + x) + y * 32;
    src++;

    for (i = 0; i < h; i++)
    {
        dst = base + i * 32;

        for (j = 0; j < w; j++)
        {
            *dst = add + *src;
            src++;
            dst++;
        }
    }
}
asm(".global sub_0806384C\n.thumb_set sub_0806384C, UnusedBlitTilemapRectWithBias\n");
