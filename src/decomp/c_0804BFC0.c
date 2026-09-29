#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804BFC0.
 * sub_0804BFC0 @ 0x0804BFC0
 */

/* The WholeFigure_Init OAM-rebuild idiom with the tail cut off: fetch the slot's
 * attributes with CopySlotSpriteAttrs, rewrite four bitfields, hand them back with
 * SetSlotSpriteAttrs. Every global it touches is a strict subset of WholeFigure_Init's,
 * so the type model came over whole and nothing here is newly declared -- the
 * data_refs-subset axis delivering exactly what it claims.
 *
 * The one thing the exemplar does NOT supply is column 9 of gUnknown_085D6A48.
 * The ROM reads it as `adds r0, r0, r3; ldrh r0, [r0, #0x12]` -- the column
 * offset in the LOAD DISPLACEMENT -- which the declared `u16 [][12]` cannot
 * emit, because fold pulls the INTEGER_CST out of the address PLUS and
 * reassociates `base + X*24 + 18` into `(base + 18) + X*24`. A COMPONENT_REF
 * applies its offset to the MEM instead. The row struct is declared locally
 * rather than retyping the global, exactly as the note on gUnknown_085D6A48 in
 * include/unknown-globals.h asks: the four promoted readers all use column 0
 * and the array spelling is settled there.
 *
 * THE TILENUM STATEMENT, closed in wave 20 by W20-A on the axis W20-B named.
 * The ROM needs three things at once and every comma spelling gets two:
 *   1. `a * 0x100` emitted FIRST (`lsls r2, r4, #8` ahead of both pool loads);
 *   2. &gUnknown_085D6A48's pseudo created before &gUnknown_03004580's, so the
 *      pool words come out in that order;
 *   3. the multiply destination-tied to the LOADED value (`muls r0, r6`, not
 *      `adds r1,r6,#0; muls r1,r0; adds r0,r1,#0`).
 * W20-B measured four comma spellings: none reaches all three, because binding
 * the loaded value inside a comma drags the whole right operand of the `+`
 * forward and loses (1). The fix is to stop using commas: give each of the
 * three values its own STATEMENT, in the order the ROM computes them. Then
 * nothing can be hoisted past anything else, `s` and `t` each die at their use,
 * and the multiply ties to `t`.
 *   The general rule, and it is the useful direction: a comma operator moves
 * ONE reference to a point no statement boundary can reach, and it drags its
 * whole subexpression with it. When a statement needs TWO values pinned in
 * opposite directions, the comma cannot do it and separate statements can --
 * so try the plain spelling BEFORE reaching for a second comma, not after.
 * (`s = a * 0x100;` bound and the rows comma kept is byte-identical to this,
 * so it is the SEPARATION that matters, not which construct does it.) */

struct Unk85D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 filler_02[0x10];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[4];
};

void SetFigureObjAttrs(u16 a, u16 b, s16 c)
{
    struct OamData oam;
    struct Unk85D6A48Row *rows;
    u16 pal;
    u16 prio;
    u16 t;
    int s;

    CopySlotSpriteAttrs(c, (struct UnkVec *)&oam);

    oam.hFlip = a ^ 1;
    pal = gUnknown_08551D0C[a][0];
    oam.paletteNum = pal;

    s = a * 0x100;
    rows = (struct Unk85D6A48Row *)gUnknown_085D6A48;
    t = rows[gUnknown_03004580[a][1]].unk12;
    oam.tileNum = s + t * b;

    prio = gUnknown_085523A4[(a + gUnknown_0300450C) & 1];
    oam.priority = prio;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
}
asm(".global sub_0804BFC0\n.thumb_set sub_0804BFC0, SetFigureObjAttrs\n");
