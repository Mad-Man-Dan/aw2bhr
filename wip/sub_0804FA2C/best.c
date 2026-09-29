#include "global.h"

/* PARKED at 636 bytes against 632 (+4) -- see data/parked.json.
 *
 * Same subsystem and type vocabulary as sub_0804D290 / sub_0804DCA8 (both
 * matched) but a genuinely different function, normalised shape ratio 0.276:
 * the OAM write happens before the position seeding, the tile number carries a
 * table term, sub_0804BCB8 gets a real second argument, and the position table
 * is indexed two-dimensionally through gUnknown_03004580[i][5].
 *
 * Settled, do not re-derive:
 *   - the row of gUnknown_085D6A48 is a STRUCT, not `u16 [12]`: the ROM keeps
 *     column 9 in the `ldrh` displacement, which an array row cannot emit
 *     because fold reassociates the constant onto the base. Worth 4 bytes. The
 *     cast is local because retyping the global is not free at column 0 -- see
 *     include/unknown-globals.h.
 *   - `(rN = pos[k])[i * 5 + j]` and not `pos[k][i * 5 + j]`: the ROM builds the
 *     row address first. Worth 12 bytes; same lever as c_0804EEFC.c.
 *   - ONE BINDING LOCAL PER STATEMENT, not one reused across all four. A single
 *     `row` assigned in four statements is one pseudo whose live range spans all
 *     of them, and it takes r7 -- a low callee-saved register the ROM spends on
 *     the constant 180. Four locals each die inside their own statement and take
 *     a scratch, which is what the ROM does. Worth 4 bytes, and it is why
 *     c_0804EEFC.c uses `e1` and `e2` rather than one name twice.
 *   - `* 0x100` and not `<< 8` (4 bytes).
 *   - the three gUnknown_03004580 columns must be spelled on the GLOBAL name,
 *     not through a pointer local: the ROM CSEs `&gUnknown_03004580[0][5]` into
 *     its own register across all four entry writes, which only the
 *     base-plus-constant form the global spelling produces can do; a pointer
 *     local collapses it to an `ldrh` displacement, which cannot be shared.
 *
 * THE REMAINING 4 BYTES. `&gUnknown_03004580` is now materialised early and
 * register-allocated, but into `sl` and ONE INSTRUCTION BEFORE
 * `ldr r5, =gUnknown_085D6A48` where the ROM has it in `r8` one instruction
 * AFTER. That is the identical failure mode the twins had, and there the fix was
 * a comma operator inside the other argument's index to move the reference one
 * slot later (see "Of two address constants" in docs/agbcc-codegen.md). Here the
 * same comma costs `&gUnknown_0300453C` its r6 and is a net loss, so the twins'
 * fix does not transfer as-is -- FA2C has one more simultaneously live value
 * than they do.
 *
 * The tile multiply (`adds r3, r1, #0; muls r3, r0; adds r0, r3, #0` where the
 * ROM has a bare `muls r0, r1`) is downstream of that, not an independent error:
 * both operand orders and a temporary for the table read give byte-identical
 * output.
 *
 * Measured and rejected: `do { } while (0)` round the tileNum statement, the
 * call statement, the last statement, or the four entry writes (+8 to +20);
 * the comma anchor in the tileNum statement or in the call statement (+4, worse
 * bytes); anchoring through `pos`, `row`, an `int` or a `register` local (all
 * bit-identical -- the anchor's variable is irrelevant, only its position);
 * `old_agbcc` and `-fno-force-addr` (the latter is bit-identical on the twins,
 * so these `ldr`s are not -fforce-addr artefacts); two 300 s+ decomp-permuter
 * runs, whose best edits were both noise (`new_var = 5` used as an array index).
 *
 * WAVE 79 (W79-F). Re-measured: the DRAFT is 636 bytes (+4) at 46.2%, first
 * difference at +0xc. The 72.9% that reached this wave's brief is best.c's, not
 * this file's -- best.c is a different-sized variant and its score has been
 * quoted as this function's for at least two waves.
 *
 * THE TWINS' FIX WAS RE-TESTED IN ITS EXACT sub_0804DCA8 FORM AND IS WORSE, so
 * the "read the matched twins first" lead is now closed rather than open:
 *   - the twins' COMBINATION (`do { } while (0)` round the sub_08057D44
 *     statement WITH `(sideDataAnchor = gUnknown_03004580, ...)` inside argument 1's
 *     subscript, which is what c_0804DCA8.c actually does and which this
 *     function's earlier notes had only ever tried as two separate changes):
 *     652 bytes (+20), 16.6%.
 *   - the comma anchor alone, moved INSIDE the tileNum statement's subscript,
 *     between the gUnknown_085D6A48 reference and the gUnknown_03004582 one,
 *     which is exactly where the ROM's `mov r8, r3` sits: 636 bytes (+4),
 *     26.7% -- same size, 20 points worse.
 * Both confirm the entry's reading: FA2C carries one more simultaneously live
 * value than the twins, and the lever that buys &gUnknown_03004580 its register
 * there costs &gUnknown_0300453C its r6 here. This is an allocno tie under a
 * different live set, not a source construct.
 *
 * ================= WAVE 85 (W85-D) ADDITION -- the ROM's full register map,
 * read off the disassembly so the next wave does not have to re-derive it:
 *   r8 = &gUnknown_03004580 (plain base; loaded at the tileNum statement,
 *        BETWEEN `ldr r5,=gUnknown_085D6A48` and `ldr r1,=gUnknown_03004582`,
 *        live to the end: sub_08057D44 arg2 via +6, the four entry writes'
 *        sb base via +10, and the sub_08015928 arg via +4 + c*16)
 *   sb = &gUnknown_03004580[c*16+10] (the entry-write CSE base, formed AFTER
 *        the sub_08057D44 call: `movs r2,#0xa; add r2,r8; mov sb,r2`)
 *   sl = &gUnknown_02029A10 (loaded right after the same call)
 * The parked text's "materialised into sl" describes the CANDIDATE only; the
 * ROM never puts &gUnknown_03004580 in sl. The candidate's own map: the early
 * sl load at +0xc and the extra `ldr r3,[pc]; mov r8,r3` pair AT the
 * sub_08057D44 call site (+4 bytes) are the same two pseudos created in the
 * opposite order. W79-F measured the anchor at the ROM's exact position
 * (26.7%) and the twins' combination (+20, 16.6%), so the position axis is
 * closed; what is left is an allocno-order tie between the two
 * gUnknown_03004580 base pseudos (plain vs [c][5]) under this function's live
 * set -- wave-77 class, nothing behind it that the project has a lever for. */

