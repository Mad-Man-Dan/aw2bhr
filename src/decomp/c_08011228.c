#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011228.
 * sub_08011228 @ 0x08011228
 */

/* The HBlank scanline buffer fill for the "curtain"/wipe effect installed by
 * sub_08011298 and sub_0801137C through StartFadeScreenLines's fifth parameter.
 *
 * 160 scanlines, two interleaved halves. On odd lines the window edge is
 * packed as `((0xF0 - v) << 8) | 0xF0` (a WIN0H-style right/left pair); on
 * even lines the raw clamped value is stored. `gUnknown_03001408` is the
 * volatile cursor both installers seed, which is why it is re-read in every
 * iteration instead of being hoisted.
 *
 * The two clamps really do differ (`v < 0` on the odd arm, `v <= 0` on the
 * even one): a compiler will not turn `bge` into `bgt`, so that asymmetry is
 * in the source.
 *
 * The buffer's address is bound to a local BEFORE the loop. That is what puts
 * its `ldr` ahead of `i = 0` in the preheader and gives it the first literal
 * pool slot; referencing gUnknown_0200B274 inside the loop body instead makes
 * gUnknown_03001408's address the first-created pseudo and swaps both the two
 * pool words and the two callee-saved registers. */
void FillWipeScanlineTable(void)
{
    u16 *p;
    int i;
    int v;

    p = gUnknown_0200B274;

    for (i = 0; i < 160; i++)
    {
        if (i & 1)
        {
            v = ((i + 1) >> 1) + gUnknown_03001408 - 0x50;
            if (v < 0)
                v = 0;
            if (v > 0xEF)
                v = 0xF0;
            p[i] = ((0xF0 - v) << 8) | 0xF0;
        }
        else
        {
            v = gUnknown_03001408 - ((i + 1) >> 1);
            if (v <= 0)
                v = 0;
            if (v > 0xEF)
                v = 0xF0;
            p[i] = v;
        }
    }
}
asm(".global sub_08011228\n.thumb_set sub_08011228, FillWipeScanlineTable\n");
