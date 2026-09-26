#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011AAC.
 * sub_08011AAC @ 0x08011AAC
 */

void sub_08011AAC(void *a1)
{
    void **p;
    s16 i;

    if (gUnknown_030030E8 == 0x10)
        return;

    p = gUnknown_03002FA0;
    i = gUnknown_030030E8++;
    p[i] = a1;
}
