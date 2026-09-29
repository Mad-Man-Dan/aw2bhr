#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084858.
 * sub_08084858 @ 0x08084858
 */

/* Return retyped from `int` to `u8` in wave 20 (W20-B). Byte-identical here --
 * `ldrb` either way -- but GetMainMenuTilePalette and GetMainMenuTileHighlightPalette both re-narrow the
 * result with a bare `lsls r0, r0, #0x18`, which an `int` return does not
 * produce. See include/unknown-functions.h for the full evidence and for the
 * one model this does NOT rule out. */
u8 IsMainMenuTileComplete(int index)
{
    return gUnknown_03000650[index];
}
asm(".global sub_08084858\n.thumb_set sub_08084858, IsMainMenuTileComplete\n");
