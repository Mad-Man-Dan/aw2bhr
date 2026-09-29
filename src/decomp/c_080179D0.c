#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080179D0.
 * sub_080179D0 @ 0x080179D0
 */

/* Clears a 23x4 halfword window of a 32-wide tilemap.  Both counters are s16
 * -- each carries the `lsl #16; asr #16` round trip, and the outer one is kept
 * pre-shifted so its increment is `+0x10000` and its scale falls out of
 * `asr #0xa` (i.e. y * 64 bytes).
 *
 * The additive `dst + x + y * 32` form is what gives two independently scaled
 * terms (`lsl #1` for x, the asr-derived y*64); `&dst[x + y * 32]` would
 * combine them into one scaled index. */
void ClearTilemapRect23x4(u16 *dst)
{
    s16 x;
    s16 y;

    for (y = 0; y <= 3; y++)
        for (x = 0; x <= 0x16; x++)
            *(dst + x + y * 32) = 0;
}
asm(".global sub_080179D0\n.thumb_set sub_080179D0, ClearTilemapRect23x4\n");
