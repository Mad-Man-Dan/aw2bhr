#include "global.h"

/* sub_080611D8 @ 0x080611D8, 304 bytes. PARKED at 75.0%, SIZE EXACT (304/304).
 *
 * The SEMANTICS are settled and every instruction in the function body is
 * reproduced -- the prologue, all five sub_08061308 call sites with their
 * operands and branch targets, the 0x5c-stride gUnknown_085D5ABC subscript,
 * the whole list walk including the ROM's `adds r3,r1,#0` / `adds r1,r3,#0`
 * two-register walker seed, the out-of-line hit block and its `b` back to the
 * sentinel test, and the mark-consumed block. What is NOT reproduced is the
 * placement of the two shared return tails, and that is the entire residual.
 *
 * ROM tail layout:
 *     _080612D8: mark;  movs r0,#1;  b _080612FC
 *     _080612EA: alt;   beq _080612FA;  movs r0,#1;  b _080612FC
 *     _080612FA: movs r0,#0
 *     _080612FC: epilogue
 * i.e. the two `return 1`s are NOT cross-jumped into each other (each keeps
 * its own `movs r0,#1; b`), while the two `return 0`s ARE merged, into the
 * copy that sits LAST, immediately before the epilogue.
 *
 * Every spelling tried does the opposite -- it merges the `return 1`s (whose
 * common tail then includes the epilogue itself, so cross_jump takes it) and
 * keeps a `return 0` early, where it is a fall-through and therefore cannot be
 * eliminated. The two facts are ONE fact: `movs r0,#0` being last is what
 * pushes the epilogue out of both `return 1` tails and leaves them only two
 * insns long, which is below cross_jump's benefit threshold.
 *
 * Attempts, all size-exact at 304 bytes:
 *   1. `if (chain) { alt-body }` nested inside the unk07 test -- alt body laid
 *      out INLINE after the last call, list walk after it. Worst layout.
 *   2. + `break` in the loop and the literal 9999 in the sentinel compare
 *      instead of the live `none` local (the ROM re-materialises the pool
 *      constant there; keeping `none` live costs a callee-saved register and
 *      an extra push/pop of sl). Fixed the loop and the walker.
 *   3. + `goto alt` with the alt block last -- THIS IS THE BEST DRAFT, 75.0%,
 *      and it is the one below. Block order, pool position and the walker all
 *      match from here on; only the two tails differ.
 *   4. Sentinel inverted to `if (out[0] != 9999) { mark; return 1; } return 0;`
 *      -- moves the merge but does not fix it: the alt block then inverts its
 *      branch and the mark block's `movs r0,#1` merges instead. No better.
 *   5. Wave 63 tried the proposed single-exit `rv` rewrite. The alt result was
 *      canonicalised to branchless `neg; orr; lsr`, so it moved farther from
 *      the ROM rather than changing only the shared-tail placement.
 *
 * NOT the problem, checked: the `.rodata` relocation where the ROM has
 * `gUnknown_0816DAE8`. That ROM word holds 0x085766E4 (verified against
 * baserom.gba), so it is agbcc's own -fforce-addr pool copy of
 * &gUnknown_085766E4 and NOT an object; the honest spelling used below is
 * correct and per the brief the pool word is placed at promotion time.
 * 0x0816DADC likewise holds 0x085D5ABC.
 *
 * NEXT LEVER TO TRY: force the two `return 1` sites to differ so cross_jump
 * declines them, or find the spelling that makes the sentinel's `return 0` a
 * jump rather than a fall-through. A single-exit rewrite (one `return rv;` at
 * the bottom with an `rv` local) is the obvious next candidate and was not
 * tried -- it costs a pseudo, but the ROM has a spare low register here.
 *
 * WAVE 70 (W70-C): explicit `ret0`, `mark`, and distinct return-one labels do
 * produce the ROM's desired late `movs r0,#0` and separate `movs r0,#1; b`
 * tails. However, gcc then moves the four-word literal pool from the ROM's
 * sentinel barrier to after the mark block, shifting it by 16 bytes and
 * changing the sentinel to one direct `beq`. Structured, switch, and do-mark
 * spellings canonicalize the same way. The original 304/304, 75.0% draft is
 * retained because its walker and pool barrier remain exact. */

u8 sub_080611D8(void *arg)
{
  u16 *out;
  u16 none;
  int i;
  int best;
  char t;
  u8 r;
  out = arg;
  i = 0;
  best = 0;
  none = 9999;
  out[0] = none;
  if (gUnknown_030046C0.unk07 != 5)
  {
    t = gUnknown_085D5ABC[gUnknown_030046C0.unk06].deployLocation;
    if ((((((gUnknown_030046C0.unk06 == 20) && (sub_08061308(t, 3, out) == 1)) || ((gUnknown_030046C0.unk06 == 23) && (sub_08061308(t, 4, out) == 1))) || (sub_08061308(t, 0, out) == 1)) || ((gUnknown_030046C0.unk06 <= 2) && (sub_08061308(0x10, 1, out) == 1))) || ((gUnknown_0857680F[gUnknown_030046C0.unk06] != 2) && (sub_08061308(t, 2, out) == 1)))
    {
      goto alt;
    }
  }
  while (gUnknown_085766E4[i].unk00 != 0xff)
  {
    if ((gUnknown_085766E4[i].unk03 <= 0xfd) && (gUnknown_0857680F[gUnknown_030046C0.unk06] == gUnknown_085766E4[i].unk02))
    {
      out[0] = gUnknown_085766E4[i].unk00;
      out[1] = gUnknown_085766E4[i].unk01;
      best = i;
      break;
    }
    i++;
  }

  r = 1;
  if (out[0] == 9999)
  {
    return 0;
  }
  gUnknown_085766E4[best].unk03 = 0xfe;
  return r;
  alt:
  r = sub_08061668(out);

  if (r != 0)
  {
    return r;
  }
  return 0;
}






