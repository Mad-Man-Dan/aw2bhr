#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015410.
 * sub_08015410 @ 0x08015410
 */

/* A pure forwarder to StartSlotScriptWithSprite that SWAPS ARGUMENTS 3 AND 4 -- the `adds
 * r4,r2,#0 / adds r2,r3,#0 / adds r3,r4,#0` shuffle is the whole body. The
 * fifth argument arrives on the stack and goes back out on the stack.
 */
s8 sub_08015410(void *a, u8 b, void *c, void *d, u8 e)
{
    return StartSlotScriptWithSprite(a, b, d, c, e);
}
