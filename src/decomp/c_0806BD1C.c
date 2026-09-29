#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806BD1C.
 * sub_0806BD1C @ 0x0806BD1C
 */

/* Renders a NUL-terminated byte string into two rows of a tilemap and returns
 * its pixel width (8 per glyph). Four punctuation codes map to fixed tiles
 * 0x7a..0x7d; anything else is the raw byte biased by 0x1f. The second row is
 * always the first row's tile + 0x20, i.e. the glyph's bottom half.
 *
 * `dst` is a plain `u16 *` and the second store is `dst[0x20]`, not a struct
 * member at +0x40 -- src/decomp/c_0806BD6C.c measured that discriminator on the
 * sibling function: through a member agbcc emits the VALUE's add before the
 * ADDRESS's, while `dst[0x20]` emits them in the ROM's order (`adds r1, #0x40`
 * then `adds r0, #0x20`). 0x20 * 2 == 0x40 is past `strh`'s 5-bit
 * displacement, which is why the add appears at all. */
int DrawCreditsHeadingText(u16 *dst, u8 *src)
{
    int width;
    int t;

    width = 0;

    while (src[0] != 0)
    {
        if (src[0] == 0x2d)
            t = 0x7a;
        else if (src[0] == 0x2e)
            t = 0x7b;
        else if (src[0] == 0x2a)
            t = 0x7c;
        else if (src[0] == 0x20)
            t = 0x7d;
        else
            t = src[0] + 0x1f;

        width += 8;
        src++;
        dst[0] = t;
        dst[0x20] = t + 0x20;
        dst++;
    }

    BG_EnableSyncBG0();
    return width;
}
asm(".global sub_0806BD1C\n.thumb_set sub_0806BD1C, DrawCreditsHeadingText\n");
