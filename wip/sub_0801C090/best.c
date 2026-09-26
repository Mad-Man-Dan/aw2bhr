/* WAVE 86 (W86-F) -- MIRROR-TWIN AXIS TESTED AND REFUTED. Full measurement in
 * work/sub_0801C090/W86-notes.md. The twin is src/decomp/c_0801BD00.c
 * (sub_0801BD00, matched): a TRUE SHAPE TWIN -- it is this function without the
 * mirror arm, and this function's else-arm IS its loop body. It supplied three
 * constructs and none of them moves anything here:
 *   - `s16 n` for the counter: REFUTED FROM THE ROM, no probe needed. The ROM's
 *     decrement ends `lsrs r1,r0,#0x10`, not `asrs`, so the counter is
 *     UNSIGNED. `u16 count` below is right; do not copy the twin's `s16`.
 *   - the walker as the `void *a3` PARAMETER with casts at each use, instead of
 *     a `u16 *src` local: BYTE-NEUTRAL. agbcc copies a3 out of r2 into a
 *     callee-saved register either way, so no pseudo is created or removed.
 *   - `*dst++; *dst++; *dst = ...; dst += 2;` instead of three `*dst++` and a
 *     bare `dst++`: BYTE-NEUTRAL, combine already merges the two `adds #2`.
 * Both were transplanted together in one compile_probe and reproduced this
 * draft's allocation exactly (a1 r7, a2 r8, walker r4, count r9, dw ip, dst r5,
 * 0x1FF in sl, a4 spilled, `sub sp,#4`). No try_match was spent.
 *
 * The residual is unchanged and the twin cannot reach it: BOTH sides spill a4
 * to [sp]; the ROM ALSO spills count to [sp,#4] and uses `sub sp,#8`. The ROM
 * carries one more simultaneously-live value than any draft has reproduced --
 * NOTES.md hypothesis 2, still the open axis. A true shape twin is not
 * automatically a lever donor; it has to be a twin of the RESIDUAL, and this
 * one is half the size with four fewer live values and never had the problem. */
#include "global.h"

/* Copies a sprite's OAM entries into the gUnknown_03002F2C write cursor,
 * offsetting them by (a1, a2) and the tile base a4, with a horizontal MIRROR
 * when bit 12 of a1 (the OAM h-flip bit) is set.
 *
 * src is [count][count x 3 halfwords of OAM]; dst walks 4 halfwords per entry
 * (attr0, attr1, attr2, pad) while gUnknown_03002F2C is advanced by 8 in step.
 * The global is re-read every iteration because the strh through dst may alias
 * it.
 *
 * gUnknown_0848B56C is the 4x4 OAM {width, height} table in PIXELS (see
 * include/unknown-globals.h); only the width is read, and the mirrored x is
 * a1 - x - width with x the 9-bit signed field of attr1.
 *
 * PARKED -- shape and every instruction are right, register allocation is not.
 * See NOTES.md for the exact remaining diff.
 *
 * WAVE 60 (W60-A). Three things, all in NOTES.md in full:
 *
 * 1. THIS DRAFT IS -16 BYTES AND 14.2%, NOT -12. Measured here. Two briefs have
 *    quoted -12.
 * 2. DO NOT START FROM THIS FILE. `work/sub_0801C090/best.c` holds a 45.83%
 *    spelling; this one is 14.2%. best.c is the file to chain from.
 * 3. THE PERMUTER WAS RUN FOR THE FIRST TIME, THREE CHAINED RUNS, AND IT IS NOT
 *    CONVERGING: 14.2% -> 42.5% -> 44.72% -> 45.83%, no zero score at any point.
 *    The first run is a real jump and its edits are the block/binding levers
 *    (a `do { } while (0)` around the whole loop, `t = 0x100` bound before the
 *    test, `new_var = a1 | src[1]` bound in the else arm) -- the same family
 *    that closed sub_0801C4D4, sub_0801C640 and sub_0801C2DC in this wave. Runs
 *    2 and 3 add about a point each. Compare sub_0801C2DC on the identical
 *    treatment: 81.6 -> 97.1 -> 99.6 -> MATCH. **A chain that gains one point
 *    per run is telling you the residual is not reachable by randomisation;
 *    spend the next budget on the spill hypothesis in NOTES.md, not on a
 *    fourth run.**
 *
 * WAVE 71: the wave-63 deleted-self-assignment lever was tested without
 * replacing this authoritative source.  A local `count = count` after the loop
 * is byte-identical and does not change allocation.  A deleted memory
 * self-assignment indexed by count rotates a2 from r8 to the ROM's r9 but only
 * moves count from r9 to r8; it does not create the ROM's [sp,#4] count spill.
 * Referencing x in a second deleted memory self-assignment moves a1 to ip and
 * src to r5, but count remains in r9.  Fixed-register dw/parameter probes spill
 * the parameter rather than count.  This confirms the needed lever is more
 * specific than merely extending count or reserving r8. */
inline u16 *inline_fn(u16 *arg0)
{
  return arg0;
}

void sub_0801C090(s32 a1, s32 a2, void *a3, s32 a4)
{
  int new_var2;
  u16 *src;
  u16 *dst;
  u16 count;
  u16 x;
  int new_var;
  s16 dx;
  s16 dw;
  u16 attr0;
  u16 attr1;
  u32 t;
  src = a3;
  count = *(src++);
  dst = gUnknown_03002F2C;
  do
  {
    while (count != 0)
    {
      if (a1 & 0x1000)
      {
        dw = -(*((const u16 *) (((const u8 *) gUnknown_0848B56C) + (((char) ((inline_fn(src)[1] >> 14) * 4)) + ((src[0] >> 14) * 16)))));
        x = src[1] & 0x1ff;
        t = 0x100;
        if (src[1] & t)
        {
          t = x | 0xffffff00;
          x = t;
        }
        dx = -x;
        t = ((a2 | src[0]) & 0xffffff00) | ((src[0] + a2) & 0xff);
        attr0 = t;
        t = ((a1 | src[1]) & 0xfffffe00) | (((a1 + dx) + dw) & 0x1ff);
        attr1 = t;
      }
      else
      {
        t = ((a2 | src[0]) & 0xffffff00) | (((src[0] + a2) + gUnknown_03002B20) & 0xff);
        attr0 = t;
        t = ((new_var = a1 | src[1]) & 0xfffffe00) | ((gUnknown_030030D0 + (src[1] + a1)) & 0x1ff);
        attr1 = t;
      }
      *(dst++) = attr0;
      *(dst++) = (attr1 & 0xcfff) | ((new_var2 = a1 ^ src[1]) & 0x3000);
      *(dst++) = src[2] + a4;
      dst++;
      gUnknown_03002F2C = ((u8 *) gUnknown_03002F2C) + 8;
      src += 3;
      count--;
    }

  }
  while (0);
}





