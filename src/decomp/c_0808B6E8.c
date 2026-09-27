#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B6E8.
 * memcpy @ 0x0808B6E8
 *
 * The game's own memcpy, and the one agbcc's struct-assignment block moves
 * call (`bl memcpy`). The size is `unsigned long`, agbcc's builtin size
 * type: `int`, `unsigned int` and the headers' `size_t` all fail with
 * "conflicting types for built-in function `memcpy'". The old sub_0808B6E8 symbol is
 * kept as a linker alias below so every other unit keeps resolving it.
 */

void *memcpy(void *dst, const void *src, unsigned long size)
{
    u8 *d = dst;
    const u8 *s = src;

    while (size--)
        *d++ = *s++;

    return dst;
}

asm(".global sub_0808B6E8\n.thumb_set sub_0808B6E8, memcpy\n");
