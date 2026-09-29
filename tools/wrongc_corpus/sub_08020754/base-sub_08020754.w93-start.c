#include "global.h"

/* WAVE 87 (W87-C) -- CLEAN NO, draft unchanged at 220/208 (+12), 27.9%
 * (`configured`). No probe and no try_match spent. Evidence in
 * work/sub_08020754/W87-notes.md.
 * TWIN AXIS, negative, all three. src/decomp/c_08044854.c, c_08026100.c (a
 * byte-identical pair) and c_08025744.c screen at J = 1.00 on the two map
 * globals but LACK this construct: sub_08026100/sub_08044854 do have an
 * if/else with a `= 1` arm, but their index is computed ONCE before the
 * if/else, neither arm touches it, and they store to a struct field rather
 * than a map cell -- so there is no duplicated block for cross-jumping to
 * merge and no label with two uses. c_08026100.c's header names its own whole
 * cost as the COMMA-CHAIN ANCHOR (pool order and address-load position), which
 * cannot reach which basic block cse_end_of_basic_block walks into.
 * Note for whoever reopens this: the park's two rule-outs close against each
 * other. Duplicating the `= 1` store is merged by the FIRST jump pass, and
 * binding the offset to a local (the obvious way to keep the blocks distinct)
 * is separately ruled out because it relieves a whole register's worth of
 * pressure. Any future shape must make the two blocks differ WITHOUT adding a
 * local. Do not re-batch on vocabulary overlap; that axis is now measured. */


/* PARKED at 27.9% / +12 bytes, wave 41 (W41-B). RE-MEASURED WAVE 58 (W58-B):
 * still exactly 220/208 and 27.9%, and the diff confirms the wave-41 reading
 * instruction for instruction. Nothing below needs re-deriving.
 *
 * CLASSIFICATION (wave 58): this is residual kind 4 -- a CSE/basic-block
 * decision, not a shape, a type or a constant placement. The wave-58 brief
 * grouped it with two other +12 functions on the theory that one cause
 * explained all three; it does not. The other two were induction-variable
 * residuals in OPPOSITE directions (sub_080200EC was missing a source biv and
 * matched; sub_0800CAA0 has a giv the ROM lacks). +12 here is a coincidence of
 * three unrelated single decisions. Do not look for a shared lever.
 *
 * THE ENTIRE RESIDUAL IS WHICH ARM CSE RECOMPUTES THE INDEX ON.
 * Everything except the two store arms is byte-exact: prologue, both loop
 * guards, the `sub sp,#4` spill of `y + 1`, the three hoisted bases
 * (rowOffset[y] at +0x417A, cell at +0x1432, unit at +0x51A), the whole
 * predicate, and both loop bottoms.
 *   The ROM recomputes `MAP->rowOffset[y] + x` from scratch (reloading
 * gUnknown_08499590 and re-adding 0x417A and y*2) in the "= 0" arm and reuses
 * the loop-top value in the "= 1" arm. This candidate does the opposite. That
 * inverts the literal pool -- the recomputing arm sits AFTER the pool instead
 * of before it, so a second gUnknown_08499590 word AND a second 0x417a word are
 * emitted at the tail (+8 bytes; wave 58 read both in the relocation list) --
 * and it puts the address and the value in swapped hard registers in the two
 * arms, which blocks the final jump pass from cross-jumping the common
 * `strb r1,[r0]` (+2 bytes, +2 for alignment).
 *
 * gcc's cse_end_of_basic_block follows a conditional jump only when the target
 * label has ONE use and is preceded by a barrier. Here the "= 1" label has two
 * (`bne` off the 0x13 test and `bls` off the range test), so CSE cannot follow
 * it, stays on the fall-through, and the "= 0" arm keeps the value. The ROM's
 * CSE clearly DID follow into the "= 1" arm, which means that at CSE time its
 * label had a single use -- i.e. the source had two separate "= 1" blocks that
 * only merged later.
 *
 * Six spellings measured, none of which reproduce it:
 *   - `int i` bound at the top of the x loop: NO spill and gUnknown_08499594
 *     hoisted, both wrong; the bound local removes a whole register's pressure.
 *   - condition negated with the arms swapped (`... != 4 && ... != 0x13 || ...`
 *     with "= 1" first): cross-jumps correctly but recomputes in BOTH arms and
 *     spills 8 bytes.
 *   - `int y, x;` declaration order: byte-identical to `int x, y;`.
 *   - NESTED ifs with the "= 1" store duplicated in the outer else (the shape
 *     that should give the "= 1" label a single use at CSE time): gets the
 *     cross-jumped `strb` AND dst in a low register, both right, but the two
 *     "= 1" blocks are merged by the FIRST jump pass, before CSE ever runs, so
 *     the label has two uses again and neither arm recomputes. 186 bytes, 22
 *     short.
 *   - the two "= 1" arms as an `else if` chain: puts "= 1" first in the layout,
 *     and the ROM's "= 0" arm is first.
 *   - `continue` in the "= 0" arm: gives that arm its own `strb` plus a branch
 *     to the loop increment, where the ROM shares one `strb` with the "= 1"
 *     arm.
 * The next thing to try is a source shape that keeps the two "= 1" blocks
 * distinct through the first jump pass; nothing byte-neutral was found. If the
 * next attempt also fails, this belongs in data/parked.json beside
 * sub_080373F0 as a second basic-block-layout example -- the permuter cannot
 * reach a cross-jumping decision either.
 *
 * The type model below IS settled and is shared with sub_080206E4 (matched):
 * width/height u16 at +0/+2, the u8 unit-id plane at +0x51A, the u8 terrain
 * plane at +0x1432, the u16 row-offset table at +0x417A, all indexed
 * `rowOffset[y] + x`. `(u8)(unk00 - 0x10) > 4` is the range test written with
 * the explicit u8 cast -- the `subs #0x10; lsls #0x18; lsrs #0x18; cmp #4`
 * shape that the note on struct Unit.unk00 already documents for
 * sub_0805CC88. The 32-bit spelling `(u32)(unk00 - 0x10) > 4` drops the two
 * shifts and does not match. */

struct Unk20754Map
{
    /* 0x0000 */ u16 width;
    /* 0x0002 */ u16 height;
    /* 0x0004 */ u8 filler_0004[0x051A - 0x0004];
    /* 0x051A */ u8 unit[0x1432 - 0x051A];
    /* 0x1432 */ u8 cell[0x417A - 0x1432];
    /* 0x417A */ u16 rowOffset[1];
};
#define MAP ((struct Unk20754Map *)gUnknown_08499590)

void sub_08020754(u8 *dst)
{
    int x, y;

    for (y = 0; y < MAP->height; y++)
    {
        for (x = 0; x < MAP->width; x++)
        {
            if (((MAP->cell[MAP->rowOffset[y] + x] & 0x1f) == 4
              || (MAP->cell[MAP->rowOffset[y] + x] & 0x1f) == 0x13)
             && (MAP->unit[MAP->rowOffset[y] + x] == 0
              || (u8)(gUnknown_08499594[MAP->unit[MAP->rowOffset[y] + x]].type - 0x10) > 4))
                dst[MAP->rowOffset[y] + x] = 0;
            else
                dst[MAP->rowOffset[y] + x] = 1;
        }
    }
}
