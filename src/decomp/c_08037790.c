#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037790.
 * sub_08037790 @ 0x08037790
 */

/* The pool word the disassembly calls `gUnknown_08125410` is NOT a symbol of
 * its own -- it is `&gUnknown_081253F0[16]`, and the INSTRUCTION ORDER proves
 * it. With a distinct base symbol agbcc loads the pool word BEFORE the index
 * arithmetic (`ldr base; ldr g; and; lsr; add`); the ROM loads it AFTER
 * (`ldr g; and; lsr; ldr base; add`), which is what folding the +0x20 into the
 * relocation's addend does. Four spellings were probed side by side and only
 * the `&gUnknown_081253F0[16 + i]` one reproduces the order.
 *
 * So 0x081253F0 is a 0x40-byte palette: StartMapPreviewPalette applies its first half
 * wholesale and this picks one colour out of the second half, phase
 * `(gGameClock & 0x3c) >> 2`, a 16-step cycle off the frame counter. */
void AnimateMapPreviewPalette(void)
{
    ApplyPaletteAndUploadNow(&gUnknown_081253F0[16 + ((gGameClock & 0x3c) >> 2)],
        gUnknown_0300057C * 0x20 + 0x1c, 2);
}
asm(".global sub_08037790\n.thumb_set sub_08037790, AnimateMapPreviewPalette\n");
