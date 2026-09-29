#include "global.h"
#include "hardware.h"

/* PARKED at 38.7%, candidate is 684 bytes against 680 (+4). Wave 78, W78-B.
 *
 * The instruction ORDER matches the ROM from the prologue to the `bx`; every
 * remaining difference is register allocation plus which address constants
 * agbcc parked in its `-fforce-addr` .rodata pool. Concretely:
 *
 *   +2  gUnknown_0300453C came out as `ldr rA,=.LC; ldr rB,[rA]` where the ROM
 *       has a plain inline pool word.
 *   +2  gUnknown_0300451C, same.
 *   -2  gUnknown_03004580 comes out as a plain pool word where the ROM has the
 *       .rodata form (its word is gUnknown_081360D4).
 *   +2  the `~0x0c` objMode mask is hoisted into sl and reused at the second
 *       objMode write; the ROM rematerialises `movs r0,#0xd; rsbs r0,r0,#0`.
 *   -2  the ROM copies the 0x3FF mask (`adds r0,r2,#0`) before the `ands`.
 *   -2  the ROM copies v (`adds r0,r1,#0`) before `cmp r0,#0x17`.
 *   The tail then differs only in which of &gUnknown_02029A10 /
 *   &gUnknown_08553C18 / &gUnknown_085D6A48 live in callee-saved registers
 *   versus being reloaded from the pool at each statement.
 * Every one of those is the SAME symptom read from both ends: this candidate
 * carries FEWER simultaneously-live values than the original, so agbcc has
 * spare callee-saved registers and spends them on constants and addresses the
 * ROM rematerialises. The missing live value has not been identified.
 * Wave 63: binding the tileNum source to a u16 local does not produce the
 * missing mask copy or supply the missing live value; configured remains
 * 692/680 (+12), 434/680 bytes differ (36.2%).
 *
 * Axes RULED OUT (each measured, do not re-spend them):
 *   - dx/dy MUST be `s16`. A controlled probe (four spellings of the
 *     conditional negate) shows only a promoted `s16` local gives the ROM's
 *     `lsls #0x10; rsbs #0; lsrs #0x10`; `u16` and `int` both give
 *     `neg; lsl; lsr`, in that order, which is a different instruction stream.
 *   - The gUnknown_085D6A48 COLUMN reads must go through a struct-row cast, not
 *     `gUnknown_085D6A48[i][j]` on the declared `u16 [][12]`. The ROM keeps the
 *     column offset in the load displacement (`adds r0,r0,r2; ldrh r0,[r0,#2]`
 *     and `ldrh r1,[r0,#0x14]`); the flat subscript reassociates the offset onto
 *     the symbol and pays an extra `adds` per read. Same finding the wave-17
 *     note on that global predicted, now confirmed at three columns.
 *   - The COMMA ANCHOR on `tbl` is required: without it the subscript is
 *     evaluated first and gUnknown_03004580's address is materialised ahead of
 *     gUnknown_085D6A48's, which is the wrong pool order. Same lever as
 *     src/decomp/c_080505A4.c.
 *   - Operand order inside the two big sums is `entries[j].x + <08553C18 term>
 *     + dx`: the ROM computes the gUnknown_02029A10 element pointer SECOND
 *     (right after the destination address) and loads the 08553C18 halfword
 *     FIRST, which is the "address computed first is loaded second" reversal.
 *     Writing the 08553C18 term first moves its whole chain ahead of the entry
 *     pointer.
 *   - gUnknown_0202972C and gUnknown_02029710[..].unk3a are BOTH real: the ROM
 *     holds `=gUnknown_0202972C` for the first store and `=gUnknown_02029710`
 *     plus `adds #0x3a` for the second, in one function.
 *   - The 0x08136 pool words were dumped out of baserom.gba: 081360CC ->
 *     02029710, 081360D0 -> 085D6A48, 081360D4 -> 03004580. All three are
 *     `-fforce-addr` .rodata address constants, not globals.
 */
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u8 filler_0a[0x0a];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 filler_16[0x02];
};

