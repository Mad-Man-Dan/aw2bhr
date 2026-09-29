#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080137AC.
 * sub_080137AC @ 0x080137AC, sub_08013830 @ 0x08013830, sub_080138B0 @ 0x080138B0, sub_08013928 @ 0x08013928, sub_0801394C @ 0x0801394C, sub_080139C4 @ 0x080139C4, sub_080139E0 @ 0x080139E0
 */

#include "hardware.h"

/* Unpacks the whole 32-row palette into the three-bytes-per-colour channel
 * shadow, biasing every channel by +0x20, and arms every row with `delta` so
 * ColFadeTick fades them. The bias is the headroom the fade clamps against:
 * a channel may be driven up to 0x20 below or above its 5-bit range before
 * ColFadeTick subtracts the 0x20 back off and saturates. */
void ColFadeToBlack(s8 delta)
{
    int i, j;

    for (i = 0x1f; i >= 0; i--)
    {
        gUnknown_0200B5F4[i] = delta;
        for (j = 0; j <= 0xf; j++)
        {
            gUnknown_0200B614[(i * 16 + j) * 3] = (gPal[i * 16 + j] & 0x1f) + 0x20;
            gUnknown_0200B614[(i * 16 + j) * 3 + 1] = ((gPal[i * 16 + j] >> 5) & 0x1f) + 0x20;
            gUnknown_0200B614[(i * 16 + j) * 3 + 2] = ((gPal[i * 16 + j] >> 10) & 0x1f) + 0x20;
        }
    }
}
asm(".global sub_080137AC\n.thumb_set sub_080137AC, ColFadeToBlack\n");

/* ColFadeToBlack with no bias: the channel shadow is seeded with the raw 5-bit
 * components, so the fade saturates at 0 immediately in the dark direction. */
void ColFadeFromBlack(s8 delta)
{
    int i, j;

    for (i = 0x1f; i >= 0; i--)
    {
        gUnknown_0200B5F4[i] = delta;
        for (j = 0; j <= 0xf; j++)
        {
            gUnknown_0200B614[(i * 16 + j) * 3] = gPal[i * 16 + j] & 0x1f;
            gUnknown_0200B614[(i * 16 + j) * 3 + 1] = (gPal[i * 16 + j] >> 5) & 0x1f;
            gUnknown_0200B614[(i * 16 + j) * 3 + 2] = (gPal[i * 16 + j] >> 10) & 0x1f;
        }
    }
}
asm(".global sub_08013830\n.thumb_set sub_08013830, ColFadeFromBlack\n");

/* The one-row form of ColFadeToBlack. The loop counter is s8, not int: it is
 * kept as a byte and sign-extended at both the use and the exit test
 * (`lsls #0x18` feeding a `lsrs` for the variable and an `asrs` for the
 * compare), which an int counter would not produce. */
void ColFadeToWhiteRow(u8 row, s8 delta)
{
    s8 i;

    gUnknown_0200B5F4[row] = delta;
    for (i = 0; i <= 0xf; i++)
    {
        gUnknown_0200B614[(row * 16 + i) * 3] = (gPal[row * 16 + i] & 0x1f) + 0x20;
        gUnknown_0200B614[(row * 16 + i) * 3 + 1] = ((gPal[row * 16 + i] >> 5) & 0x1f) + 0x20;
        gUnknown_0200B614[(row * 16 + i) * 3 + 2] = ((gPal[row * 16 + i] >> 10) & 0x1f) + 0x20;
    }
}
asm(".global sub_080138B0\n.thumb_set sub_080138B0, ColFadeToWhiteRow\n");

/* Applies ColFadeToWhiteRow to all 32 palette slots. The counter is `s8`: it is
 * carried between iterations as `(u8)i` and the exit test reads the shifted
 * temp (`cmp r4, #0; bge` on `i << 24`), which is what ends the loop at -1.
 * `a1` is `int` per its declaration, so the `(s8)` narrowing for the callee's
 * second parameter stays inside the loop while the `lsls #0x18` it shares
 * hoists out. */
