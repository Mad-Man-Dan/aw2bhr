#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049B70.
 * sub_08049B70 @ 0x08049B70
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* The insert half of the gUnknown_03000000 callback list, where RemoveMapPreviewPaletteHook
 * above is a remove. ShopScreen_ScrollBackgroundHook is already promoted as void(void) in
 * src/decomp/c_08049BAC.c.
 */

void ShopScreen_AddScrollBackgroundHook(void)
{
    AddVBlankHook((void *)ShopScreen_ScrollBackgroundHook);
}

asm(".global sub_08049B70\n.thumb_set sub_08049B70, ShopScreen_AddScrollBackgroundHook\n");
