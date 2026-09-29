#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C5A4.
 * sub_0804C5A4 @ 0x0804C5A4
 */

/* MATCHED -- wave 86 (W86-G), 312/312 bytes, exit 0 from try_match.
 * Parked since wave 43; "CONFIRMED IMMOVABLE" in wave 77.
 *
 * PROMOTION NOTE: try_match reports
 *   relocs: name different symbols that resolve to the same address
 *     +0x110  original gUnknown_0813605C  candidate .rodata
 * which is a MATCH. This function's data/promoted.json entry needs
 *   "rodata": ["0x0813605C"]
 * and then tools/split_rodata.py + tools/gen_lds.py before the build.
 *
 * HOW IT CLOSED -- TWO LEVERS, IN THIS ORDER, AND THE SECOND ONLY WORKS
 * BECAUSE OF THE FIRST.
 *
 * 1. W86-C's DELAY LEVER took it 86.9% -> 99.4%. The residual was a base
 *    address constant loaded TWELVE instructions early, and W73-H had shown
 *    that every allocno lever then known moves a reference EARLIER. The lever
 *    that moves one LATER without reassociating the index is: BIND THE WHOLE
 *    INDEX EXPRESSION TO ITS OWN LOCAL, IN THE ROM'S OWN TERM ORDER, IN A
 *    STATEMENT BEFORE THE REFERENCE, then write the access with the base
 *    added to that local (`off` below). -fforce-addr emits the FORCE_REG of
 *    the array's address when the ARRAY_REF is expanded, which is before its
 *    index subtree; once the index is already a live pseudo from an earlier
 *    statement, the only thing left to expand at the reference is the base
 *    load and one add, so the load lands at the point of use. W73-E's
 *    byte-offset-first spelling failed because it reassociated the index
 *    INLINE; the same reassociation as its OWN STATEMENT costs nothing.
 *
 * 2. THE LAST 2 BYTES were one register -- ROM `ldr r2` / candidate `ldr r0`
 *    -- and W86-C measured NINE hand spellings that all gave r0, with only a
 *    dead-index form giving r2 and then at the wrong POSITION (position and
 *    register traded). decomp-permuter, chained from the 99.4% draft, found
 *    it in 1,080 iterations: `s16 (*new_var)[6][2] = gUnknown_085D6C94;`
 *    bound in its own statement BEFORE the guard, with the access written
 *    through `new_var`. Binding the base's ADDRESS to a local EARLY, while
 *    the index stays bound late, splits the two references into two allocnos
 *    created in the ROM's order, and the base wins r2.
 *
 * DO NOT TIDY. The `new_var` declaration, its separate assignment statement,
 * its position among the locals and the fully-parenthesised `off` sum are the
 * permuter's edit and are load-bearing. Folding the assignment into the
 * declaration, sinking it to the point of use, or restoring the natural
 * `gUnknown_085D6C94[a][b][c]` array spelling all reopen either the register
 * or the twelve-instruction placement.
 *
 * THE GENERAL RULE (docs/agbcc-codegen.md, appended this wave): AN EARLY
 * ADDRESS BINDING AND A LATE INDEX BINDING ARE INDEPENDENT LEVERS THAT
 * COMPOSE -- the index local fixes WHERE the base load lands, the address
 * local fixes WHICH REGISTER it lands in. Nine spellings of the index
 * statement could not reach the register because they all moved the same
 * single allocno.
 *
 * Two earlier permuter negatives are recorded against this function; both
 * were taken against the 86.9% draft and neither was evidence about the
 * 99.4% one. Wave 86 re-confirms wave 83's lifecycle rule: CHAIN THE
 * PERMUTER FROM THE NEW BEST, a run from a superseded base says nothing.
 *
 * Settled earlier, still true: the operand order of the gUnknown_03004508
 * compare with the C14 byte bound to its own `u8 k` first; gUnknown_085D6C94
 * as the ROW symbol rather than the gUnknown_085D6C88[..].unk0c member view
 * (the member view costs a runtime `adds rB,#0xc` this ROM does not have --
 * screened against the matched twin src/decomp/c_0804D4E8.c); the toolchain
 * axis. */

#include "global.h"

void SubmarineFigure_Loop(void)
{
  u16 side;
  u16 slot;
  s16 (*new_var)[6][2];
  u8 k;
  int off;
  side = gUnknown_03001470[gUnknown_03001FBC].unk30;
  slot = gUnknown_03001470[gUnknown_03001FBC].unk34;
  sub_080156C4(gUnknown_03001FBC);
  new_var = gUnknown_085D6C94;
  k = gUnknown_08553C14[gUnknown_0300450C][side];
  if (gUnknown_03004508 == (gUnknown_08553318[gUnknown_020296B0[side].unk18] + k))
  {
    if (gUnknown_020296B0[side].unk0c[gUnknown_020296B0[side].unk18] != 0xff)
    {
      SpawnThirdEffectAndProjectile(side, gUnknown_020296B0[side].unk18);
      gUnknown_02029A10[side].entries[slot].unk1e++;
      off = (((gUnknown_020296B0[side].unk1a & 1) * 2) + ((gUnknown_03004580[side][2] - 1) * 4)) + (gUnknown_03004580[side][1] * 24);
      sub_0803B48C(*((const s16 *) (((const u8 *) new_var) + off)));
      gUnknown_020296B0[side].unk1a++;
    }
    gUnknown_020296B0[side].unk18++;
  }
  sub_0804CA98(side, slot, gUnknown_03001FBC);
  StepFigureHitFlash2(side, slot, gUnknown_03001FBC);
  RidePartOnFigure(side, slot, gUnknown_03001FBC);
  StepFigureSlide(side, slot);
}
asm(".global sub_0804C5A4\n.thumb_set sub_0804C5A4, SubmarineFigure_Loop\n");