void ColFadeToWhite(int a1)
{
    s8 i;

    for (i = 31; i >= 0; i--)
        ColFadeToWhiteRow(i, a1);
}
asm(".global sub_08013928\n.thumb_set sub_08013928, ColFadeToWhite\n");

/* ColFadeToWhiteRow with twice the bias, giving the fade 0x40 of headroom instead
 * of 0x20 -- the brighten direction, where a channel has to travel further
 * before ColFadeTick's clamp lets it show. */
void ColFadeFromWhiteRow(u8 row, s8 delta)
{
    s8 i;

    gUnknown_0200B5F4[row] = delta;
    for (i = 0; i <= 0xf; i++)
    {
        gUnknown_0200B614[(row * 16 + i) * 3] = (gPal[row * 16 + i] & 0x1f) + 0x40;
        gUnknown_0200B614[(row * 16 + i) * 3 + 1] = ((gPal[row * 16 + i] >> 5) & 0x1f) + 0x40;
        gUnknown_0200B614[(row * 16 + i) * 3 + 2] = ((gPal[row * 16 + i] >> 10) & 0x1f) + 0x40;
    }
}
asm(".global sub_0801394C\n.thumb_set sub_0801394C, ColFadeFromWhiteRow\n");

/* Applies ColFadeFromWhiteRow to all 32 rows. The parameter stays wide: the shift
 * pair around the call is the conversion to ColFadeFromWhiteRow's s8, with the `lsls`
 * hoisted out of the loop. */
void ColFadeFromWhite(s32 delta)
{
    int i;

    for (i = 0x1f; i >= 0; i--)
        ColFadeFromWhiteRow(i, delta);
}
asm(".global sub_080139C4\n.thumb_set sub_080139C4, ColFadeFromWhite\n");

/* One step of the fade: for every armed row, add its s8 delta to all three
 * channels of all 16 colours, then take the bias back off and repack.
 *
 * The channel is read back as `(s8)` even though gUnknown_0200B614 is u8 --
 * that is what makes the clamp work. The `+=` above can drive a channel past
 * 0x7f or below 0, and reading the sum as signed is what turns an overflow
 * into a negative that the `< 0` arm saturates to 0. The two loads are
 * distinguishable in the ROM: the `+=` uses `ldrb` because only the low byte
 * reaches the `strb`, while the clamp's read is `ldrsb`.
 *
 * The clamp is one s16 local per channel, not a mask: the ROM keeps the value
 * in a u16 (`lsrs #0x10`) and compares the sign-extended s16 (`asrs #0x10`),
 * which is just how agbcc holds an s16 across a compare. */
void ColFadeTick(void)
{
    int i, j;
    s16 r, g, b;

    for (i = 0x1f; i >= 0; i--)
    {
        if (gUnknown_0200B5F4[i] != 0)
        {
            for (j = 0xf; j >= 0; j--)
            {
                gUnknown_0200B614[(i * 16 + j) * 3] += gUnknown_0200B5F4[i];
                gUnknown_0200B614[(i * 16 + j) * 3 + 1] += gUnknown_0200B5F4[i];
                gUnknown_0200B614[(i * 16 + j) * 3 + 2] += gUnknown_0200B5F4[i];
                r = (s8)gUnknown_0200B614[(i * 16 + j) * 3] - 0x20;
                if (r > 0x1f)
                    r = 0x1f;
                if (r < 0)
                    r = 0;
                g = (s8)gUnknown_0200B614[(i * 16 + j) * 3 + 1] - 0x20;
                if (g > 0x1f)
                    g = 0x1f;
                if (g < 0)
                    g = 0;
                b = (s8)gUnknown_0200B614[(i * 16 + j) * 3 + 2] - 0x20;
                if (b > 0x1f)
                    b = 0x1f;
                if (b < 0)
                    b = 0;
                gPal[i * 16 + j] = (b << 10) + (g << 5) + r;
            }
        }
    }
    EnablePaletteSync();
}
asm(".global sub_080139E0\n.thumb_set sub_080139E0, ColFadeTick\n");
