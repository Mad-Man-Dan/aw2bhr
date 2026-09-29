#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A5B8.
 * sub_0806A5B8 @ 0x0806A5B8
 */

#include "hardware.h"

/* One step of the RED-ONLY fade, and a strict subset of ColFadeTick
 * (src/decomp/c_080137AC.c): for every armed row it advances ONLY channel 0 by
 * the row's delta and clamps it, then repacks all three channels into gPal.
 * Channels 1 and 2 are read raw and never accumulated, which is what makes the
 * pack drift towards red rather than towards white.
 *
 * The `(s8)` casts are load-bearing for the same reason c_080137AC.c records:
 * gUnknown_0200B614 is u8, the `+=` overflows past 0x7f, and reading the sum
 * back as SIGNED is what turns the overflow into a negative the `< 0` arm
 * saturates. The ROM distinguishes the two reads of channel 0 -- `ldrb` for the
 * accumulate (only the low byte reaches the `strb`) and `ldrsb` for the clamp.
 *
 * `g` and `b` are int, NOT the s16 that ColFadeTick uses: they carry no clamp,
 * so nothing narrows them, and the ROM shows no `lsl #0x10 / asr #0x10` pair on
 * either. They must still be LOCALS rather than subexpressions of the store --
 * that is what puts both `ldrsb`s ahead of the two `- 0x20`s, and it is also
 * what hands gPal the callee-saved register: written inline, gUnknown_0200B5F4's
 * address pseudo is created first and wins sl instead, moving the pool word
 * order with it. */
void StepPaletteRedTint(void)
{
    int i, j;
    s16 r;
    int g, b;

    for (i = 0x1f; i >= 0; i--)
    {
        if (gUnknown_0200B5F4[i] != 0)
        {
            for (j = 0xf; j >= 0; j--)
            {
                gUnknown_0200B614[(i * 16 + j) * 3] += gUnknown_0200B5F4[i];
                r = (s8)gUnknown_0200B614[(i * 16 + j) * 3] - 0x20;
                if (r > 0x1f)
                    r = 0x1f;
                if (r < 0)
                    r = 0;
                g = (s8)gUnknown_0200B614[(i * 16 + j) * 3 + 1];
                b = (s8)gUnknown_0200B614[(i * 16 + j) * 3 + 2];
                gPal[i * 16 + j] = ((b - 0x20) << 10) + ((g - 0x20) << 5) + r;
            }
        }
    }
    EnablePaletteSync();
}
asm(".global sub_0806A5B8\n.thumb_set sub_0806A5B8, StepPaletteRedTint\n");
