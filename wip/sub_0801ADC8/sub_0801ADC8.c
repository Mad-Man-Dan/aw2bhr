#include "global.h"

/* PARKED at 34.5% / -4 bytes -- wave 56, W56-F. READ THE SIZE, NOT THE SCORE:
 * the candidate is exactly FOUR bytes short and those four bytes are ONE
 * two-instruction pair. Everything else is shifted by it.
 *
 * The whole function is derived: the four sequential 16-slot passes, the
 * duplicated inner retry loop (`for (k = 0; k < 4; k++)` with a `break` on
 * sub_0801B018 succeeding), the guards, and the statement order inside each
 * body. gUnknown_0808EF5C is agbcc''s -fforce-addr slot holding &gUnknown_0200CC2C
 * and is spelled honestly (the `.rodata` reloc against it is the CORRECT
 * output, per the brief); promotion will need "rodata": ["0x0808EF5C"].
 * gUnknown_0200CC58 in asm/ is gUnknown_0200CC38.unk20 and gUnknown_02002FFF is
 * gUnknown_02002000[0xfff] -- neither is a global and neither is declared.
 *
 * THE MISSING FOUR BYTES. In the `else` arm of the sub_0801B120(0xff) test the
 * ROM writes
 *
 *     movs r6,#0x10 / movs r4,#8 / movs r3,#0 / movs r5,#0xf   <- preheader
 *     ...
 *     adds r0,r1,#0 / orrs r0,r4 / orrs r0,r3 / movs r1,#0xef / ands r0,r1
 *
 * i.e. `unk20[i] = (unk20[i] | 8 | v) & 0xef` with a THIRD OR operand that is a
 * register holding ZERO, hoisted into the preheader alongside the literals.
 * sub_0801A7D8 has the same idiom with `& 0xfb`, so it is a real source
 * construct, used twice, and not noise.
 *
 * SIX SPELLINGS PROBED, ALL REFUTED -- do not repeat them. `u8 v = 0` just
 * before the loop; `v = 0` before an intervening `bl`; `v` assigned 0 in both
 * arms of an `if`; `v` assigned inside a preceding loop; `v` never assigned at
 * all; `v = 0 & <global>`; and `| (v & 0x10)` / `| (v * 0x10)` with v zero. Five
 * of the eight fold the operand away entirely; the uninitialised one instead
 * hoists `v | 8` as a single register, which the ROM does NOT do -- and that
 * last observation is the useful constraint: in the ROM the two ORs are BOTH
 * inside the loop, so at reassociation time agbcc did not consider the operand
 * loop-invariant, even though the `movs #0` that defines it sits in the
 * preheader. A local whose value agbcc can see is zero cannot produce that.
 * A local zero-filled ARRAY indexed by the loop counter produces the right
 * un-reassociated shape but adds an `ldrb`.
 *
 * WAVE 65: `int v`, rather than `u8 v`, is a positive result. It restores the
 * missing `movs #0` / `orrs` pair, takes the candidate from -4 / 34.53% to
 * exact-size / 64.57%, and changes the later loops' allocation. The old claim
 * that every zero local folds was therefore too broad: the declared MODE of a
 * zero local can survive into the OR even when its value is known. The active
 * draft keeps `int`; the remaining residual is allocation/block layout in the
 * duplicated retry loops, not the missing flag operation.
 *
 * The rest of the diff is the knock-on: the flag loop stays ascending-with-index
 * instead of being reversed to a descending counter with a walking pointer, and
 * in the two big passes the PRE hoist picks &unk10[i] where the ROM picks
 * &unk00[i]. Both are register-pressure effects of the one missing live value,
 * so fix the OR operand first and re-measure before touching anything else. */

void sub_0801ADC8(void)
{
  int erased;
  int i;
  int new_var;
  int k;
  int len;
  int v;
  sub_0801B598(gUnknown_0200CC30, (void (**)(void)) gUnknown_0200CC34);
  gUnknown_0200CD08 = 0;
  ((void (*)(void)) gUnknown_0200CC28)();
  len = ((int (*)(int)) ((unsigned int) gUnknown_0200CC24))(gUnknown_0200CC2C);
  for (i = 0; i < 0x10; i++)
  {
    gUnknown_0200CC38.unk20[i] &= 0xf7;
  }

  if (sub_0801B120(0xff) == 0xffff)
  {
    sub_0801A7D8(0, (void *) gUnknown_0200CC2C, len);
  }
  else
  {
    v = 0;
    for (i = 0; i < 0x10; i++)
    {
      if ((gUnknown_0200CC38.unk20[i] & 0x10) != 0)
      {
        gUnknown_0200CC38.unk20[i] = gUnknown_0200CC38.unk20[i] | 8;
        gUnknown_0200CC38.unk20[i] = gUnknown_0200CC38.unk20[i];
        new_var = (gUnknown_0200CC38.unk20[i] | v) & 0xef;
        gUnknown_0200CC38.unk20[i] = new_var;
      }
    }

  }
  sub_0801B120(0);
  for (i = 0; i < 0x10; i++)
  {
    if (((gUnknown_0200CC38.unk20[i] & 0x18) == 0) && (gUnknown_0200CC38.unk00[i] == 0))
    {
      do
      {
        for (k = 0; k < 4; k++)
        {
          sub_0801B5E8(i);
          if (sub_0801B018(i) != 0)
          {
            gUnknown_0200CC38.unk40[i] = gUnknown_02002000[0xfff];
            erased = -1;
            gUnknown_0200CC38.unk00[i] = erased;
            gUnknown_0200CC38.unk10[i] = 0xff;
            gUnknown_0200CC38.unk20[i] |= 4;
            gUnknown_0200CC88.sectorGeneration[i] = 0;
            break;
          }
        }

      }
      while (0);
    }
  }

  for (i = 0; i < 0x10; i++)
  {
    if ((((gUnknown_0200CC38.unk20[i] & 0x10) != 0) && ((gUnknown_0200CC38.unk20[i] & 8) == 0)) && (gUnknown_0200CC38.unk00[i] == 0))
    {
      for (k = 0; k < 4; k++)
      {
        sub_0801B5E8(i);
        if (sub_0801B018(i) != 0)
        {
          gUnknown_0200CC38.unk40[i] = gUnknown_02002000[0xfff];
          gUnknown_0200CC38.unk10[i] = 0xff;
          gUnknown_0200CC38.unk00[i] = -1;
          gUnknown_0200CC38.unk20[i] |= 4;
          gUnknown_0200CC88.sectorGeneration[i] = 0;
          break;
        }
      }

    }
  }

  for (i = 0; i < 0x10; i++)
  {
    if (((gUnknown_0200CC38.unk20[i] & 8) == 0) && (gUnknown_0200CC38.unk00[i] != 0xff))
    {
      gUnknown_0200CC38.unk10[i] |= 0xff;
      (&gUnknown_0200CC38)->unk00[i] |= 0xff;
    }
  }

}





