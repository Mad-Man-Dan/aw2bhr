#include "global.h"

/* Wave 54, W54-H; updated wave 63, W63-A. NOT A MATCH: 468 bytes against 472,
 * i.e. exactly 2 instructions short, and 31.78% only because the shortfall shifts every later
 * byte (the "first difference at +0x1c" is the `b` at the end of the
 * count == 0 guard, whose displacement moves with the size).
 *
 * WAVE 63 UPDATE: wave 59's opaque-store LICM barrier applies here. The
 * zero-code `gUnknown_08551E64[0][0] += 0` in both scan loops stops the extra
 * address hoist and fixes loop 1's entire four-byte shortfall, including its
 * pool order and high-register allocation. The remaining -4 is loop 2 only:
 * the ROM keeps the two arm tails distinct through the shared test/store,
 * while this candidate still cross-jumps that tail. Moving the barrier within
 * the loop and spelling it through x/out/side are byte-identical; wrapping the
 * conditional in `do { } while (0)` and block-scoping x both regress the
 * allocation. The statements below remain settled.
 *
 * EVERY STATEMENT IS SETTLED. The residual is TWO LICM DECISIONS, both in
 * loop.c and both self-consistent fixpoints, not a wrong decomposition:
 *
 *   1. Loop 1. The ROM hoists exactly FIVE invariants -- side*5 (r7),
 *      side*40 (sb), &gUnknown_020296BC (sl), side*0xb4 (r8) and
 *      &gUnknown_08552148[side] (ip) -- and leaves BOTH pool words used in the
 *      body (gUnknown_08552150 and gUnknown_02029A10) to be rematerialised
 *      each iteration. That fills every register, so &gUnknown_08552148[side]
 *      lands in ip and the body pays a `mov r2, ip` to read it, which is what
 *      keeps r2 as a third scratch. The wave-54 baseline candidate hoisted SIX (it also took
 *      gUnknown_02029A10 into ip), needs only two scratch registers, and so
 *      parks side*5 in r2: 2 instructions short. The ROM's pool slot order
 *      085D6A48, 03004580, 020296BC, 08552148, 08552150, 02029A10 is the
 *      readout -- the two unhoisted words sort last.
 *      Inlining the `x` read into the `if` (no local) flips WHICH of the two
 *      gets hoisted -- 08552150 instead of 02029A10 -- but still hoists one,
 *      and it breaks loop 2's cross-jump. Not the lever.
 *   2. Loop 2. The ROM spills side*5 to sp[0] and recomputes side*40 in the
 *      body from it (`lsls r1, r1, #3`), which is what makes the two arms'
 *      tails differ and stops cross-jumping at the store; the wave-54 baseline
 *      spills side*40 instead, so both arms' tails are identical and
 *      cross-jumping swallows the shared `cmp r0, #0; beq` as well:
 *      2 more instructions short.
 *
 * Both fixpoints are stable under every source-level lever tried. What is NOT
 * in question, and should not be re-derived: the `row[2]` pointer-local
 * spelling for gUnknown_085D6A48 column 2 (the ROM's `ldrh [r0, #4]`
 * displacement -- the array spelling hoists the +4 onto the base),
 * gUnknown_020296CE as the destination symbol of the last loop (agbcc 2.9 does
 * not distribute `* 2` over `i + 9`), and the flat `(side * 5 + i) * 4` index
 * into gUnknown_08552150. Loop 3 is instruction-for-instruction correct and
 * differs only by a permutation of r8/r9/sl/ip inherited from loops 1-2.
 * Wave 70, W70-B: configured re-verdict remains 468/472. The existing
 * barriers were left in place; the remaining residual is still the
 * two-instruction lifetime/allocation difference described above. */
void sub_08055768(u16 side, u16 count)
{
  u16 i;
  u16 out;
  u16 x;
  u16 *p;
  u16 *row;
  {
    out = 0;
    i = 0;
    if (count == 0)
    {
      return;
    }
    row = gUnknown_085D6A48[gUnknown_03004580[side][1]];
    if (row[2] == 1)
    {
      while (out != count)
      {
        x = gUnknown_08552150[(side * 5) + i][0];
        if (gUnknown_02029A10[side].entries[x].unk00 != 0)
        {
          gUnknown_020296BC[side][out] = *((u16 *) (((u8 *) gUnknown_08552148) - (-(side * 2))));
          out++;
        }
        i++;
        if (i == 5)
        {
          out = count;
        }
      }

    }
    else
    {
      while (out != count)
      {
        x = gUnknown_08552150[(side * 5) + i][0];
        if (side == gUnknown_0300450C)
        {
          if (gUnknown_02029A10[side].entries[x].unk00 != 0)
          {
            gUnknown_020296BC[side][out] = x;
            out++;
          }
        }
        else
          if ((gUnknown_02029A10[side].entries[x].unk00 != 0) && (gUnknown_02029A10[side].entries[x].unk01 != 0))
        {
          gUnknown_020296BC[side][out] = x;
          out++;
        }
        i++;
        gUnknown_08551E64[0][0] += 0;
        if (i == 5)
        {
          out = count;
        }
      }

    }
    p = gUnknown_08551E64[gUnknown_030045A0[gUnknown_0300450C]];
    for (i = 0; i < count; i++)
    {
      gUnknown_020296CE[side][i] = p[(gUnknown_08551E74[(side * 2) + gUnknown_0300450C] * 5) + i] + ((0, gUnknown_08551D26))[gUnknown_030045A0[side]][0];
    }

  }
}
