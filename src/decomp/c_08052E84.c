#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052E84.
 * sub_08052E84 @ 0x08052E84
 */

#include "hardware.h"

/* Mid-frame scroll: while VCOUNT is inside the visible area, park BG0 and BG1
 * horizontally at half the current scanline.
 *
 * The `u16` local is load-bearing. Its truncation is emitted as the bare
 * `lsl #16` before the compare, and combine folds the matching `lsr #16` into
 * the `>> 1` to give the `lsr #17` inside the branch. Declaring the local
 * `int` and casting at the shift instead keeps the value un-narrowed, turns
 * the unsigned `bhi` into a signed `bgt` and drops the shift pair entirely.
 *
 * The two register writes share one pool word and an `adds r0, #4`: agbcc CSEs
 * the second absolute address against the first.
 */
void BattleAnimHBlank_ScrollByScanline(void)
{
    u16 vcount = REG_VCOUNT & 0xff;

    if (vcount <= 0x9f)
    {
        REG_BG0HOFS = vcount >> 1;
        REG_BG1HOFS = vcount >> 1;
    }
}
asm(".global sub_08052E84\n.thumb_set sub_08052E84, BattleAnimHBlank_ScrollByScanline\n");
