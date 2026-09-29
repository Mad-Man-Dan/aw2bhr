#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072680.
 * sub_08072680 @ 0x08072680
 */

/* Fills a w-by-h rectangle of a 32-wide BG tilemap with consecutively
 * numbered tiles, clipping to the 32x32 screen block. `tile` is u16 and wraps
 * (`adds #1; lsls #0x10; lsrs #0x10`) and it is bumped on EVERY inner
 * iteration, including the clipped ones, which is why it belongs in the `for`
 * increment list rather than in the body: put in the body it is emitted
 * before the ix/pointer increments, and the ROM emits it after them.
 *
 * The two clip tests are unsigned so one compare covers both ends of the
 * range -- `cmp #0x1f; bhi` is `(unsigned)v <= 0x1f`. */
void TmPutSequentialTilesClipped(u16 *map, int x, int y, u16 tile, int w, int h)
{
    int ix;
    int iy;

    for (iy = y; iy < y + h; iy++)
        for (ix = x; ix < x + w; ix++, tile++)
            if ((unsigned) ix < 0x20 && (unsigned) iy < 0x20)
                map[iy * 0x20 + ix] = tile;
}
asm(".global sub_08072680\n.thumb_set sub_08072680, TmPutSequentialTilesClipped\n");
