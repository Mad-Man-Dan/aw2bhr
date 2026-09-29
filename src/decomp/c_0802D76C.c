#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802D76C.
 * sub_0802D76C @ 0x0802D76C
 */

/* Blanks columns 1..15 of rows 5..18 of the 32-wide tilemap gBG0TilemapBuffer
 * points at, then hands the buffer to BG_EnableSyncBG0.
 *
 * Both counters are s16, which is what puts them in the HIGH half of a register
 * for the whole loop: `lsls r4,r2,#0x10` / `asrs r3,r4,#0xb` is `y * 32` folded
 * into y's own sign extension, and the outer step is `+ (0x80 << 9)`, i.e. one
 * in the high half. Do not author any of that -- write `y * 32 + x` and let
 * combine fold it. `int` counters give plain adds and a different loop.
 *
 * The tilemap pointer is re-`ldr`ed inside the inner loop because the store
 * through it may alias the pointer variable; that is free, not a spelling. */
void ClearDeploymentListArea(void)
{
    s16 x;
    s16 y;

    for (y = 5; y <= 0x12; y++)
    {
        for (x = 1; x <= 0xf; x++)
            gBG0TilemapBuffer[y * 32 + x] = 0;
    }

    BG_EnableSyncBG0();
}
asm(".global sub_0802D76C\n.thumb_set sub_0802D76C, ClearDeploymentListArea\n");