void sub_080506B0(void)
{
  struct OamData oam;
  u16 v;
  struct Unk85D6A48Row *tbl;
  u16 (*new_var2)[8];
  s16 dx;
  u16 new_var;
  s16 dy;
  u16 *sidep;
  u16 *new_var3;
  u16 *slotp;
  u16 lv0;
  new_var3 = &gUnknown_0300451C;
  sidep = &gUnknown_0300453C;
  slotp = new_var3;
  sub_0801566C(gUnknown_03001FBC, (struct UnkVec *) (&oam));
  gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
  gUnknown_03001470[gUnknown_03001FBC].unk30 = *sidep;
  gUnknown_03001470[gUnknown_03001FBC].unk34 = *slotp;
  oam.hFlip = (*sidep) ^ 1;
  oam.paletteNum = 8;
  lv0 = gUnknown_02029710[*sidep].unk00;
  oam.tileNum = lv0;
  oam.objMode = 0;
  new_var2 = gUnknown_03004582;
  if ((((tbl = (struct Unk85D6A48Row *) gUnknown_085D6A48, tbl[v = *((u16 *) ((((u8 *) gUnknown_03004580) + 2) + ((*sidep) * 16)))].unk02)) == 2) && ((v == 0x17) || (v == 0x11)))
  {
    do
    {
      sub_08012358();
      gUnknown_030030E0.bits.effect = 1;
      gUnknown_030030E0.bits.target2_enable_obj = 1;
      gUnknown_030030E0.bits.target2_enable_bg2 = 1;
      gUnknown_030030E0.bits.target2_enable_bg3 = 1;
      gUnknown_03002020 = 0xF;
      gUnknown_03002B28 = 0xA;
      oam.objMode = 1;
    }
    while (0);
  }
  oam.priority = 3;
  sub_08015608(gUnknown_03001FBC, *((struct UnkVec *) (&oam)));
  sidep = &gUnknown_0300453C;
  slotp = &gUnknown_0300451C;
  dx = gUnknown_085D81E8[gUnknown_03004582[*sidep][0]][gUnknown_020296B0[*sidep].unk18].unk00[*sidep];
  if ((*sidep) != 0)
  {
    dx = -dx;
  }
  new_var = *sidep;
  dy = gUnknown_085D81E8[gUnknown_03004582[*sidep][0]][gUnknown_020296B0[new_var].unk18].unk04;
  gUnknown_0202972C[*sidep][*slotp] = (gUnknown_02029A10[*sidep].entries[*slotp].x + gUnknown_08553C18[((struct Unk85D6A48Row *) gUnknown_085D6A48)[((struct Unk85D6A48Row *) gUnknown_085D6A48)[gUnknown_03004582[*sidep][0]].unk00].unk14].unk08[*sidep]) + dx;
  gUnknown_02029710[*sidep].unk3a[*slotp] = (gUnknown_02029A10[*sidep].entries[*slotp].y + gUnknown_08553C18[((struct Unk85D6A48Row *) gUnknown_085D6A48)[((struct Unk85D6A48Row *) gUnknown_085D6A48)[new_var2[*sidep][0]].unk00].unk14].unk10) + dy;
  sub_08050528(*sidep, gUnknown_03001FBC, gUnknown_0202972C[*sidep][*slotp], gUnknown_02029710[*sidep].unk3a[*slotp]);
}

/* WAVE 78 (W78-B).  Improved from 692/680 (+12), 36.2%, to 684/680
 * (+4), 38.7%, first difference +0xa.  Taking the addresses of
 * gUnknown_0300453C and gUnknown_0300451C into `sidep`/`slotp`, and assigning
 * those pointers again at the boundary after sub_08015608, changes the two
 * scalar globals from by-name memory references into address-only uses.
 * That removes exactly their two unwanted force-address words while preserving
 * the ROM's plain pool words and per-region value reloads.
 *
 * The remaining +4 is the one missing force-address word for
 * gUnknown_03004580: the ROM has gUnknown_081360D4 and no plain word; this
 * draft still has the plain word.  The already-exact gUnknown_02029710 and
 * gUnknown_085D6A48 choices remain force-addressed.  Newly ruled out from this
 * improved fixpoint: `v` as int/u32/s16, nested-if and switch spellings for
 * the 0x17/0x11 test, and a distinct copied comparison local.  All leave the
 * pool unchanged; the copy is coalesced, so none produces the ROM's
 * `adds r0,r1,#0` or changes gUnknown_03004580's surviving use. */

