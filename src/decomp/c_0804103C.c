#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804103C.
 * sub_0804103C @ 0x0804103C
 */

/* Loads one unit's sprite sheet and palette -- the LoadCaptureUnitSprite shape with a
 * switch. a1 packs an army in its top three bits and a class in its low five:
 * the class picks the ROM sheet, the army picks both GetCellCountry's variant
 * index and the gPlayers record whose unk1a is the palette bank.
 *
 * THE FIELD EXTRACTIONS MUST BOTH BE MASKED. Wave 35 parked this at 82.2% with
 * `u8 sel = a1;` and a bare `sel >> 5`, and every unmasked spelling measured
 * this wave reproduces that miss:
 *   - ANY logical right shift of the narrowed value folds. agbcc merges
 *     `((a1 << 24) >> 24) >> 5` into `(a1 << 24) >> 29`, which leaves the
 *     SHIFTED-LEFT value live across the call instead of the narrowed one --
 *     `u8 sel`, `u32 sel = (u8)a1`, `(u32)sel >> 5`, `sel >>= 5`, a real `u8`
 *     PARAMETER, and narrowing in place into a1 itself all fold identically.
 *   - ANY signed right shift blocks the fold and gets the ROM's register
 *     allocation exactly, but spells the shift `asrs r4,r4,#5` where the ROM
 *     has `lsrs`. Two bytes, and no allocation lever reaches it.
 * A redundant mask on the SHIFT RESULT is what does both at once: `& 7` is a
 * no-op agbcc discards (it already knows the value fits in three bits), but it
 * breaks the shift-merge, and the remaining shift comes out logical. The
 * equivalent `(u8)sel >> 5` measures byte-identical. See docs/agbcc-codegen.md.
 *
 * Case-arm order is source order: case 8, `case 6: default:`, 14, 10, 11, 20.
 * `case 6` sharing the default arm is what makes the jump table run 6..20
 * (fifteen entries) rather than 8..20 -- the ROM's `subs r0,r5,#6; cmp r0,#0xe`.
 * `(u16)((a3 + 0x10) * 0x20)` is a MULTIPLY, as in the matched LoadCaptureUnitSprite. */
void LoadCapturePropertySprite(int a1, int a2, int a3)
{
    int sel = (u8)a1;
    int k = sel & 0x1f;
    int i = GetCellCountry(sel);
    u8 pal = gPlayers[(sel >> 5) & 7].teamColor;
    u8 *src;

    switch (k)
    {
    case 8:
        src = &gUnknown_081218BC[(i - 1) * 0x400];
        break;
    case 6:
    default:
        src = gUnknown_08122CBC;
        break;
    case 14:
        src = gUnknown_081230BC;
        break;
    case 10:
        src = gUnknown_081234BC;
        break;
    case 11:
        src = gUnknown_081238BC;
        break;
    case 20:
        src = gUnknown_08123CBC;
        break;
    }

    RegisterDataMove(src, (void *)(0x06010000 + (a2 & 0x3ff) * 32), 0x400);
    ApplyPaletteExt(&gUnknown_081213F4[pal * 0x10], (u16)((a3 + 0x10) * 0x20), 0x20);
}
asm(".global sub_0804103C\n.thumb_set sub_0804103C, LoadCapturePropertySprite\n");
