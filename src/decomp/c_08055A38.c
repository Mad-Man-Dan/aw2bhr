#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08055A38.
 * sub_08055A38 @ 0x08055A38, sub_08055D4C @ 0x08055D4C
 */

/* Wave 54, W54-H. MATCHES byte-for-byte (trymatch exit 0; the three reloc
 * notes are gUnknown_020298E0+0x1a vs gUnknown_020298FA and
 * gUnknown_08551D22+6 vs gUnknown_08551D28, i.e. the same addresses spelled
 * against the disassembler's invented row symbols).
 *
 * Three levers, in the order they were worth bytes:
 *   1. `x = gUnknown_085521B4[side][i][0];` as its OWN statement in the
 *      row[2] == 1 arm. Writing the subscript directly in the store puts the
 *      destination address first; the ROM evaluates the source first, which is
 *      what a separate assignment gives. Same instruction count either way, so
 *      the size matched at 85.7% before this was found.
 *   2. `n` is REUSED as the inner counter of the last two nested loops. The
 *      "two disjoint counters in one function are usually ONE local" rule --
 *      with a separate `j` the whole r2/r3/r4/r5 block permutes.
 *   3. `new_var` (the permuter's name, kept because the declaration ORDER
 *      matters): binding the `unk00 != 0` test to an int local swaps ip and sl
 *      between &gUnknown_02029A10 and side*20 in the second search loop. That
 *      was the last 7 bytes, found by decomp-permuter in 14k iterations after
 *      hand work took it to 99.1%; no source lever tried reached it.
 * `k = i + E7C[..] * 5` (not `E7C[..] * 5 + i`) is a readout of the ROM's
 * `adds r2, r6, r2`, and binding it at all is what puts the p subscript ahead
 * of the destination -- also a permuter find, at 85.7% -> 94.4%. */
void BuildHitListForSide(u16 count, u16 side)
{
  u16 i;
  u16 out;
  u16 n;
  u16 x;
  int new_var;
  u16 *p;
  u16 *row;
  int k;
  out = 0;
  i = 0;
  if (count == 0)
  {
    gUnknown_020298E0[side].unk1a[0] = 0xff;
    return;
  }
  row = gUnknown_085D6A48[gUnknown_03004580[side][1]];
  if (row[2] == 1)
  {
    for (i = 0; i < 5; i++)
    {
      x = gUnknown_085521B4[side][i][0];
      gUnknown_020298E0[side].unk1a[out] = x;
      out++;
      if (out == count)
      {
        gUnknown_020298E0[side].unk1a[count - 1] = gUnknown_08552148[side];
        gUnknown_020298E0[side].unk1a[count] = 0xff;
        break;
      }
    }

  }
  else
  {
    while (out != count)
    {
      x = gUnknown_0855218C[side][i][0];
      new_var = gUnknown_02029A10[side].entries[x].unk00 != 0;
      if (new_var)
      {
        gUnknown_020298E0[side].unk1a[out] = x;
        out++;
        gUnknown_020298E0[side].unk1a[out] = 0xff;
      }
      i++;
      if (i == 5)
      {
        i = 0;
      }
    }

  }
  n = 0;
  for (i = 0; i < 5; i++)
  {
    if (gUnknown_02029A10[side].entries[i].unk00 != 0)
    {
      n++;
    }
  }

  if (count < n)
  {
    for (i = 0; i < 5; i++)
    {
      if (gUnknown_02029A10[side].entries[i].unk01 == 1)
      {
        gUnknown_020298E0[side].unk1a[0] = i;
      }
    }

  }
  i = 0;
  p = gUnknown_08551E64[gUnknown_030045A0[gUnknown_0300450C]];
  while (gUnknown_020298E0[side].unk1a[i] != 0xff)
  {
    k = i + (gUnknown_08551E7C[(side * 2) + gUnknown_0300450C] * 5);
    gUnknown_020298E0[side].unk0c[i] = p[k] + gUnknown_08551D22[gUnknown_030045A0[side ^ 1]][3];
    gUnknown_020298E0[side].unk3a[i]++;
    i++;
  }

  if (gUnknown_02029BE8[side ^ 1] != 1)
  {
    for (i = 0; gUnknown_020298E0[side].unk1a[i] != 0xff; i++)
    {
      for (n = 0; n < 5; n++)
      {
        if (gUnknown_020298E0[side].unk1a[i] == n)
        {
          gUnknown_020298E0[side].unk3a[n]++;
        }
      }

    }

  }
  else
  {
    for (i = 0; gUnknown_020298E0[side].unk1a[i] != 0xff; i++)
    {
      for (n = 0; n < 5; n++)
      {
        if (gUnknown_020298E0[side].unk1a[i] == n)
        {
          gUnknown_020298E0[side].unk3a[n] = 0xc;
        }
      }

    }

  }
}
asm(".global sub_08055A38\n.thumb_set sub_08055A38, BuildHitListForSide\n");

void BuildDeathLists(u16 p0, u16 p1)
{
    u16 v[2];
    u16 alive[2];
    u16 num[2];
    u16 mode[2];
    int flag;
    int side;
    int i;
    int j;

    mode[0] = p0;
    mode[1] = p1;
    flag = 0;
    side = gUnknown_0300450C;
    for (i = 0; i < 2; i++) {
        num[i] = 0;
        alive[i] = 0;
    }
    v[1] = GetDeathEffectKind(0);
    v[0] = GetDeathEffectKind(1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            if (gUnknown_02029A10[i].entries[j].unk00 != 0
             && gUnknown_02029A10[i].entries[j].unk01 == 0)
                alive[i]++;
            if (gUnknown_020296BC[i][j] != 0xff)
                num[i]++;
            gUnknown_02029808[i].unk30[j] = v[i];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            gUnknown_02029808[i].unk44[j] = gUnknown_08552D80[v[i]].unk0c[i];
            gUnknown_02029808[i].unk58[j] = gUnknown_08552D80[v[i]].unk14[i];
        }
    }
    if (mode[side] == 2) {
        flag = 1;
        alive[side] = 0;
        sub_080560A4(num[side], side ^ 1, alive[side ^ flag], 2, v[side]);
    }
    if (flag == 0) {
        PlanSideDeaths(0, num[0], alive[1], v[1]);
        PlanSideDeaths(1, num[1], alive[0], v[0]);
    }
    if (mode[side] == 2)
        alive[side] = 0;
    if (mode[side ^ 1] == 2)
        alive[side] = 0;
}
asm(".global sub_08055D4C\n.thumb_set sub_08055D4C, BuildDeathLists\n");
