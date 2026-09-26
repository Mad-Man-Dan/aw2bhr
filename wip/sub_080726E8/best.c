#include "global.h"

/* WAVE 72 (W72-F): configured baseline reverified at 228/216 (+12). Keeping
 * the original src parameter live for the unflipped arm is byte-neutral: the
 * flipped arm still strength-reduces both pointer GIVs and keeps the 16-byte
 * frame. Distinct q/out locals used only by the later arm increase the frame
 * to 20 bytes without suppressing either GIV. These pressure probes therefore
 * rule out cross-arm pointer liveness; the four-loop body and existing IV
 * diagnosis remain unchanged. */

/* Blits a w-by-h block of tilemap entries into a 32-wide BG tilemap at
 * (x, y), adding `base` to each entry and clipping to the 32x32 screen block.
 * `flip` mirrors the block horizontally, which also toggles each entry's
 * HFLIP bit (0x400).
 *
 * PARKED. Size 228 vs 216 (+12), first difference at +0xa. decomp-permuter:
 * 300 s, 4 threads, no byte match.
 *
 * REMAINING DIFF, and it is one fact: agbcc strength-reduces both pointers in
 * the FLIPPED arm here -- `subs r2, #2` walking the source backwards and
 * `adds r3, #2` walking the destination forwards, plus two extra frame slots
 * for their hoisted initial values, which is where the +12 bytes and the
 * spill of `x` out of r7 come from -- where the ROM recomputes BOTH addresses
 * from scratch on every iteration and reduces nothing. The ROM's flipped
 * inner loop hoists only `lsls r3, r5, #6` (iy * 64) and the 0x400 constant,
 * and its only induction variable is `ix` itself, whose increment is computed
 * at the top of the body (`adds r4, r2, #1`) and reused by `w - (ix + 1)`.
 *
 * Everything else is byte-exact and should be kept: the UNFLIPPED arm (which
 * the ROM does reduce, and which this reproduces instruction for
 * instruction), the prologue, the frame holding map at [sp] and p at [sp,#4],
 * both clip tests, and the register map (ip = w, sl = y, base in sb).
 *
 * AXES RULED OUT, all measured with compile_probe:
 *  - source index spelling. `src[iy*0x20 + (w-ix-1)]`, `*(src + (w-ix-1) +
 *    iy*0x20)`, `src[iy*0x20 + w - ix - 1]` and `*(p + (w - (ix+1)))` all
 *    strength-reduce identically. The ROM's own two arms are the proof that
 *    spelling is not the variable: they index the same map with the same clip
 *    tests and differ only in whether the compiler reduced.
 *  - destination spelling. `map[(y+iy)*0x20 + (x+ix)]` folds the index and
 *    scales it once (`lsl #5; add; lsl #1`) and is wrong for both arms;
 *    `*(map + (x+ix) + (y+iy)*0x20)` gives the ROM's two separate scalings
 *    (`lsls #6` for the row, `lsls #1` for the column) and is what is used
 *    here. This one WAS worth bytes and is kept.
 *  - `p` as a separate local seeded from `src` versus reassigning the `src`
 *    parameter. The local is right: it puts p's `ldr`/`str` after the
 *    promoted-mode conversion group, exactly where the ROM has it, and the
 *    same change is what closed the siblings sub_080727C0 and sub_0807286C.
 *  - `u8 flip` versus `int flip`. The lone `lsls r0, r0, #0x18` in the
 *    conversion group is the u8 parameter's PROMOTE_MODE extension with the
 *    `lsr` dropped by combine, so u8 is right.
 *  - two separate loop nests versus one nest with the flip test inside. The
 *    ROM has two nests (the `beq` is above both), which is what is here.
 *  - `w - (ix + 1)` spelled explicitly in place of `w - ix - 1` (wave 57,
 *    W57-D). Still strength-reduces both pointers in the flipped arm, and it
 *    additionally spills `x` out of r7 into [sp,#0xc], so it is strictly
 *    worse. Note what this means for reading the ROM: its `adds r4, r2, #1` at
 *    the top of the flipped inner body, shared between the source index and
 *    the loop increment, is a CONSEQUENCE of the giv not being reduced, not a
 *    cause of it. Do not try to reproduce that sharing from the source -- it
 *    appears on its own once reduction is suppressed.
 *
 * loop.c discards a giv when `lifetime * threshold * benefit < insn_count`.
 * Nothing reachable from C addresses that comparison, and no spelling probed
 * moved it. Next wave: do not spend attempts on index spellings. The open
 * lever is register PRESSURE, not spelling -- the ROM's flipped arm reads like
 * a loop that strength_reduce declined to reduce because too much was already
 * live, so a spelling that keeps one more value live across the inner loop is
 * the thing that has not been tried. */
void sub_080726E8(u16 *map, int x, int y, u16 base, int w, int h, const u16 *src, u8 flip)
{
  const u16 *p = src;
  volatile int new_var;
  int ix;
  int iy;
  if (flip)
  {
    for (iy = 0; iy < h; iy++)
    {
      for (ix = 0; ix < w; ix++)
      {
        if ((((unsigned) (x + ix)) < 0x20) && (((unsigned) (y + iy)) < 0x20))
        {
          new_var = ix;
          *((map + (x + new_var)) + ((y + iy) * 0x20)) = (p[(((iy * 0x20) + w) - ix) - 1] + base) ^ 0x400;
        }
      }

    }

  }
  else
  {
    for (iy = 0; iy < h; iy++)
    {
      for (ix = 0; ix < w; ix++)
      {
        if ((((unsigned) (x + ix)) < 0x20) && (((unsigned) (y + iy)) < 0x20))
        {
          *((map + (x + ix)) + ((y + iy) * 0x20)) = (*((p + ix) + (iy * 0x20))) + base;
        }
      }

    }

  }
}
