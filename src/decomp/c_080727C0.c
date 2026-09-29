#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080727C0.
 * sub_080727C0 @ 0x080727C0, sub_0807286C @ 0x0807286C
 */

/* Blits tile `idx` of a w-by-h tile sheet into a 32-wide BG tilemap at
 * (x, y), adding `base` to every entry and clipping to the 32x32 screen
 * block; the sheet is packed 0x20/w tiles per row.
 *
 * `p` has to be a separate local seeded from `src` rather than a reassignment
 * of the parameter. agbcc emits parameter copies (and then, separately, the
 * promoted-mode conversions like the `u16 base` narrowing) ahead of the
 * function body, so a stack parameter that is only READ once stays in memory
 * and its `ldr` lands where the body uses it -- after the base conversion,
 * which is exactly where the ROM has it. Reassigning `src` makes it a
 * parameter copy instead, moving the `ldr` up two instructions and letting
 * the accumulation happen in a low register, which is 4 bytes short. */
void TmCopySheetCellClipped(u16 *map, int x, int y, u16 base, int w, int h, const u16 *src, int idx)
{
    const u16 *p = src;
    int ix;
    int iy;
    int tpr;
    int row;

    tpr = Div(0x20, w);
    row = Div(idx, tpr);
    p = p + w * DivRem(idx, tpr) + row * h * 0x20;

    for (iy = 0; iy < h; iy++)
        for (ix = 0; ix < w; ix++)
            if ((unsigned) (x + ix) < 0x20 && (unsigned) (y + iy) < 0x20)
                *(map + (x + ix) + (y + iy) * 0x20) = *(p + ix + iy * 0x20) + base;
}
asm(".global sub_080727C0\n.thumb_set sub_080727C0, TmCopySheetCellClipped\n");

/* Same blit as TmCopySheetCellClipped but vertically mirrored, and the sheet's real row
 * stride comes from a two-byte header: src[0] + 1 halfwords, pixel data from
 * src + 2. See TmCopySheetCellClipped for why `p` is a separate local seeded from the
 * parameter rather than a reassignment of it.
 *
 * The two odd-looking constructs are the whole difference between 97.3% and a
 * match, and both were found by decomp-permuter after the hand version had
 * the right size and the right instructions in the right order:
 *  - the `do { } while (0)` around the pointer bump and the two divisions.
 *    Without it `x` and `stride` swap the callee-saved r9/sl pair: `stride`
 *    has to win r9, which means it has to outrank `x` on
 *    n_refs/live_length, and the block shortens its live range enough to do
 *    that.
 *  - binding `h - iy` to its own local inside the `if`. Inline,
 *    `(h - iy - 1) * stride` is folded before the multiply; bound, the
 *    subtraction of 1 stays on the near side of it, which is the ROM's
 *    `subs r0, r1, r5; subs r0, #1; muls r2, r0`. */
void TmCopyTsaSheetCellRowsReversedClipped(u16 *map, int x, int y, u16 base, int w, int h, const u16 *src, int idx)
{
    const u16 *p = src;
    int ix;
    int iy;
    int stride;
    int rows;
    int tpr;
    int row;

    stride = *(const u8 *) p + 1;

    do
    {
        p++;
        tpr = Div(stride, w);
        row = Div(idx, tpr);
    } while (0);

    p = p + w * (idx - tpr * row) + row * h * 0x20;

    for (iy = 0; iy < h; iy++)
    {
        for (ix = 0; ix < w; ix++)
        {
            if ((unsigned) (x + ix) < 0x20 && (unsigned) (y + iy) < 0x20)
            {
                rows = h - iy;
                *(map + (x + ix) + (y + iy) * 0x20) =
                    *(p + ix + (rows - 1) * stride) + base;
            }
        }
    }
}
asm(".global sub_0807286C\n.thumb_set sub_0807286C, TmCopyTsaSheetCellRowsReversedClipped\n");
