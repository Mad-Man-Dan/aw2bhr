#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B690.
 * sub_0801B690 @ 0x0801B690
 */

/* A byte copy, written like its neighbour FillBytesWithFF's 0xff-fill: the
 * pointers advance inside the store expression and the count is the loop
 * variable, which gives the duplicated `cmp #0; beq` entry guard rather than
 * the reversed-counter shape. */
void CopyBytes2(u8 *src, u8 *dst, u32 size)
{
    while (size != 0)
    {
        *dst++ = *src++;
        size--;
    }
}
asm(".global sub_0801B690\n.thumb_set sub_0801B690, CopyBytes2\n");
