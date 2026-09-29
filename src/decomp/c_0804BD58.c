#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BD58.
 * sub_0804BD58 @ 0x0804BD58
 */

void BuildHitFlashPalette(volatile u16 *src, void *dst)
{
    int i;
    int r;
    volatile u16 **q;
    int g;
    int b;

    for (i = 0; i < 0x40; i++)
    {
        q = &src;
        r = ((((*q)[i] & 0x1f) + 0x1f) >> 1) & 0x1f;
        g = (((src[i] & 0x3e0) + 0x3e0) >> 1) & 0x3e0;
        b = (((src[i] & 0x7c00) + 0x7c00) >> 1) & 0x7c00;
        ((u16 *)((u8 *)gUnknown_02028E5C + 8))[i] = (r + g) + b;
    }

    CpuFastSet((u16 *)((u8 *)gUnknown_02028E5C + 8), dst, 0x10);
}
asm(".global sub_0804BD58\n.thumb_set sub_0804BD58, BuildHitFlashPalette\n");
