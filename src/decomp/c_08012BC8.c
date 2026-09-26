#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012BC8.
 * sub_08012BC8 @ 0x08012BC8
 */

void sub_08012BC8(u16 *dst, u16 x, u16 y, u16 width, u16 height, u16 value)
{
    u16 i;
    u16 j;
    u16 offset;
    u16 *base;
    u16 *p;

    offset = 0;
    base = (dst + x) + y * 32;

    for (i = 0; i < height; i++)
    {
        p = base + offset;

        for (j = 0; j < width; j++)
        {
            *p = value;
            p++;
        }

        offset += 0x20;
    }
}
