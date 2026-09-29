#include "global.h"

/* sub_0805634C @ 0x0805634C
 *
 * PARKED at 38.2%, -4 bytes (360 vs 364), wave 49, W49-M. Read the diff, not
 * the score: the score is positional and the FIRST loop plus everything in
 * front of it is byte-exact apart from one register pair.
 *
 * SETTLED, do not re-derive:
 *   - `c = sub_0805653C(c, b)` -- the callee takes this function's THIRD and
 *     SECOND arguments in that order, and the result goes back into `c`. That
 *     reuse is what makes the ROM's `mov r8, r2` / `mov r0, r8` / `mov r8, r0`
 *     one pseudo, and it is why sub_0805653C returns u16: any wider return
 *     costs an `lsls`/`lsrs` pair either at the assignment or at the
 *     sub_080564B8 call, and the ROM has neither.
 *   - the first loop's `+` operands are in the ROM's order, `p[...]` first and
 *     `gUnknown_08551D2A[...][0]` second. The other order compiles to the same
 *     value and emits a `-fforce-addr` `.rodata` word for gUnknown_0300450C
 *     plus a wrong pool order; see the new chapter in docs/agbcc-codegen.md.
 *   - `p` is a source-written local, not a LICM hoist: its `ldr`/`mov sl` sits
 *     BEFORE the `movs r4, #0` that is the first `for`'s init, and a hoist by
 *     construction lands after the init.
 *
 * THE WHOLE RESIDUAL IS THE SECOND LOOP'S TWO COLUMN OFFSETS, and they are one
 * fact twice. The ROM keeps the BARE symbol in a register and adds the constant
 * at run time:
 *     ldr r2, =gUnknown_08551D22 ; adds r2, #6  ; adds r2, r1, r2
 *     mov r0, sl (=gUnknown_020298E0) ; adds r0, #0x1a ; adds r0, r3, r0
 * i.e. the address tree is `(base + K) + offset`, with the constant applied to
 * the base BEFORE the varying part. Every spelling below puts the constant
 * LAST, which `plus_constant` then folds into the pool word (`g+0x1a`, `g+6`)
 * because the base is still a SYMBOL_REF at that point:
 *     gUnknown_020298E0[b].unk1a[j]                      pool word g+0x1a
 *     rec = gUnknown_020298E0; rec[b].unk1a[j]           bare word, but the
 *                                                       whole `rec+144b+0x1a`
 *                                                       hoists and the
 *                                                       `144b + j*2` CSE with
 *                                                       gUnknown_020298EC dies
 *     (rec = gUnknown_020298E0, rec)[b].unk1a[j]         same
 *     rec bound as the first statement of the outer or inner loop body   same
 *     rec->unk1a[b * 72 + j]                             pool word g+0x1a
 *     gUnknown_08551D22[x][3]                            pool word g+6
 *     (t = gUnknown_08551D22, t)[x][3]                   bare word but `ldrh
 *                                                       [rB,#6]` displacement
 *     t = gUnknown_08551D22; *(t[0] + 3 + x * 5)         `ldrh [rB,#6]`
 *     c3 = gUnknown_08551D22[0] + 3; c3[x * 5]           pool word g+6
 * expand_expr applies get_inner_reference's variable OFFSET first and its
 * constant BITPOS second, so what is needed is a base expression that already
 * carries the constant while the offset is added outside it -- and a binding
 * local does not give that, because a local assigned a constant address is
 * const-propagated straight back into the symbol.
 * The knock-ons all follow from those two: the ROM spills `b * 0x6c` AND
 * `b * 0x90` (`sub sp, #8`, this candidate #4), hoists gUnknown_020298E0's bare
 * word into the outer preheader, and hoists `&gUnknown_030045A0[b ^ 1]` out of
 * the inner `if`. Nothing here is a type error; the globals are settled.
 *
 * Wave 63, W63-A re-tested the wave-60 wrapper lever without re-deriving the
 * body. A leading-filler wrapper still folds the two constants into relocations
 * (`gUnknown_020298E0+0x1a`, `gUnknown_08551D22+6`). A wrapper containing an
 * array of row structs makes D22 bare but leaves `ldrh [rN,#6]`, while E0 still
 * relocates at +0x1a. Neither produces the ROM's runtime base adds, and both
 * preserve the same 360/364 shortfall or regress allocation. The exact
 * residual therefore remains these two column-offset placements.
 * Wave 70, W70-B: configured re-verdict remains 360/364. Two separate
 * statements `p = base; p += K;` prove that bare-pool-word plus run-time add
 * is reachable for both +0x1a and +6. Before the loop they spill adjusted
 * pointers and destroy the inner CSE; in the body they const-propagate back to
 * `base+K`. The operation is reachable, but not at the required lifetime.
 */

void sub_0805634C(u16 a, u16 b, u16 c)
{
  u16 *p;
  u16 i;
  u16 j;
  volatile unsigned int new_var2;
  const u16 (*new_var)[5];
  unsigned int side;
  new_var = gUnknown_08551D2A;
  if (c == 0)
  {
    return;
  }
  c = sub_0805653C(c, b);
  sub_080564B8(a, b, c);
  p = gUnknown_08551E64[gUnknown_030045A0[gUnknown_0300450C]];
  for (i = 0; i < c; i++)
  {
    gUnknown_02029822[b][i] = p[(gUnknown_08551E7C[(b * 2) + gUnknown_0300450C] * 5) + i] + gUnknown_08551D2A[gUnknown_030045A0[b ^ 1]][0];
  }

  for (i = 0; i < c; i++)
  {
    ;
    for (j = 0; j < 5; j++)
    {
      if (gUnknown_0202980A[b][i] == gUnknown_020298E0[b].unk1a[j])
      {
        gUnknown_02029822[b][i] = (gUnknown_020298EC[b][j] - gUnknown_08551D22[gUnknown_030045A0[b ^ 1]][new_var2 = 3]) + new_var[gUnknown_030045A0[b ^ 1]][0];
      }
    }

  }

  sub_08056638(b);
}



