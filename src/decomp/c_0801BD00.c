#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BD00.
 * sub_0801BD00 @ 0x0801BD00, sub_0801BDB4 @ 0x0801BDB4
 */

void PutOamHi(s32 a1, s32 a2, void *a3, s32 a4)
{
    u16 *dst;
    s16 n;

    n = *(u16 *)a3;
    a3 = (u16 *)a3 + 1;
    dst = gUnknown_03002F2C;

    while (n != 0)
    {
        *dst++ = ((a2 | ((u16 *)a3)[0]) & ~0xff)
               | ((((u16 *)a3)[0] + a2 + gUnknown_03002B20) & 0xff);
        *dst++ = ((a1 | ((u16 *)a3)[1]) & ~0x1ff)
               | ((((u16 *)a3)[1] + a1 + gUnknown_030030D0) & 0x1ff);
        *dst = ((u16 *)a3)[2] + a4;
        dst += 2;
        gUnknown_03002F2C = (u8 *)gUnknown_03002F2C + 8;
        a3 = (u16 *)a3 + 3;
        n--;
    }
}
asm(".global sub_0801BD00\n.thumb_set sub_0801BD00, PutOamHi\n");

void PutOamLo(s32 a1, s32 a2, u16 *src, s32 a4)
{
    u16 *dst;
    s16 n;

    n = *src++;
    dst = gUnknown_0300141C;

    while (n != 0)
    {
        *dst++ = ((a2 | src[0]) & ~0xff) | ((src[0] + a2) & 0xff);
        *dst++ = ((a1 | src[1]) & ~0x1ff) | ((src[1] + a1) & 0x1ff);
        *dst = src[2] + a4;
        dst += 2;
        gUnknown_0300141C = (u8 *)gUnknown_0300141C + 8;
        src += 3;
        n--;
    }
}
asm(".global sub_0801BDB4\n.thumb_set sub_0801BDB4, PutOamLo\n");
