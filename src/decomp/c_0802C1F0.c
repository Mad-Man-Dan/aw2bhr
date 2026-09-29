#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C1F0.
 * sub_0802C1F0 @ 0x0802C1F0
 */

void CopyBytes(const u8 *src, u8 *dst, int size)
{
    while (size--)
        *dst++ = *src++;
}
asm(".global sub_0802C1F0\n.thumb_set sub_0802C1F0, CopyBytes\n");
