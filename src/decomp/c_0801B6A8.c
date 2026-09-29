#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B6A8.
 * sub_0801B6A8 @ 0x0801B6A8
 */

/* A 0xff-fill. The pointer is advanced in its own expression and the count is
 * the loop variable, which is why the guard is `cmp #0; beq` and the bottom
 * test `cmp #0; bne` rather than the reversed-counter shape. */
void FillBytesWithFF(u8 *dst, u32 size)
{
    while (size != 0)
    {
        *dst++ = 0xff;
        size--;
    }
}
asm(".global sub_0801B6A8\n.thumb_set sub_0801B6A8, FillBytesWithFF\n");
