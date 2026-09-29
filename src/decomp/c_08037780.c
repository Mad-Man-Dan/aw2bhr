#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037780.
 * sub_08037780 @ 0x08037780
 */

/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */


/* Removes AnimateMapPreviewPalette from the 16-slot gUnknown_03000000 callback list.
 * RemoveVBlankHook is the remover and AddVBlankHook the inserter, both `void *`,
 * so a function argument casts -- same spelling as sub_080111AC's
 * registration. AnimateMapPreviewPalette is not promoted yet; `void (void)` is read off
 * its own bytes (`push {lr}` ... `pop {r0}; bx r0`, no argument register
 * read).
 */

void RemoveMapPreviewPaletteHook(void)
{
    RemoveVBlankHook((void *)AnimateMapPreviewPalette);
}
asm(".global sub_08037780\n.thumb_set sub_08037780, RemoveMapPreviewPaletteHook\n");
