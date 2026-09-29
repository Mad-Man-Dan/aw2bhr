#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072BCC.
 * sub_08072BCC @ 0x08072BCC
 */

#include "hardware.h"

/* Scales palette `pal`'s 16 colours to 3/4 brightness in place: each 5-bit
 * channel is masked out, halved twice by the `>> 2`, tripled, and re-masked
 * so the carry into the next channel is dropped.
 *
 * Two loop facts had to be read off the preheader. (1) The counter is
 * `for (i = 0; i < 0x10; i++)` even though the ROM counts 0xF down to 0 with
 * `bge` -- `i` is dead in the body, so agbcc's check_dbra_loop reverses it,
 * and the giveaway is that `movs r5, #0xf` and the pointer init sit AFTER the
 * three hoisted mask constants: LICM's movables are emitted before
 * strength_reduce's biv/giv initialisations, so anything that follows a
 * hoisted invariant in the preheader was written by the loop optimiser, not
 * by the source. (2) The channels must be read as `p[i]` three times rather
 * than through a local copy of the colour: with a local, agbcc copies the
 * COLOUR into the AND's destination (`adds r0, r1, #0; ands r0, r7`); reading
 * the memory each time it copies the MASK instead (`adds r0, r7, #0; ands
 * r0, r1`), which is what the ROM has. */
void DimPaletteByQuarter(int pal)
{
    u16 *p = gPal + pal * 0x10;
    int i;

    for (i = 0; i < 0x10; i++)
    {
        int r = ((p[i] & 0x1F) >> 2) * 3;
        int g = ((p[i] & 0x3E0) >> 2) * 3;
        int b = ((p[i] & 0x7C00) >> 2) * 3;

        p[i] = (r & 0x1F) | (g & 0x3E0) | (b & 0x7C00);
    }
}
asm(".global sub_08072BCC\n.thumb_set sub_08072BCC, DimPaletteByQuarter\n");
