#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062C94.
 * sub_08062C94 @ 0x08062C94
 */

/* MATCHED wave 90 (W90-C) from the 93.7% park, with two changes and no new
 * construct:
 *   - the inner m-loop of the third loop counts with `t` (dead there) instead
 *     of a fresh `m`. With `m`, global.c ranked the table address and the loc
 *     giv above the counter (priority 1.1 vs 0.66); `t`'s longer, busier range
 *     puts the counter first, in r5, as in the ROM.
 *   - the first loop counts with `u` and is ONE statement,
 *     `loc[u] = (TAB->v[u] = 0, 0);`. expand_assignment expands an ARRAY_REF
 *     LHS's offset BEFORE the RHS, so the loc address giv is recorded ahead of
 *     the table's. That gives the ROM's giv-init order (table, then loc) while
 *     the table store still comes first in the body. The comma's `0` avoids the
 *     re-read that the value of a volatile assignment costs. With `u` as the
 *     counter, the zero gets r3 and the reversed counter r4.
 * The permuter-era `(u = 0)` scratch and every older note below are
 * superseded. Needs "rodata": ["0x0816DB38", "0x0816DB3C"]. */
#include "global.h"
/* WAVE 77, W77-B. Size-exact 348/348, 22 bytes differ (93.7%).
 * CLASSIFIED as NOT ORDER. Sixteen of the 22 differing bytes are register
 * number fields of a systematic renumbering: r3/r4 exchanged at +0x11
 * (movs r3,#0 against movs r4,#0), +0x17 (movs r4,#24 against movs r3,#24),
 * +0x23 and +0x25 (subs/cmp Rd); and an r5-r7-r6 rotation at +0x101..+0x107
 * (movs Rd, adds Rd, mov Rd,sp, adds Rd,#6) carried through +0x132..+0x13B
 * (ldrh/strh base register, adds Rd,#2, adds Rd,#1, cmp Rd,#24).
 * The remaining 6 bytes at +0x18..+0x1D are one moved store: this draft emits
 * `strh rX,[r1,#0]` before `ldrh r0,[r2,#0]` where the ROM emits it after.
 * +0x98 is one further immediate/addend byte. So order is present but is a
 * minority of the residual and rides on the renumbering.
 *
 * WAVE 79 (W79-E) -- THE MOVED STORE WAS PROBED AND IT IS COUPLED TO THE
 * `(u = 0)` LEVER, WHICH IS WHY IT CANNOT BE FIXED ON ITS OWN.
 *
 * The ROM's first loop is, in order:
 *     _08062CAC:  ldrh r0,[r2]   <- the volatile table's dead load
 *                 strh r3,[r2]   <- v[i] = 0
 *                 strh r3,[r1]   <- loc[i] = 0   (stack twin)
 *                 adds r2,#2 / adds r1,#2 / subs r4,#1 / cmp r4,#0 / bge
 * so the VOLATILE store is the first statement, the stack store the second,
 * and ONE hoisted zero (r3) feeds both while the down-counter takes r4.
 * That reads as the plain two-statement source in that order. It is not.
 * Three spellings measured this wave, all against the 93.7% draft:
 *
 *   1. `loc[i] = TAB->v[i] = 0;` (the chained assignment, W30-A's rule for a
 *      dead load on a volatile lvalue -- the obvious reading of `ldrh` before
 *      `strh`). REGRESSES: the first loop grows, an alignment `.short 0x0000`
 *      and a fourth pool word appear, and the two `gUnknown_0816DB3x`
 *      force-address words become this unit's own `.rodata` pair. The dead
 *      load here is NOT the chained-assignment read-back; it is agbcc's
 *      ordinary read-before-write on a volatile store, which the wave-47 note
 *      already said and which needs no source spelling at all.
 *   2. Volatile store first, `loc[i] = 0;` second, i.e. the ROM's literal
 *      statement order with the permuter's `(u = 0)` scratch dropped:
 *      85.9%, 49 of 348 bytes differ (from 22). The order is right and the
 *      function is much worse.
 *   3. Volatile store first, `loc[i] = (u = 0);` second, i.e. the order fixed
 *      with the scratch KEPT: catastrophic -- 352 bytes (+4), 13.8%.
 *
 * So `(u = 0)` only works while the stack store is the FIRST statement: it is
 * what moves u's first reference ahead of the middle loops (W38-E's
 * creation-order lever), and moving it behind the volatile store destroys the
 * allocation for the whole rest of the function. The moved store and the
 * r3/r4 renumbering are ONE fact pulling in opposite directions, and this
 * draft is at the better end of it. NAMED CONSTRUCT: statement order --
 * measured, and it costs more than it buys. What is left of the residual is
 * the r3/r4 and r5-r7-r6 allocno rotation, which has nothing behind it.
 * Do not re-probe the first loop's statement order. */

