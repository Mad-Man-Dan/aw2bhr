#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052154.
 * sub_08052154 @ 0x08052154
 */

#include "hardware.h"

/* The wave-17 shape cluster of src/decomp/c_08051DE0.c, two members further
 * on: rebuild the OBJ attributes of the current gUnknown_03001470 slot, then
 * re-place the sprite. Read that file's header before this one -- the `int
 * tile` temp and the struct (not `[][14]`) spelling of gUnknown_08552D80 are
 * both load-bearing here for the same reasons, and neither is guessable.
 *
 * What is new in this member: an extra `unk28 = 0` ahead of the other two slot
 * writes, and a much shorter position sum -- x takes ONE table term and y takes
 * none at all. The term is `gUnknown_08552D80[3]`, a CONSTANT row, which agbcc
 * folds to `adds rB, #0x56` (3 * 0x1c + offsetof unk02) on the base rather than
 * into the relocation; that is the "add on the base" row of the pool-fold table
 * and it is what identifies the row index as a literal 3.
 *
 * The entry term is written FIRST in both sums, and that is a readout, not a
 * style choice: gUnknown_02029A10's pool word lands ahead of gUnknown_08552D80's
 * exactly when the source names it first, because agbcc expands the addresses
 * in source order and issues the loads in the opposite one. */
void AirBlastEffect_Init(void)
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
    oam.priority = 1;

    x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x
        + gUnknown_08552D80[3].unk02[gUnknown_0300453C];
    y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetEffectScreenPosition(gUnknown_0300453C, gUnknown_03001FBC, x, y);
}
asm(".global sub_08052154\n.thumb_set sub_08052154, AirBlastEffect_Init\n");
