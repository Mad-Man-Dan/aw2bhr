#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C6DC.
 * sub_0804C6DC @ 0x0804C6DC
 */

/* One of the ~40 sprite-attribute setters between 0x0804B180 and 0x08053614.
 * Rebuilds the OBJ attributes of the gUnknown_03001470 slot named by
 * gUnknown_03001FBC, re-seats it at its gUnknown_02029A10 entry's position and
 * arms a continuation.
 *
 * Twin of LanderPart_Init: the two differ in exactly one immediate (the tile base,
 * 0x50 vs 0x80) and one pool symbol (the continuation), which is the same pair
 * of discriminators CruiserPart2_Init/BattleshipPart2_Init turn on. Same three spellings as
 * those two -- `pal`/`prio` bound to locals so the bitfield store does not
 * narrow the table `ldrh` to `ldrb`, `* 0x100` rather than `<< 8`, and both
 * globals of the priority index read directly so the `lsl #16; lsr #15` pair
 * survives.
 *
 * x and y are `u16` here even though the call site emits `ldrsh`: converting a
 * u16 memory operand to the s16 parameter is a sign-extension of the low half,
 * which combine folds into the load. The `ldrh` + `lsl #16; asr #16` in
 * CruiserFigure_Init is the same members read into an int subtraction instead. */
void SubmarinePart_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100 + 0x50;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    SetSlotSpritePosition(gUnknown_03001FBC,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
    SetSlotSpriteHook(gUnknown_03001FBC, (u32)SubmarinePart_StreamHook);
}
asm(".global sub_0804C6DC\n.thumb_set sub_0804C6DC, SubmarinePart_Init\n");
