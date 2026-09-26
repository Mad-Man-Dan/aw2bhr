#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078740.
 * sub_08078740 @ 0x08078740
 */

void sub_08078740(void)
{
    u32 *base;
    u32 *p;
    u32 v;

    base = gUnknown_030059C0;
    v = 0;
    p = base + 4;

    do
        *p-- = v;
    while ((int)p >= (int)base);
}
