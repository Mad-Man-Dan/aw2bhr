#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080116E8.
 * sub_080116E8 @ 0x080116E8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F063. Same wrapper shape as F060 with the size constant too large for
 * an imm8, so agbcc materialises it as `movs #imm8; lsls #n` -- 0x800 is
 * 0x80 << 4 and 0x380 is 0xe0 << 2. Nothing in the source asks for that.
 *
 * The members do not share a destination class either:
 * LoadGlyphSpriteTiles and UnitInfoPanel_CopyPictureToVram blit to VRAM through
 * CpuCopyAuto, CopyHiOamShadowToOam pushes an OAM shadow to 0x07000080 through
 * CpuFastCopy -- a different callee, listed in `varies`. Both destinations
 * are bare address literals in the ROM's pool (`.4byte 0x06017800`, not a
 * relocation), so `(void *)0xNNNNNNNN` is the honest spelling and it is also
 * the one that matches; this is NOT the invented-lds-symbol case in the
 * Workflow section of docs/agbcc-codegen.md, where the pool word would have
 * carried a relocation. */

void LoadGlyphSpriteTiles(void)
{
    CpuCopyAuto(gUnknown_080A1424, (void *)0x06017800, 0x800);
}

asm(".global sub_080116E8\n.thumb_set sub_080116E8, LoadGlyphSpriteTiles\n");
