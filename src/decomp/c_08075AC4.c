#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075AC4.
 * sub_08075AC4 @ 0x08075AC4
 */

#include "hardware.h"

void StepBank15WhiteFade(int a1, int a2)
{
    int i;
    int n;
    s16 r;
    s16 g;
    s16 b;

    n = 0xF9;

    for (i = 9; i >= 0; i--)
    {
        if (gUnknown_0200B5F4[15] > 0)
        {
            r = Interpolate(0, (s8)gUnknown_0200B614[n * 3], 0x1F, a1, a2);
            g = Interpolate(0, (s8)gUnknown_0200B614[n * 3 + 1], 0x1F, a1, a2);
            b = Interpolate(0, (s8)gUnknown_0200B614[n * 3 + 2], 0x1F, a1, a2);
        }
        else
        {
            r = Interpolate(0, 0x1F, (s8)gUnknown_0200B614[n * 3], a1, a2);
            g = Interpolate(0, 0x1F, (s8)gUnknown_0200B614[n * 3 + 1], a1, a2);
            b = Interpolate(0, 0x1F, (s8)gUnknown_0200B614[n * 3 + 2], a1, a2);
        }

        if (r > 0x1F)
            r = 0x1F;

        if (r < 0)
            r = 0;

        if (g > 0x1F)
            g = 0x1F;

        if (g < 0)
            g = 0;

        if (b > 0x1F)
            b = 0x1F;

        if (b < 0)
            b = 0;

        gPal[n] = b * 0x400 + g * 0x20 + r;

        n--;
    }

    EnablePaletteSync();
}
asm(".global sub_08075AC4\n.thumb_set sub_08075AC4, StepBank15WhiteFade\n");
