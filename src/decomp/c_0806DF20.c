#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806DF20.
 * sub_0806DF20 @ 0x0806DF20
 */

#include "hardware.h"

/* A grey ramp written straight into the palette. `x * 0x421` is NOT what the
 * ROM has: agbcc's synth_mult expands the multiply as `((x*32)+x)*32+x`, four
 * instructions in a different order. The ROM's `lsls #0xa` / `lsls #5` / two
 * adds is the sum of three shifted terms written out in the source -- the RGB15
 * "same value in all three channels" idiom -- and `+` rather than `|`, which
 * the `adds` settles.
 *
 * `(n + 10)` has to appear at each of the three terms rather than being folded
 * into `n` first: bound to `n` beforehand, agbcc reuses the counter's register
 * for the sum and the pool `ldr` for gPal lands one slot earlier.
 *
 * The `subs r0, r1, r0` reuses the 0x1F still sitting in r1 from the mask --
 * that is the constant-reuse rule, not a second literal. 0x290 / 2 == 0x148. */
void RulesScreenPulseGreyPaletteColor(void)
{
    int n = gGameClock & 0x1F;

    if (n > 0xF)
        n = 0x1F - n;

    gPal[0x148] = ((n + 10) << 10) + ((n + 10) << 5) + (n + 10);
    EnablePaletteSync();
}
asm(".global sub_0806DF20\n.thumb_set sub_0806DF20, RulesScreenPulseGreyPaletteColor\n");
