#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BA1C.
 * sub_0801BA1C @ 0x0801BA1C, sub_0801BA4C @ 0x0801BA4C
 */

void PutTextTileRows(void *dst, u16 tile, int n)
{
    u16 *p = (u16 *)dst;
    int i;

    for (i = 0; i < n; i++)
    {
        p[i] = tile + i * 2;
        p[i + 0x20] = (s16)tile + 1 + i * 2;
    }
}
asm(".global sub_0801BA1C\n.thumb_set sub_0801BA1C, PutTextTileRows\n");

s16 SinDegrees(int a)
{
    int b;

    while (a < 0)
        a += 360;
    while (a > 359)
        a -= 360;

    b = a;
    if (a > 179)
        a -= 180;
    if (a > 90)
        a = 180 - a;

    if (b > 179)
        return -gUnknown_0808EF90[a];
    return gUnknown_0808EF90[a];
}
asm(".global sub_0801BA4C\n.thumb_set sub_0801BA4C, SinDegrees\n");
