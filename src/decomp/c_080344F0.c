#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080344F0.
 * sub_080344F0 @ 0x080344F0
 */

/* MATCHED, wave 73 (W73-D). 68 bytes, byte-for-byte identical.
 *
 * This function had been parked for four waves as a `check_dbra_loop`
 * reversal: the ROM runs its four-iteration byte copy with `movs r4,#0` /
 * `adds r4,#1` / `cmp r4,#3` / `ble`, and twelve `for` / `do-while` spellings
 * measured across waves 25, 46, 54 and 73 ALL came out `movs r3,#3` /
 * `subs r3,#1` / `cmp r3,#0` / `bge`. The entry had concluded that an
 * ascending counter and two strength-reduced pointers were mutually
 * exclusive through the source.
 *
 * They are not. Three steps closed it:
 *
 * 1. THE GOTO LOOP (80.9% -> 83.8%). loop.c only processes a loop it has a
 *    NOTE_INSN_LOOP_BEG for, so a goto loop is never seen by
 *    check_dbra_loop at all and the counter stays ascending. A goto loop
 *    also costs every loop optimisation, so it is only affordable when the
 *    body needs no giv -- and this body is a bare byte copy already written
 *    with explicit `s++` / `d++` source bivs, so strength reduction had
 *    nothing to contribute. (sub_0806412C is the same reversal and the goto
 *    form does NOT help there, because its outer loop needs a 0x18-stride
 *    giv that only strength_reduce produces. That contrast is the test to
 *    apply before reaching for this lever.)
 *
 * 2. THE COUNTER'S INIT POSITION (83.8% -> 91.2%), a separate fact from the
 *    loop form: `i = 0;` moved ahead of the `s` / `d` setup, which is where
 *    the ROM emits `movs #0`. The instruction stream is 1:1 from here on and
 *    the only residual was one swapped pair -- ROM `p`=r3 / `i`=r4, candidate
 *    the other way round.
 *
 * 3. THE PERMUTER (91.2% -> match), 195 iterations. Its win is `new_var`
 *    below: a SECOND binding local for the post-loop `*p`, so that `p`'s live
 *    range no longer spans the loop and the two allocnos stop tying. This is
 *    the wave-17 "splitting one binding local into N" lever, found
 *    mechanically. DO NOT FOLD `new_var` BACK INTO `p` -- it is the match.
 *
 * The earlier permuter run this wave found nothing and the parked entry was
 * closed on that basis; it had been run against the OLD descending-counter
 * draft, where the residual was a loop-DIRECTION decision the permuter cannot
 * reach. The lesson is that a permuter negative is only evidence about the
 * draft it was run from.
 *
 * RULED OUT along the way: DECLARATION ORDER is not the register tie-break.
 * Moving `int i;` to the head of the declaration list compiles
 * BYTE-IDENTICALLY, so agbcc is not creating these pseudos at `expand_decl`
 * time in declaration order -- the creation order that breaks
 * `allocno_compare`'s tie is first REFERENCE order.
 *
 * The relocation: the ROM's .text pool word relocates against
 * gUnknown_08090D84 -- agbcc's -fforce-addr address constant for
 * gUnknown_030044B0, the same idiom as c_080345C8.c's gUnknown_08090D88.
 * Naming gUnknown_030044B0 directly is also size-exact but relocates against
 * this unit's own .rodata, a DIFFERENT relocation.
 */
void sub_080344F0(int a)
{
  u8 * const *p;
  u8 * const *new_var;
  u8 *blk;
  u8 *s;
  u8 *d;
  int i;
  p = &gUnknown_08090D84;
  blk = *p;
  blk[0] = 0xd;
  blk[6] = a;
  blk[7] = gUnknown_03004490[0];
  blk[0xc] = gUnknown_03004490[1];
  blk[0xd] = gUnknown_03004490[2];
  i = 0;
  s = gUnknown_03004490 + 4;
  d = blk + 0xe;
  new_var = p;
  _copy:
  *d = *s;

  s++;
  d++;
  i++;
  if (i <= 3)
  {
    goto _copy;
  }
  LinkQueueCommand(*new_var);
}
