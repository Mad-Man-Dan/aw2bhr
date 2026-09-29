#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012FA4.
 * sub_08012FA4 @ 0x08012FA4
 */

void ClearBytes2(u8 *dst, int size)
{
    while (size != 0)
    {
        *dst = 0;
        dst++;
        size--;
    }
}
asm(".global sub_08012FA4\n.thumb_set sub_08012FA4, ClearBytes2\n");
