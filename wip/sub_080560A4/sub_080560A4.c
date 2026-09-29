#include "global.h"

/* sub_080560A4 @ 0x080560A4
 *
 * PARKED at 24.9%, +4 bytes (684 vs 680), wave 49, W49-M. The score is
 * positional and misleading: every block below is in the ROM's order with the
 * ROM's instructions, and the residual is ADDRESS-EXPANSION ORDER plus the
 * register allocation that follows from it. Nothing here is a type error and no
 * global needs re-deriving -- sub_0805653C, matched byte-for-byte this wave,
 * shares gUnknown_085D6A48's row-struct cast, gUnknown_0855218C's [5][2]
 * declaration and gUnknown_02029A10's entry access with this function.
 *
 * Parameters 4 and 5 are dead: the prologue narrows only r0 and r1 and never
 * reads r2, r3 or the stack word. The arity is the caller's (sub_0805601C,
 * already matched, passes five).
 *
 * The tail from `n > a` onwards is sub_080564B8's body written out inline --
 * note BOTH fill loops are bounded by `a`, not by `n` -- followed by the same
 * gUnknown_08551E64 loop sub_0805634C ends with. That last loop is worth
 * copying rather than re-deriving: its `+` operands must be `p[...]` first and
 * `gUnknown_08551D2A[...][0]` second, or a `-fforce-addr` `.rodata` word
 * appears for gUnknown_0300450C (docs/agbcc-codegen.md, "A -fforce-addr
 * .rodata word is a REGISTER-PRESSURE symptom").
 *
 * THE THREE MEASURED RESIDUALS, in the order they appear:
 *  1. In the first arm the ROM expands the SOURCE table's pool word before the
 *     destination's -- relocs 085D6A48, 03004580, 085521B4, 0202980A, where
 *     this candidate has 0202980A before 085521B4. That is the wave-30 (W30-E)
 *     "the destination's address is created FIRST, so a source array wants its
 *     own pointer local" case. Wave 66 measured both row- and element-pointer
 *     locals bound as the first statement INSIDE that loop body: they do put
 *     085521B4 before 0202980A and remove the +4-byte section delta, but rotate
 *     the remaining allocation to 15.6% identity. The configured 24.9% draft
 *     is retained; this lever pays for residual (1) while worsening (2)/(3).
 *  2. In the second arm's first loop the ROM scales the two index terms
 *     SEPARATELY (`lsls r0,r3,#2` for i*4, `lsls r4,r5,#2` for b*20) while this
 *     candidate folds them and scales the sum, `(b*5 + i)*4`. Same fold
 *     sub_080564B8 is parked on. In sub_0805653C, where `b * 20` is hoisted out
 *     of the loop, the identical `gUnknown_0855218C[b][i][0]` spelling is
 *     byte-exact -- so the declaration is right and the fold is a consequence of
 *     `b * 5` also being live here for gUnknown_02029C14[b][t].
 *  3. The ROM spills two values (`sub sp, #8`); this candidate spills one. It
 *     also parks `a` in ip and `b` in r7, where this candidate has both in low
 *     registers -- i.e. the ROM is carrying one more live value than this C
 *     does, which is the same diagnosis as (2).
 *
 * Wave 72 reverified this configured draft at 684/680 (+4), 24.9%. A narrower
 * scalar binding for residual (1), `t = source; dest = t;`, removes the size
 * surplus completely (680/680) but rotates the remaining allocation to 19.1%
 * identity; a block-local `u16 v` is the same mechanism at 18.5%. Binding an
 * else-arm row pointer for gUnknown_0855218C is worse (704/680, 9.9%), and
 * combining it with the scalar binding is 700/680, 15.1%. Thus the scalar
 * split fixes the source-before-destination defect with a narrower lifetime
 * than Wave 66's pointer bindings.
 *
 * Wave 78 copied sub_080564B8's exact flat fill-loop address spelling and its
 * separate `(u16)(i - 1)` binding into the shared tail. Configured measurement
 * is still 684/680 (+4), but identity improves slightly to 25.4%; this is the
 * retained fixpoint. The missing frame value and the three earlier address /
 * allocation residuals are unchanged.
 */

struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u8 filler_08[0x12 - 0x08];
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 filler_14[0x18 - 0x14];
};

void sub_080560A4(u16 a, u16 b, u16 c, u16 d, u16 e)
{
  struct Unk85D6A48Row *rows;
  u16 *p;
  const u16 *q;
  u16 n;
  u16 i;
  u16 j;
  u16 t;
  u16 new_var;
  n = 0;
  if (a == 0)
  {
    return;
  }
  rows = (struct Unk85D6A48Row *) gUnknown_085D6A48;
  if (rows[gUnknown_03004580[b][1]].unk04 == 1)
  {
    for (i = 0; i < 5; i++)
    {
      gUnknown_0202980A[b][n] = gUnknown_085521B4[b][i][0];
      j = a - 1;
      n++;
      if (n == a)
      {
        gUnknown_0202980A[b][j] = gUnknown_08552148[b];
        break;
      }
    }

  }
  else
  {
    for (i = 0; i < 5; i++)
    {
      t = gUnknown_0855218C[b][i][0];
      if (gUnknown_02029A10[b].entries[t].unk01 != gUnknown_02029A10[b].entries[t].unk00)
      {
        gUnknown_0202980A[b][n] = t;
        gUnknown_02029C14[b][t] = 2;
        n++;
      }
    }

    if (a > n)
    {
      for (j = 0; j < 5; j++)
      {
        t = gUnknown_0855218C[b][j][0];
        if (gUnknown_02029C14[b][t] != 0)
        {
          gUnknown_02029C14[b][t] = 0;
        }
        else
        {
          q = gUnknown_08554A00[(b * 5) + t];
          gUnknown_02029A10[b].entries[t].x = q[200];
          gUnknown_02029A10[b].entries[t].y = q[201];
          gUnknown_02029C14[b][t] = 1;
          gUnknown_0202980A[b][n] = t;
          n++;
          if (n == a)
          {
            break;
          }
        }
      }

    }
  }
  if (n > a)
  {
    for (i = 0; i < a; i++)
    {
      *((u16 *) (((b * 0x6c) + (i * 2)) + ((char *) gUnknown_02029816))) = 1;
    }

    new_var = (u16) (i - 1);
    gUnknown_02029808[b].unk0e[new_var] += n - a;
  }
  else
  {
    for (i = 0; i < a; i++)
    {
      *((u16 *) (((b * 0x6c) + (i * 2)) + ((char *) gUnknown_02029816))) = 1;
    }

  }
  p = gUnknown_08551E64[gUnknown_030045A0[gUnknown_0300450C]];
  for (i = 0; i < a; i++)
  {
    gUnknown_02029822[b][i] = p[(gUnknown_08551E7C[(b * 2) + gUnknown_0300450C] * 5) + i] + gUnknown_08551D2A[gUnknown_030045A0[b ^ 1]][0];
  }

  sub_08056638(b);
}