/* WAVE 86 (W86-B). Held at 93.7%, 22 of 348, size-exact, first difference
 * +0x11 (configured profile). THE WAVE-83 LIVE-RANGE-SPLIT LEVER WAS TESTED
 * ON BOTH OF THIS FUNCTION'S TIES AND IS REFUTED ON BOTH.
 *
 * The question this function was picked to answer: with TWO independent ties
 * in one function (the r3/r4 exchange in the first loop and the r5-r7-r6
 * rotation in the last), can a split move ONE without disturbing the other?
 * MEASURED ANSWER: NO. Both splits made the whole function worse and BOTH
 * moved the first difference EARLIER than the baseline's +0x11, i.e. into the
 * first loop, which is upstream of where the split was placed.
 *   D1  narrow live copy of the THIRD loop's index (`short kk; kk = k;` inside
 *       the `v[k] != 0` arm, used at every k-site in the inner m-loop):
 *       size-exact 348, 129 of 348 differ, 62.9%, first difference +0xe.
 *       The split is in the LAST loop and it moved the first difference from
 *       +0x11 to +0xe -- it perturbed the FIRST loop.
 *   D2  narrow live copy of the FIRST loop's index (`short ii; ii = i;` at the
 *       top of the body, both stores on ii): size-exact 348, 315 of 348
 *       differ, 9.5%, first difference +0xc.
 * Baseline 22 differing bytes. Both stay size-exact, so this is pure
 * allocation churn: the copies re-run global allocation for the whole
 * function rather than re-ranking one pair.
 *
 * WHY, and it is the same discriminator as the rest of the batch (chapter
 * appended to docs/agbcc-codegen.md this wave): a live-range split re-ranks a
 * tie only when the ORIGINAL pseudo DIES at the copy. In the promoted proof
 * src/decomp/c_08040EF4.c, `new_var = y;` is y's LAST use, so y's range is
 * genuinely split in two. Here both copy sources are LOOP COUNTERS, kept live
 * to the bottom of the body by the for-increment and the exit test, so
 * `kk = k;` and `ii = i;` split nothing -- they ADD a live pseudo inside a
 * loop. This function is the worst possible host for that: the volatile table
 * means every `TAB->v[..]` store already carries the wave-47 dead load, the
 * stack twin `loc[]` is live throughout, and W34-C's rule (a volatile scalar
 * read into a local emits a copy) means any added copy near the table costs
 * an instruction as well as a register.
 *
 * NOTHING WAS CHANGED. The permuter output below is preserved exactly, still
 * 93.7%. Do not tidy it; see the wave-77/79 header above and NOTES.md.
 */

/* gUnknown_02029C20's first 0x32 bytes are a VOLATILE u16[0x19] counter table:
 * every store to them in the ROM carries the wave-47 dead load, and every store
 * to the stack twin beside them does not. */
struct Unk62C94Tab
{
    /* 0x00 */ volatile u16 v[0x19];
};
#define TAB ((struct Unk62C94Tab *)gUnknown_02029C20)

/* W60-F: 82.2% -> 91.4% -> 93.7% over two chained decomp-permuter runs (each
 * started from the previous run's best.c; runs 3 and 4 from 93.7% found
 * nothing). THE BODY BELOW IS PERMUTER OUTPUT AND MUST NOT BE TIDIED -- two of
 * its shapes are load-bearing levers, not noise:
 *   - `loc[i] = (u = 0);`  reuses `u` as the scratch carrying the zero, which
 *     moves u's first REFERENCE earlier (W38-E's allocno creation-order lever),
 *     and stores the stack twin BEFORE the volatile table.  Wave 79 measured
 *     both halves of this and they are inseparable -- see the header.
 *   - the else arm binds its loop base to `k` (`k = t * 0x40; for (u = k; ...)`)
 *     while the if arm leaves it inline -- the two arms are deliberately
 *     ASYMMETRIC.
 * Re-run try_match after any cosmetic edit. */
void AiCalcEnemyMassNetOfOwnCounters(void)
{
  u16 loc[0x19];
  int t;
  int u;
  int k;
  for (u = 0; u <= 0x18; u++)
  {
    loc[u] = (((struct Unk62C94Tab *) gUnknown_02029C20)->v[u] = 0, 0);
  }

  for (t = 0; t <= 3; t++)
  {
    if (((gPlayers[gUnknown_030033EC].unk2c >> t) & 1) == 0)
    {
      for (u = t * 0x40; u < ((t * 0x40) + 0x40); u++)
      {
        if (gUnknown_08499594[u].type != 0)
        {
          loc[gUnknown_08499594[u].type] += gUnknown_08499594[u].hp;
        }
      }

    }
    else
    {
      k = t * 0x40;
      for (u = k; u < (k + 0x40); u++)
      {
        if (gUnknown_08499594[u].type != 0)
        {
          ((struct Unk62C94Tab *) gUnknown_02029C20)->v[gUnknown_08499594[u].type] += gUnknown_08499594[u].hp;
        }
      }

    }
  }

  for (k = 1; k < (0x18 + 1); k++)
  {
    if (((struct Unk62C94Tab *) gUnknown_02029C20)->v[k] != 0)
    {
      for (t = 1; t <= 0x18; t++)
      {
        ((struct Unk62C94Tab *) gUnknown_02029C20)->v[k] -= GetCoAdjustedBaseDamage(gPlayers[gUnknown_030033EC].co, gPlayers[gUnknown_030033EC].coMode, t, k, 0) * loc[t];
      }

    }
  }

}
asm(".global sub_08062C94\n.thumb_set sub_08062C94, AiCalcEnemyMassNetOfOwnCounters\n");
