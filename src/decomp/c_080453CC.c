#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080453CC.
 * sub_080453CC @ 0x080453CC
 */

#include "hardware.h"

/* CoPowerOverlay_Init with every proc field replaced by a ROM constant -- same five
 * calls in the same order. */
void CoPowerOverlayFixed_Init(void)
{
    Decompress(gUnknown_08112704, (void *)(gUnknown_03002B6C.bits.chr_block * 0x4000 + 0x06005600));
    Decompress(gUnknown_0811315C, gBG0TilemapBuffer);
    AddToHalfwords(gBG0TilemapBuffer, 0x800, 0x82b0);
    ApplyPaletteExt(gUnknown_08113BA0, 0x100, 0x20);
    BG_EnableSyncBG0();
}
asm(".global sub_080453CC\n.thumb_set sub_080453CC, CoPowerOverlayFixed_Init\n");
