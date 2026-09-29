#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073D1C.
 * sub_08073D1C @ 0x08073D1C
 */

/* Packs eight nibbles into one word, eight times: row `i` of the output is
 * built from src[0..7] and the cursor then steps `stride` rows of eight bytes.
 * The counter runs 7..0 with `bge`, which is GCC's reversal of an ascending
 * `for (i = 0; i < 8; i++)` -- init N-1 plus `bge` is the ascending form, not a
 * source count-down (init N plus `bgt` would be that).
 * `stm r5!, {r0}` is `*dst++ = v`; the `lsls r4, #3` inside the loop is
 * `stride * 8` recomputed each pass. */

void ApplyBitmapTile(u8 * src, u32 * dst, int stride)
{
    int i;

    for (i = 0; i < 8; i++)
    {
        *dst++ = (((((((src[7] << 4 | src[6]) << 4 | src[5]) << 4 | src[4]) << 4
            | src[3]) << 4 | src[2]) << 4 | src[1]) << 4) | src[0];

        src += stride * 8;
    }
}
asm(".global sub_08073D1C\n.thumb_set sub_08073D1C, ApplyBitmapTile\n");
