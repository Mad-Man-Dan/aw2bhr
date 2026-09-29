#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805297C.
 * sub_0805297C @ 0x0805297C
 */

/* MATCHED. Sibling of SmokeEffect_Init (shape ratio 0.937); see the comment on
 * that function for the three non-obvious spellings, all of which carry over
 * unchanged. This one differs in exactly two source statements:
 *   - an extra `gUnknown_03001470[proc].unk28 = 0;` ahead of the unk30 store;
 *   - `oam.priority = 3` instead of 1, which is why the ROM has a bare
 *     `orrs #0xc` here and a `movs #0xd; rsbs; ands; movs #4; orrs` there --
 *     `(x & ~0xc) | 0xc` folds, exactly as docs/agbcc-codegen.md records for
 *     `priority = 3`. Read the GROUP, not the statement: the missing AND mask
 *     is not evidence of a `|=`.
 * The third apparent difference -- `adds r2,#0x34` here vs `adds r1,r2,#0;
 * adds r1,#0x34` there -- is a liveness consequence of the extra store and
 * needs nothing in the source: with a third use of the gUnknown_03001470 base
 * still ahead of it, gcc copies rather than clobbers. */
void BattleAnimExplosion_Init(void)
{
    struct OamData oam;
    int tile;
    u16 x;
    u16 y;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.paletteNum = 8;
    tile = gUnknown_02029808[gUnknown_0300453C].unk3a[gUnknown_02029808[gUnknown_0300453C].unk2e];
    oam.tileNum = tile;
    oam.priority = 3;

    x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk02[gUnknown_0300453C]
        + gUnknown_085D7E28[gUnknown_03004580[gUnknown_0300453C][1]][gUnknown_0300451C].unk00[gUnknown_0300453C];
    y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk06
        + gUnknown_085D7E28[gUnknown_03004580[gUnknown_0300453C][1]][gUnknown_0300451C].unk04;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetEffectScreenPosition(gUnknown_0300453C, gUnknown_03001FBC, x, y);
}
asm(".global sub_0805297C\n.thumb_set sub_0805297C, BattleAnimExplosion_Init\n");
