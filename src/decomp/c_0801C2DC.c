#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C2DC.
 * sub_0801C2DC @ 0x0801C2DC
 */

/* MATCHED in wave 60 (W60-A) by THREE CHAINED PERMUTER RUNS plus one hand fix.
 * Parked since wave 36 at 81.6% size-exact; it had never been through the
 * permuter. Scores, each run started from the PREVIOUS run's best.c:
 *
 *   draft 81.6%  ->  run 1  97.1%  ->  run 2  99.6%  ->  run 3  100.0%
 *
 * This replicates wave 59 exactly (87.3 -> 94.3 -> 98.2 -> match). A single
 * long run from the original draft is NOT the same thing: run 1 spent 300 s and
 * a permuter win IS a different starting point.
 *
 * THE ONE HAND FIX, AND IT IS THE ONLY REASON RUN 3 DID NOT SELF-REPORT A
 * MATCH. Run 3's output scored `bytes: 0 of 272 differ (100.0% identical)` and
 * still failed, at `+4` bytes with `first difference at +0x110` -- i.e. past the
 * end of the function. The permuter writes its identity helper as a bare
 * `inline`, and **gcc 2.9 emits an out-of-line copy of a non-static `inline`
 * function into the same .text**, which `objcopy --only-section=.text` then
 * hands to trymatch as four trailing bytes. `static inline` inlines it AND
 * suppresses the copy. Measured, so do not "simplify" it either way:
 *
 *   inline        -> 0 of 272 differ, +4 bytes  (out-of-line copy emitted)
 *   static        -> NOT inlined at -O2; a real `bl inline_fn` and every
 *                    later byte shifts by 4. gcc 2.9 inlines only what is
 *                    declared `inline`; -O2 does not imply -finline-functions.
 *   static inline -> MATCH
 *
 * A 100%-identical-but-+4 result is worth recognising on sight: it is a section
 * artefact, not a codegen residual. Compare with AP_PutAnimFrameAtTime, which is +2 for
 * the alignment-pad reason and cannot be fixed at all.
 *
 * DO NOT TIDY ANY OF THE FOLLOWING -- each is a permuter edit that is load
 * bearing, and folding them back is what regresses the function:
 *   - `unsigned int v` rather than `u16 v`.
 *   - `inline_fn(a->unk10)`, an identity function, which stops CSE reusing the
 *     value already in a register and forces the ROM's reload.
 *   - re-reading `a->unk0c->unk02` where `p->unk02` would do (three sites).
 *     This is what produces the ROM's `adds r2, r4, #0` live-range split.
 *   - `new_var = (u16 **) a->unk04;` bound in the else-arm only. This is
 *     RESIDUAL 3 of the wave-36 park, which correctly predicted "the next thing
 *     to try is binding the base to its own local in that arm only".
 *   - `if (1) { return 0; }` in the second guard, and the `do { return 0; }
 *     while (0)` in `case 0`.
 *
 * The wave-36 park's type model was right and is unchanged: `struct
 * Unk0801C210` and `struct Unk0801C210Cmd` live in include/unknown-globals.h,
 * and `s16 unk18` is settled by this body reading +0x18 both ways in one block.
 * Wave 36's RESIDUAL 1 (agbcc CSEing `a->unk1a` across the guard where the ROM
 * reloads) turned out to be a consequence of the allocation, not a separate
 * fact -- nothing here addresses it directly and it is gone. */
static inline u16 *inline_fn(u16 *arg0)
{
  return arg0;
}

u8 AP_ExecFrame(struct Unk0801C210 *a)
{
  u16 **new_var;
  struct Unk0801C210Cmd *p;
  unsigned int v;
  u16 w;
  if (a == ((void *) 0))
  {
    return 0;
  }
  if (a->unk00 == ((void *) 0))
  {
    if (1)
    {
      return 0;
    }
  }
  if (a->unk1a == 0)
  {
    return 1;
  }
  if (a->unk18 != 0)
  {
    if ((--a->unk18) > 0)
    {
      return 1;
    }
    a->unk18 = 0;
    a->unk0c++;
  }
  p = a->unk0c;
  v = p->unk00;
  if (v != 0)
  {
    if (v != 0xFFFF)
    {
      a->unk1c = (a->unk1a * v) + a->unk1c;
      if (a->unk1c > 0xFF)
      {
        goto expand;
      }
      a->unk18 = 1;
      return AP_ExecFrame(a);
    }
  }
  else
  {
    switch (a->unk0c->unk02)
    {
      case 0xFF:
        a->unk0c = a->unk08;
        return AP_ExecFrame(a);

      case 1:
        AP_Delete(a);
        return 0;

      case 0:
        do
      {
        return 0;
      }
      while (0);

    }

  }
  a->unk28 = a->unk0c->unk02;
  a->unk0c = a->unk0c + 1;
  return AP_ExecFrame(a);
  expand:
  a->unk18 = a->unk1c >> 8;

  a->unk1c &= 0xFF;
  if (a->unk20 & 2)
  {
    a->unk10 = (u16 *) (a->unk04 + ((((u16 *) a->unk04)[a->unk0c->unk02] / 2) * 2));
  }
  else
  {
    new_var = (u16 **) a->unk04;
    a->unk10 = new_var[a->unk0c->unk02];
  }
  w = *a->unk10;
  if (w & 0x8000)
  {
    a->unk14 = a->unk10;
    a->unk10 = (u16 *) (((u8 *) inline_fn(a->unk10)) + (((w & 0x7FFF) * 6) + 2));
  }
  else
  {
    a->unk14 = (void *) 0;
  }
  a->unk20 |= 1;
  return 1;
}
asm(".global sub_0801C2DC\n.thumb_set sub_0801C2DC, AP_ExecFrame\n");
