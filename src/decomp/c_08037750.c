#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037750.
 * sub_08037750 @ 0x08037750
 */

/* The `(u16)` cast on the palette offset is load-bearing and not cosmetic:
 * ApplyPaletteExt's second parameter is declared `u32`, so `a * 0x20` alone
 * emits a bare `lsl #5`. The ROM has `lsl #0x15; lsr #0x10`, which is that
 * shift merged with a 16-bit truncation. Either the cast is in the source or
 * the prototype's second parameter is really `u16`; the cast is chosen because
 * it is local to this call site and ApplyPaletteExt has matched callers
 * elsewhere that the prototype change would disturb. */
void StartMapPreviewPalette(int a)
{
    gUnknown_0300057C = a;
    ApplyPaletteExt(gUnknown_081253F0, (u16)(a * 0x20), 0x20);
    AddVBlankHook((void *)AnimateMapPreviewPalette);
    ResetTeamColorSnapshot();
}
asm(".global sub_08037750\n.thumb_set sub_08037750, StartMapPreviewPalette\n");