struct UnkPosPair
{
    u16 x;
    u16 y;
};

struct Unk85D6A48Row
{
    u16 unk00;
    u8 filler_02[0x10];
    u16 unk12;
    u8 filler_14[0x04];
};

void sub_0804FA2C(void)
{
  u16 (*sideData)[8];
  struct OamData oam;
  u16 pal;
  u16 prio;
  struct UnkPosPair (*pos)[10];
  struct UnkPosPair *row;
  u16 (*sideDataAnchor)[8];
  sub_0801566C(gUnknown_03001FBC, (struct UnkVec *) (&oam));
  gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
  gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
 do { oam.hFlip = gUnknown_0300453C ^ 1; pal = gUnknown_08551D0C[gUnknown_0300453C][0]; oam.paletteNum = pal; oam.tileNum = (gUnknown_0300453C * 0x100) + (((struct Unk85D6A48Row *) gUnknown_085D6A48)[sideDataAnchor = gUnknown_03004580, gUnknown_03004582[gUnknown_0300453C][0]].unk12 * gUnknown_0300451C); prio = gUnknown_0855239C[(gUnknown_0300453C * 2) + gUnknown_0300450C]; oam.priority = prio; sideData = gUnknown_03004580; sub_08015608(gUnknown_03001FBC, *((struct UnkVec *) (&oam))); pos = (struct UnkPosPair (*)[10]) sub_08057D44(gUnknown_085D6A48[gUnknown_03004582[gUnknown_0300453C][0]][0], sideData[gUnknown_0300453C][3]); gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 = (row = pos[gUnknown_03004580[gUnknown_0300453C][5]])[(gUnknown_0300453C * 5) + gUnknown_0300451C].x; gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x = (row = pos[gUnknown_03004580[gUnknown_0300453C][5]])[(gUnknown_0300453C * 5) + gUnknown_0300451C].x; gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 = (row = pos[gUnknown_03004580[gUnknown_0300453C][5]])[(gUnknown_0300453C * 5) + gUnknown_0300451C].y; } while (0);
  gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y = (row = pos[gUnknown_03004580[gUnknown_0300453C][5]])[(gUnknown_0300453C * 5) + gUnknown_0300451C].y;
  sub_0804BCB8(gUnknown_0300453C, gUnknown_0300451C, 0, 0);
  sub_080155C0(gUnknown_03001FBC, gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x, gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
  sub_08015928(gUnknown_03001FBC, gUnknown_08552FB8[gUnknown_03004582[gUnknown_0300453C][0]][sideData[gUnknown_0300453C][2]][2]);
}
