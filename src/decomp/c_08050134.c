#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050134.
 * sub_08050134 @ 0x08050134, FootFigure_Loop @ 0x080501DC, FigureTileHook_WholePose @ 0x08050364
 */

#include "hardware.h"

/* MATCHED, and the FIRST of a byte-identical pair -- FigureTileHook_WholePose is the same
 * function again, same instruction stream and same pool words, found by
 * tools/overlap_screen.py rather than by reading the block. Both are
 * zero-fan-in callbacks.
 *
 * It re-syncs one OBJ's tile number against the slot's stashed attributes: read
 * the current attributes with CopySlotSpriteAttrs, take the 10-bit delta between the
 * caller's attr2 and them, write the fetched tileNum back into the caller's
 * attr2, and if that delta is new for this proc AND the proc still owns the
 * gUnknown_02029A10 entry, publish it.
 *
 * The second parameter is `u16 *` and NOT `struct OamData *`, which is the one
 * derivation that cost anything. The ROM subtracts the RAW halfword at +4
 * (`ldrh r3,[r1,#4]; subs r0,r3,r0`) with no bitfield extraction on that side,
 * and rebuilds it with an explicit `movs #0xfc; lsls #8; ands; adds`. Through
 * `struct OamData *` the `p->tileNum` read emits its own `lsls #22; lsrs #22`
 * pair that force_to_mode does not remove, and the store mask comes out as a
 * 32-bit `ldr =-0x400` from the pool instead of the ROM's 16-bit 0xFC00. The
 * local `oam` IS a struct OamData -- only the caller's buffer is the raw
 * halfword view.
 *
 * `oam.tileNum` is read TWICE rather than bound: the ROM keeps `attr2 << 22` in
 * one register and re-extracts it with a second `lsrs #22` at the store, which
 * a `t = oam.tileNum;` local collapses into one. And the store is
 * `(p[2] & 0xFC00) + oam.tileNum`, in that order -- the mask term first, which
 * is what puts `ands` ahead of the second `lsrs`. */
void sub_08050134(s16 a, u16 *p)
{
    struct OamData oam;
    u16 v;
    u16 w;
    u16 side;
    u16 slot;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);

    v = (p[2] - oam.tileNum) & 0x3FF;
    w = v >> 4;

    side = gUnknown_03001470[a].unk30;
    slot = gUnknown_03001470[a].unk34;

    p[2] = (p[2] & 0xFC00) + oam.tileNum;

    if (v != gUnknown_03001470[a].unk28
        && a == gUnknown_02029A10[side].entries[slot].unk18)
    {
        gUnknown_03001470[a].unk28 = v;
        StreamWholePose(side, slot, w);
    }
}

/* Sibling of the matched sub_0804FCA4 (src/decomp/c_0804FCA4.c), which supplies
 * the whole head verbatim -- the gUnknown_03001470 slot reads, the
 * sub_080156C4 state, the gUnknown_085D6C88 sound lookup and the
 * `e * sizeof(entry) + c * sizeof(group) + (u8 *)base` pointer idiom. What is
 * new is a six-step per-slot counter (gUnknown_02029B94, stepped by
 * gUnknown_02029B80 and wrapped at 6) that indexes a per-phase (x, y) offset
 * pair out of gUnknown_085523B0.
 *
 * ONE spelling is load-bearing and it is a wave-20 rule
 * (docs/agbcc-codegen.md): `gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]]`.
 * Written as the plain `gUnknown_02028E5C[c][1]`, fold associates the +2 onto
 * the symbol -- `(base + 2) + c * 4` -- and pays an `adds` that the ROM puts in
 * the `ldrh` displacement. Binding the row fixes that, but only if the binding
 * happens inside the expression: as its own statement it is emitted twelve
 * instructions early, ahead of the two gUnknown_085523B0 addresses the ROM
 * computes first. `row` is never read again.
 *
 * `dx` and `dy` are u16 locals rather than inline expressions because the ROM
 * truncates the y difference to 16 bits BEFORE adding it to the field (`lsls
 * #16` early, `lsrs #16` at the use, split around the entry-pointer
 * computation), which no inline spelling reproduces -- with the whole
 * expression in the assignment, combine folds the truncation into the `strh`. */

void FootFigure_Loop(void)
{
    struct Unk02029A10 *entry;
    u16 c, e, t, n;
    u16 dx, dy;
    u16 *p1, *p2;
    u16 *row;

    c = gUnknown_03001470[gUnknown_03001FBC].unk30;
    e = gUnknown_03001470[gUnknown_03001FBC].unk34;
    t = sub_080156C4(gUnknown_03001FBC);
    StepFigureSlide(c, e);

    gUnknown_02029B94[c][e] += gUnknown_02029B80[c][e];
    if (gUnknown_02029B94[c][e] == 6)
    {
        gUnknown_02029B94[c][e] = 0;
        gUnknown_02029B80[c][e] = 0;
    }

    p1 = gUnknown_084C3F70[c];
    p2 = gUnknown_084C3F78[c];

    if (t == 0x1C)
    {
        PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[c][1]]
                                      .unk0c[gUnknown_03004580[c][2] - 1]
                                      [gUnknown_020296B0[c].unk1a & 1]);
        gUnknown_020296B0[c].unk1a++;
    }

    if (t == 3)
        gUnknown_03004544++;

    dx = gUnknown_085523B0[c][gUnknown_02029B94[c][e] * 2];
    dy = gUnknown_085523B0[c][gUnknown_02029B94[c][e] * 2 + 1]
         - gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];

    entry = (struct Unk02029A10 *)(e * sizeof(struct Unk02029A10)
                                   + c * sizeof(struct Unk02029A10Group)
                                   + (u8 *)gUnknown_02029A10);

    entry->x += dx;
    entry->y += dy;

    SetSlotSpritePosition(gUnknown_03001FBC, entry->x - *p1, entry->y - *p2);
}

/* MATCHED. Byte-for-byte the same function as sub_08050134 -- identical
 * instruction stream and identical pool words, so this is transcription, not a
 * second derivation. Read sub_08050134's comment; the `u16 *` second parameter
 * and the twice-read `oam.tileNum` are the only two things in it that are not
 * guessable. */
void FigureTileHook_WholePose(s16 a, u16 *p)
{
    struct OamData oam;
    u16 v;
    u16 w;
    u16 side;
    u16 slot;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);

    v = (p[2] - oam.tileNum) & 0x3FF;
    w = v >> 4;

    side = gUnknown_03001470[a].unk30;
    slot = gUnknown_03001470[a].unk34;

    p[2] = (p[2] & 0xFC00) + oam.tileNum;

    if (v != gUnknown_03001470[a].unk28
        && a == gUnknown_02029A10[side].entries[slot].unk18)
    {
        gUnknown_03001470[a].unk28 = v;
        StreamWholePose(side, slot, w);
    }
}

asm(".global sub_08050364\n.thumb_set sub_08050364, FigureTileHook_WholePose\n");
asm(".global sub_080501DC\n.thumb_set sub_080501DC, FootFigure_Loop\n");