/* WAVE 77 (W77-A): re-measured, +12 bytes / 36.2% (the brief quoted +2; the
 * measured delta is +12).  Checked against the sub_0804CA98 finding of this
 * wave and it does NOT apply here: this function's ROM pool holds a PLAIN
 * `gUnknown_02029A10` word (_08050948) referenced twice, with no
 * `-fforce-addr` .rodata word for it anywhere, so the array spelling in the
 * body is correct and must not be converted to the `(u8 *) + sizeof` idiom.
 * The ROM instead spends its extra register on the CONSTANT 0xb4
 * (`movs r1,#0xb4; mov r8,r1; mov r0,r8; muls r0,r4,r0`), which is the same
 * "one more live value, and it is a constant" symptom the note above records.
 * Residual kind 3 (allocation).  No new axis; not re-spent.
 */

/* WAVE 77 (W77-H).  Re-verified +12 bytes / 36.2%, first difference at +0xc.
 * Draft unchanged.  The +12 is a force-addr WORD COUNT, and here is the whole of
 * it:
 *   ROM pool:       plain gUnknown_0300453C, plain gUnknown_0300451C, THREE
 *                   force-addr words (gUnknown_081360CC/D0/D4), and NO plain
 *                   gUnknown_03004580 word at all.
 *   candidate pool: FOUR force-addr words, plus plain gUnknown_0300453C, plain
 *                   gUnknown_0300451C AND plain gUnknown_03004580.
 * So this candidate force-addrs two symbols the ROM keeps plain and fails to
 * force-addr the one symbol the ROM does.  The rest of the diff follows: the two
 * surplus `ldr rN,[rM,#0]` indirections at the unk30/unk34 stores, the
 * `mov sl, r2 / mov r0, sl` needed to park the -13 constant, and the ROM's
 * uncoalesced `adds r0, r1, #0` ahead of `cmp r0, #23`.
 *
 * MEASURED AND RULED OUT this wave: giving gUnknown_03004580 a SECOND by-name
 * read (re-reading `gUnknown_03004580[side][1]` in the `v == 0x17` test instead
 * of caching it at the subscript) is BYTE-IDENTICAL -- same size, same 36.2%,
 * same first difference.  Nothing stores to memory between the two reads so CSE
 * collapses them; per wave 37 W37-K3 the count force-addr settles on is the one
 * that SURVIVES CSE, and you cannot add a surviving reference by writing one.
 *
 * The W77-A note above is confirmed: this function's gUnknown_02029A10 word is a
 * PLAIN literal in the ROM, so it must keep the array spelling.  Residual
 * kind 3. */

/* WAVE 85 (W85-D).  Re-read the ROM head around the v-read; one structural
 * fact to add for the next attempt, no probe run (draft preserved verbatim):
 * the ROM's v-read applies the [1] column offset to the BASE BEFORE the row
 * index -- `ldr r1,[r4](&03004580 via 081360D4); adds r1,#2; ...; adds r0,r0,r1
 * (side*16); ldrh r1,[r0]` -- i.e. the address is plus(plus(base,2),side*16),
 * while this draft's `gUnknown_03004580[(*sidep)][1]` expands to
 * plus(plus(base,side*16),2) and lands the +2 in the `ldrh` displacement. That
 * is the `p[X + C]` vs `*(p + X + C)` shape from the brief, untested here; a
 * spelling that binds base+2 first (e.g. reading through
 * `(u16 *)((u8 *)gUnknown_03004580 + 2)`) is the untried axis. It may also be
 * what makes agbcc force-addr this once-used address: the C5A4 force-addr rule
 * ties the .rodata word to an address pseudo whose live range spans a region,
 * and a base+2-bound pseudo would exist from the anchor to the load. */
