#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078758.
 * sub_08078758 @ 0x08078758, sub_08078770 @ 0x08078770
 */

void SetCoSelectGroupSwitchAll(void)
{
    u32 *base;
    u32 *p;
    u32 v;

    base = gUnknown_030059C0;
    v = 1;
    p = base + 4;

    do
        *p-- = v;
    while ((int)p >= (int)base);
}
asm(".global sub_08078758\n.thumb_set sub_08078758, SetCoSelectGroupSwitchAll\n");

void SetCoSelectGroupSwitchAllButFirst(void)
{
    int i;

    for (i = 0; i <= 4; i++) {
        if (i == 0)
            gUnknown_030059C0[i] = 0;
        else
            gUnknown_030059C0[i] = 1;
    }
}
asm(".global sub_08078770\n.thumb_set sub_08078770, SetCoSelectGroupSwitchAllButFirst\n");
