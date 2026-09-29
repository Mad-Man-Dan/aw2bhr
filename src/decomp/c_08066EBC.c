#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066EBC.
 * sub_08066EBC @ 0x08066EBC
 */

#include "hardware.h"

/* The residual that stood at 96.0% for four attempts was the ORDER of
 * `movs r1,#15` and `lsrs r0,r4,#1`, and it is NOT reachable by commuting the
 * mask: agbcc canonicalises a constant to tree operand 1 and expand_expr
 * always expands operand 0 first, so `0xf & x` and `x & 0xf` emit the shift
 * first (measured by compile_probe, wave 52). The fix is to make the mask a
 * PSEUDO that is live before the shift -- a plain local assigned the constant
 * in its own statement. cse does not fold it back, because THUMB `andsi3` has
 * no immediate form and the constant has to sit in a register anyway.
 *
 * Settled earlier and still load-bearing:
 *   - `(t + 0xa) * 0x421` is WRONG. agbcc expands a multiply by 0x421 with
 *     Horner; the ROM has the flat sum-of-shifts, so the source spells them.
 *   - the shift is LOGICAL even though gGameClock is s32, so the source
 *     casts to u32. Plain `>> 1` gives `asrs`.
 *   - the `+ 0xa` must stay INSIDE the store expression; hoisting it moves the
 *     gPal pool load after it. */

void MatchSetupCyclePaletteColors(void)
{
    int t;
    int i;
    int m;

    t = gGameClock & 0x1f;

    if (t > 0xf)
        t = 0x1f - t;

    gPal[0x148] = ((t + 0xa) << 10) + ((t + 0xa) << 5) + (t + 0xa);

    m = 0xf;
    i = (u32)gGameClock >> 1;
    i &= m;

    gPal[0x1d9] = gUnknown_0817AF18[0x20 + i];
    gPal[0x1df] = gUnknown_0817AF18[0x20 + i];

    EnablePaletteSync();
}
asm(".global sub_08066EBC\n.thumb_set sub_08066EBC, MatchSetupCyclePaletteColors\n");
