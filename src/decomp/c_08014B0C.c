#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014B0C.
 * sub_08014B0C @ 0x08014B0C, sub_08014B60 @ 0x08014B60
 */

void DrawTallNumberRightAligned(int x, int y, u16 *base, u32 n, u16 pal, u16 tileGroup)
{
    u16 *dest = base + (s16)x + ((s16)y * 32);
    u32 tileBase = tileGroup * 20;

    do
    {
        u32 d = n % 10;

        dest[0] = (tileBase + d * 2 + 1) | pal;
        dest[32] = (tileBase + d * 2 + 2) | pal;
        dest--;
        n = n / 10;
    } while (n != 0);
}
asm(".global sub_08014B0C\n.thumb_set sub_08014B0C, DrawTallNumberRightAligned\n");

void sub_08014B60(int x, int y, u16 *base, int n, u16 pal)
{
    u16 *dest = base + (s16)x + ((s16)y * 32);

    do
    {
        int d = n % 10;

        dest[0] = (d + 0x123) | pal;
        dest[32] = (d + 0x133) | pal;
        dest--;
        n = n / 10;
    } while (n != 0);
}
