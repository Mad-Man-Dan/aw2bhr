#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037A20.
 * sub_08037A20 @ 0x08037A20
 */

/* Fills a 32-entry-per-row tilemap with consecutive tile ids, covering the
 * map's extent rounded up to whole 2x2-tile blocks.
 *
 * The outer counter is a PIXEL row: it steps 16 (one block) and its bound is
 * `hh * 8`, the map height in pixels. Written as a block-row counter
 * (`y < hh >> 1`, `y++`) agbcc emits `lsl #6` for the row address and steps by
 * 1; the ROM's `lsls r0, r1, #2` with a step of 16 is the pixel form, where
 * `dst[y * 2 + x]` recovers the same row offset from a counter 16x larger.
 *
 * `base` is `int`, NOT u16 -- it is copied with a bare `adds r6, r1, #0` and
 * never narrowed, where a u16 parameter would arrive with PROMOTE_MODE's
 * `lsls #0x10; lsrs #0x10`.
 *
 * The counter is incremented in its OWN statement rather than as `base + k++`
 * in the store: the post-increment makes agbcc evaluate k first and emit
 * `adds r0, r4, r6`, where the ROM has the operands the other way round.
 * That was the last differing byte.
 *
 * `ww >> 1` is a shift, not `/ 2`: a signed divide adds the round-toward-zero
 * correction and the ROM has a bare `asrs`. */
void FillMapPreviewTilemap(u16 *dst, int base)
{
    int ww;
    int hh;
    int x;
    int y;
    int k;

    ww = (((u8 *)gUnknown_03003F68)[0] + 1) & ~1;
    hh = (((u8 *)gUnknown_03003F68)[1] + 1) & ~1;
    k = 0;
    for (y = 0; y < hh * 8; y += 16)
    {
        for (x = 0; x < ww >> 1; x++)
        {
            dst[y * 2 + x] = base + k;
            k++;
        }
    }
}
asm(".global sub_08037A20\n.thumb_set sub_08037A20, FillMapPreviewTilemap\n");
