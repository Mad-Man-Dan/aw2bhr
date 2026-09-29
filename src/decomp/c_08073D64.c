#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073D64.
 * sub_08073D64 @ 0x08073D64
 */

/* Fills a `width` x `height` rectangle of a 0x20-halfword-wide tilemap with
 * consecutive tile references. `tileref` runs across the WHOLE rectangle,
 * not per row, so it is one post-increment inside the inner loop.
 *
 * `adds r2, r0, #1` at the top of the outer body and `adds r0, r2, #0` at
 * the bottom are one variable, not two: agbcc sinks the outer counter's
 * increment past the inner loop because the row base is computed from the
 * pre-increment value. */

void PutAppliedBitmap(u16 * tm, int tileref, int width, int height)
{
    int ix, iy;

    for (iy = 0; iy < height; iy++)
        for (ix = 0; ix < width; ix++)
            tm[iy * 0x20 + ix] = tileref++;
}
asm(".global sub_08073D64\n.thumb_set sub_08073D64, PutAppliedBitmap\n");
