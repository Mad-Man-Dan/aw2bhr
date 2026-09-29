#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022A08.
 * sub_08022A08 @ 0x08022A08, sub_08022A34 @ 0x08022A34, sub_08022A6C @ 0x08022A6C
 */

/* Four teardown calls, a flag byte cleared, then one script started.
 * sub_0801537C returns int and the result is discarded; the epilogue is
 * `pop {r0}; bx r0`, which is what makes this function void rather than a tail
 * forward. */
void HideRangeOverlay(void)
{
    ClearBg0Tilemap();
    BG_EnableSyncBG0();
    RedrawUnitIconLayer();
    SetMapLayersDefault();
    gUnknown_03000559 = 0;
    sub_0801537C(gUnknown_08499B4C);
}
asm(".global sub_08022A08\n.thumb_set sub_08022A08, HideRangeOverlay\n");

/* Two tile blobs and the palette between them, in that order -- the palette
 * store sits between the decompressions in the ROM and cannot be moved to
 * either end.
 *
 * 0x220 is `movs #0x88; lsls #2`: agbcc builds an even constant above 255 from
 * an 8-bit value and a shift rather than spending a pool word. */
void LoadCursorSpriteGraphics(void)
{
    Decompress(gUnknown_081019C4, (void *)0x06016CA0);
    ApplyPaletteExt(gUnknown_08101904, 0x220, 0x20);
    Decompress(gUnknown_08124268, (void *)0x06016A40);
}
asm(".global sub_08022A34\n.thumb_set sub_08022A34, LoadCursorSpriteGraphics\n");

/* Two colours per frame, both picked by the same 16-phase counter, into gPal
 * halfwords 0x228 and 0x238. The second table is the first plus 0x10 entries,
 * which is why the ROM bumps the held base by 0x20 bytes in place instead of
 * loading a second pool word.
 *
 * `lsrs #2` on gGameClock is a LOGICAL shift, and that is a second
 * independent proof -- alongside PickRandomCoFromList's __umodsi3 -- that the counter
 * is unsigned; the declaration is still `s32` for src/title-screen.c's sake, so
 * the cast carries it. See the note in unknown-globals.h.
 *
 * The mask constant is materialised once and held in a register across both
 * calls, which is what pays for r5. */
void AnimateCursorPalette(void)
{
    ApplyPaletteAndUploadNow(gUnknown_08101984 + (((u32)gGameClock >> 2) & 0xF), 0x228, 2);
    ApplyPaletteAndUploadNow(gUnknown_08101984 + 0x10 + (((u32)gGameClock >> 2) & 0xF), 0x238, 2);
}
asm(".global sub_08022A6C\n.thumb_set sub_08022A6C, AnimateCursorPalette\n");
