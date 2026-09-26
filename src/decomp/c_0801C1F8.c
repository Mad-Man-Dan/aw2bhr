#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C1F8.
 * sub_0801C1F8 @ 0x0801C1F8
 */

void sub_0801C1F8(void)
{
    struct Unk03000288 *base;
    struct Unk03000288 *p;
    u32 v;

    base = gUnknown_03000288;
    v = 0;
    p = base + 15;

    do
    {
        p->unk00 = v;
        p--;
    }
    while ((int)p >= (int)base);
}
