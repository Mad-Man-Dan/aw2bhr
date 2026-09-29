#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073CB8.
 * sub_08073CB8 @ 0x08073CB8, sub_08073CF4 @ 0x08073CF4
 */

/* The outer level of the same blit chain: one pass per tile row, each
 * advancing the nibble source by a whole row of `width` tiles (0x40 bytes
 * each) and the destination by `width` tiles of 8 words. Both strides are
 * loop invariants, so LICM hoists them into the preheader and they need the
 * two high registers -- that is what the `mov r7, sb` save is for, not a
 * loop signal.
 *
 * As in ApplyBitmapLine, `width` is passed through unchanged while a separate
 * register counts the rows down. */

void ApplyBitmap(u8 * src, u32 * dst, int width, int height)
{
    int i;

    for (i = 0; i < height; i++)
    {
        ApplyBitmapLine(src, dst, width);

        src += width * 0x40;
        dst += width * 8;
    }
}
asm(".global sub_08073CB8\n.thumb_set sub_08073CB8, ApplyBitmap\n");

/* Blits one row of tiles: each pass hands the innermost packer the SAME
 * `width` it was given, then steps the nibble source 8 bytes (one packed
 * row) and the tile destination 8 words (0x20 bytes, one 8x8 tile).
 *
 * The third argument is the untouched parameter, not the loop counter: the
 * ROM keeps `width` in r7 across the whole loop and counts down a separate
 * r4 initialised from it. That is agbcc's reversal of an ascending
 * `for (i = 0; i < width; i++)` -- the `cmp #0; ble` guard ahead of the
 * loop is the source's signed `<` test, so this is a `for`, not a
 * `do/while`. */

void ApplyBitmapLine(u8 * src, u32 * dst, int width)
{
    int i;

    for (i = 0; i < width; i++)
    {
        ApplyBitmapTile(src, dst, width);

        src += 8;
        dst += 8;
    }
}
asm(".global sub_08073CF4\n.thumb_set sub_08073CF4, ApplyBitmapLine\n");
