#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B6E8.
 * sub_0808B6E8 @ 0x0808B6E8
 */

void *sub_0808B6E8(void *dst, const void *src, int size)
{
    u8 *d = dst;
    const u8 *s = src;

    while (size--)
        *d++ = *s++;

    return dst;
}
