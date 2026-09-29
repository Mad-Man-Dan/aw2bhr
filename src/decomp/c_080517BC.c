#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080517BC.
 * MissileHitEffect_Loop @ 0x080517BC
 */

/* PARKED SIZE-EXACT at 356/356 bytes, 21.6% identical, first difference at
 * +0xa. Wave 36, W36-C, derived from scratch.
 *
 * Size-exact on the first try that used the right struct spelling, so every
 * statement is present and in the right order; what differs is almost entirely
 * REGISTER ALLOCATION, which is why the percentage is low and the size is not.
 *
 * SETTLED, and most of it is new declaration work now recorded in
 * include/unknown-globals.h:
 *   - gUnknown_081360FC / 08136100 / 08136104 are NOT globals. They are agbcc's
 *     own `-fforce-addr` address-constant pool -- three consecutive words
 *     holding &gUnknown_03001470, &gUnknown_03001FBC and &gUnknown_020298E0,
 *     dereferenced in baserom.gba. The honest spelling (name the three real
 *     objects) is right and the build emits `.rodata` relocations with addends
 *     0, 4, 8 in the ROM's positions. The `mov sl,r3` / `mov sb,r2` that keeps
 *     the SLOT ADDRESSES in callee-saved registers and reloads the pointer at
 *     each use is the `**`-local tell from the wave-35 note, and it appears on
 *     its own -- do not try to reproduce it with a `**` in the C.
 *   - struct Unk020298E0 gained unk4e/unk58/unk62/unk6c/unk76/unk80/unk8a,
 *     which tile the old filler_44 exactly; gUnknown_02029924 is the +0x44
 *     overlay symbol, and it is u16 (an s16 element makes the
 *     `*p + gUnknown_03001470[proc].unk28` addition genuinely 32-bit and turns
 *     the ROM's `ldrh` into `ldrsh`).
 *   - Two 8.8 fixed-point axes stepped exactly as struct Unk02029710's are:
 *     `unk58 += unk62; g02029924 += (s16)unk58 >> 8` and
 *     `unk6c += unk76; unk4e += (s16)unk6c >> 8`, the `>> 8` spelled
 *     `lsls #0x10; asrs #0x18`.
 *   - EndMissileHitEffect had a promoted definition (src/decomp/c_08051920.c) and no
 *     declaration anywhere; its signature is now in unknown-functions.h.
 *
 * THE REMAINING DIFFERENCE, from the +0xa diff:
 *   - The ROM keeps `gUnknown_020298E0`'s dereferenced base in a LOW register
 *     (`adds r1, r2, #0` at each member) where this holds it in `ip`
 *     (`mov r1, ip`) -- same instruction count, different encoding.
 *   - The ROM spills `&gUnknown_02029924[side][slot]` to the stack slot
 *     (`str r1,[sp]` / `ldr r4,[sp]`) because it has one more value live; this
 *     candidate keeps it in a register. That is the wave-17 diagnostic read
 *     backwards: the ROM has MORE simultaneously live values here, so the
 *     original bound something this draft recomputes -- look for the binding,
 *     not for a spill.
 *   - The gUnknown_02029924 address is formed as `(slot*2 + base) + side*0x90`
 *     here and as `idx + base` in the ROM, i.e. the ROM reuses the CSEd
 *     `slot*2 + side*0x90`.
 * Wave 63 bound the final unk4e address to a second pointer local to test the
 * missing-live-value diagnosis. It stayed size-exact but moved away to 19.9%
 * (285/356 bytes differ), so that local is not the missing binding and was
 * reverted.
 * The `struct Unk85D6A48Row` cast below is load-bearing: the flat
 * `gUnknown_085D6A48[j][2]` folds the +4 into the base with `adds r2,#4` where
 * the ROM keeps it in the `ldrh` displacement. */

/* MATCHED, wave 77 (W77-A).  Was PARKED SIZE-EXACT at 21.6% since wave 36,
 * with further attempts in waves 63 and 66.  Three levers, in this order:
 *   1. Drop the `p` binding and recompute `&gUnknown_02029924[side][slot]` at
 *      both use sites.  `p` spanned the two statements and took a callee-saved
 *      register the ROM does not spend; recomputing shortens the live range.
 *      21.6% -> 91.9%, size-exact throughout.  (Found by decomp-permuter.)
 *   2. Comma-anchor `tbl` ahead of the subscript, so &gUnknown_085D6A48's
 *      address constant is created before &gUnknown_03004580's.  Without it
 *      the two pool words come out swapped.  Same lever the note in
 *      work/sub_080506B0 records, and the one in src/decomp/c_080505A4.c.
 *      91.9% -> 96.6%.
 *   3. Bind `gUnknown_03001470[gUnknown_03001FBC].unk28` to a local before the
 *      SetEffectScreenPosition call.  That materialises gUnknown_03001470's address ahead
 *      of the gUnknown_03001FBC read, which is the ROM's `ldr r2,[pc,#76]`
 *      position.  A comma anchor in the same place does NOT do it (94.4%) --
 *      it has to be a plain local binding.  96.6% -> MATCH.
 *
 * PROMOTION REQUIREMENT: this match needs its .rodata pool words placed.  Add
 * to this function's data/promoted.json entry:
 *     "rodata": ["0x081360FC", "0x08136100", "0x08136104"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py before building.
 * trymatch reports `relocs: name different symbols that resolve to the same
 * address` and exits 0; that is the match, not a near-miss.
 *
 * Do not tidy the body.  `*(&gUnknown_02029924[side][slot])` is not the same
 * code as binding it to a pointer -- that is lever 1, and folding it back
 * loses the match.  The unused `u16 *p;` declaration is harmless; the `u32
 * new_var` is lever 3.
 */
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u8 filler_00[4];
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x12];
};

void MissileHitEffect_Loop(void)
{
  struct Unk85D6A48Row *tbl;
  u16 side;
  u16 slot;
  u32 new_var;
  u16 *p;
  side = gUnknown_03001470[gUnknown_03001FBC].unk30;
  slot = gUnknown_03001470[gUnknown_03001FBC].unk34;
  if (gUnknown_020298E0[side].unk80[slot] == gUnknown_020298E0[side].unk8a)
  {
    if (((tbl = (struct Unk85D6A48Row *) gUnknown_085D6A48, tbl[gUnknown_03004580[side][1]].unk04)) == 0)
    {
      gUnknown_020298E0[side].unk26[slot] = 1;
    }
    else
    {
      gUnknown_020298E0[side].unk26[gUnknown_08552148[side]] = 1;
    }
    EndMissileHitEffect(side, slot, gUnknown_03001FBC);
  }
  gUnknown_020298E0[side].unk80[slot]++;
  gUnknown_020298E0[side].unk58[slot] += gUnknown_020298E0[side].unk62[slot];
  *(&gUnknown_02029924[side][slot]) += ((s16) gUnknown_020298E0[side].unk58[slot]) >> 8;
  gUnknown_020298E0[side].unk6c[slot] += gUnknown_020298E0[side].unk76[slot];
  gUnknown_020298E0[side].unk4e[slot] += ((s16) gUnknown_020298E0[side].unk6c[slot]) >> 8;
  new_var = gUnknown_03001470[gUnknown_03001FBC].unk28;
  SetEffectScreenPosition(side, gUnknown_03001FBC, (*(&gUnknown_02029924[side][slot])) + new_var, gUnknown_020298E0[side].unk4e[slot]);
  RaiseHitDoneStageFlag(side, slot, gUnknown_03001FBC);
}

asm(".global sub_080517BC\n.thumb_set sub_080517BC, MissileHitEffect_Loop\n");
