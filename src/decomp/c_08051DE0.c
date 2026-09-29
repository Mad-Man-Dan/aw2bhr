#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051DE0.
 * sub_08051DE0 @ 0x08051DE0
 */

/* MATCHED. The representative of the wave-17 shape cluster
 * SmokeEffect_Init / SplashEffect_Init / BattleAnimExplosion_Init: rebuild the OBJ attributes of
 * the current gUnknown_03001470 slot, then re-place the sprite at a position
 * summed from three tables. The two siblings differ only in an extra
 * `unk28 = 0`, the paletteNum source and the priority constant.
 *
 * Three things in here are not guessable and each was worth a probe round:
 *
 *  - `int tile`, NOT `u16 tile` and not the bare expression. All three spell
 *    the same semantics and all three emit the same `ldrh`, but only the `int`
 *    temp produces the ROM's `ldr r2,=0x3FF; adds r0,r2,#0; ands r1,r0` --
 *    the bare expression and a `u16` temp both drop the register copy and come
 *    out two bytes short. Same discriminator as the matched CruiserPart2_Init,
 *    whose value arrives from arithmetic rather than a load.
 *
 *  - gUnknown_08552D80 and gUnknown_085D7E28 are STRUCT arrays, not `u16
 *    [][14]` / `u16 [][5][4]`. The constant-offset members (unk06, unk04) want
 *    their offset in the `ldrh` immediate; the array spelling folds the same
 *    constant into the symbol's address instead and costs an extra `add` at
 *    each of the two sites. See the note on those two in unknown-globals.h.
 *
 *  - `x` starts from the gUnknown_02029A10 entry, not from the gUnknown_08552D80
 *    row. Writing the table term first reverses the pool order AND the load
 *    order (agbcc expands the addresses in source order and issues the loads in
 *    the opposite one), which shows up as gUnknown_08552D80's pool word landing
 *    ahead of gUnknown_02029A10's. */
void SmokeEffect_Init(void)
{
    struct OamData oam;
    int tile;
    u16 x;
    u16 y;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.paletteNum = 8;
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
asm(".global sub_08051DE0\n.thumb_set sub_08051DE0, SmokeEffect_Init\n");
