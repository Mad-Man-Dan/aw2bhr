#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080524C0.
 * sub_080524C0 @ 0x080524C0
 */

/* MATCHED. The looser relative of SmokeEffect_Init / BattleAnimExplosion_Init (shape ratio
 * 0.744-0.750); see SmokeEffect_Init for the shared spellings. Differences:
 *   - it has BattleAnimExplosion_Init's `unk28 = 0`;
 *   - paletteNum comes from gUnknown_08551D0C rather than being the literal 8,
 *     and it needs the `pal` temp for the reason docs/agbcc-codegen.md gives
 *     under "Large functions": assigning a u16 table element straight into a
 *     bitfield narrower than 16 bits lets force_to_mode push the store's byte
 *     mask back into the load and emit `ldrb`. The ROM has `ldrh`, so the
 *     original bound the element first, exactly as CruiserPart2_Init does;
 *   - priority is 1, as in SmokeEffect_Init.
 * Everything else -- including which address ends up in r7 for the whole
 * function -- falls out of the extra gUnknown_0300453C read and needs no
 * source-level encouragement. */
void SplashEffect_Init(void)
{
    struct OamData oam;
    u16 pal;
    int tile;
    u16 x;
    u16 y;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    tile = gUnknown_02029808[gUnknown_0300453C].unk3a[gUnknown_02029808[gUnknown_0300453C].unk2e];
    oam.tileNum = tile;
    oam.priority = 1;

    x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk02[gUnknown_0300453C]
        + gUnknown_085D7E28[gUnknown_03004580[gUnknown_0300453C][1]][gUnknown_0300451C].unk00[gUnknown_0300453C];
    y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk06
        + gUnknown_085D7E28[gUnknown_03004580[gUnknown_0300453C][1]][gUnknown_0300451C].unk04;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetEffectScreenPosition(gUnknown_0300453C, gUnknown_03001FBC, x, y);
}
asm(".global sub_080524C0\n.thumb_set sub_080524C0, SplashEffect_Init\n");
