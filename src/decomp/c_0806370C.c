#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806370C.
 * sub_0806370C @ 0x0806370C
 */

void sub_0806370C(int a1, int a2)
{
    int n = a2 & 0x3FF;

    if (a1 < 0)
        RegisterFillZero16((void *)(0x06010000 + n * 32), 0x80);
    else
        RegisterDataMove(gUnknown_08614258[gUnknown_02028E40] + a1 * 128,
                     (void *)(0x06000000 + (n * 32 | 0x10000)), 0x80);
}
