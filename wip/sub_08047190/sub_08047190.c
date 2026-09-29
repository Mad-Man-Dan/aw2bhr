#include "global.h"

/* Rebuild the panel's unit list in gUnknown_02028DD8.  NOT MATCHED -- wave 66,
 * W66-C.  size: MATCH (1292 bytes), 293 bytes differ (77.3%), first difference
 * at +0x11.  One uninterrupted 300-second permuter run found this large gain
 * but no byte match; the process exited normally and this clean source was
 * re-verified with the configured profile afterward.
 *
 * The gain is the COMBINATION of `o = (rank = 0); n = 0;` and spelling the two
 * mirrored scan tests with the element subscript inline.  The earlier draft
 * used three independent zero assignments and bound `e` in those two scans;
 * that version was section-size exact but only 18.0%.  Do not restore either
 * old spelling without measuring the combination, because the first-loop
 * allocation cascades through the rest of this call-free function.
 *
 * Established here and not worth re-deriving:
 *   - the second parameter is `int`, not `u8` -- the ROM spills r1 with no
 *     `lsls #24; lsrs #24` in front of it (see docs/agbcc-codegen.md).
 *   - Inlining the two `s`-scan element tests is part of the current 77.3%
 *     fixpoint.  Bound `e` was locally attractive for pool order in wave 56,
 *     but the chained initialization changes that allocation context.
 *   - `gUnknown_02028DD8[n] = s; n++;` as two statements, not `[n++]` -- the
 *     ROM stores before it increments.
 *   - the inner `j` loop of the first loop really is invariant in the original:
 *     it re-tests and re-stores the same `gUnknown_081BA068[i]` 63 times.
 *     LICM hoists the load, which is why it is an `ldrsb` there and a
 *     `ldrb`+shift pair in the outer test.
 *   - the third level uses `list[q3] |= 0xff`, the first two use `= 0xff`.
 * The remaining class is allocation/order, not missing statement structure.
 * Wave 71: chaining `o` with `n` instead of `rank` makes the first three zero
 * moves resemble the ROM, but grows the unit to 1296 bytes and collapses the
 * configured score to 19.0%.  Fixing `rank` to r3 grows it to 1304 bytes and
 * 10.7%.  Both were reverted to this clean 1292-byte, 77.3% fixpoint. */

void sub_08047190(void *arg, int a2)
{
  u8 list[0x40];
  struct Unit *e;
  s8 rank;
  s8 i;
  s8 k;
  s8 t;
  s8 g;
  u8 j;
  u8 s;
  u8 n;
  u8 o;
  u8 q;
  u8 q2;
  u8 q3;
  u8 v;
  s16 c1;
  s16 c2;
  s16 d1;
  s16 d2;
  int lv0;
  o = (rank = 0);
  n = 0;
  for (i = 0; i <= 0x18; i++)
  {
    if (gUnknown_081BA068[i] > 0)
    {
      for (j = 1; j <= 0x3f; j = lv0)
      {
        lv0 = j + 1;
        if (gUnknown_081BA068[i] > rank)
        {
          rank = gUnknown_081BA068[i];
        }
      }

    }
  }

  if (gUnknown_02028E18 == 0)
  {
    q3 = 1;
    for (k = 1; k < (rank + q3); k++)
    {
      for (t = 0; t <= 0x18; t++)
      {
        if (gUnknown_081BA068[t] == k)
        {
          for (s = 1; s <= 0x3f; s++)
          {
            if (t == gUnknown_08499594[gUnknown_03003F2C + s].type)
            {
              gUnknown_02028DD8[n] = s;
              n++;
            }
          }

        }
      }

    }

    gUnknown_02028DD8[n] = 0xff;
    ((struct Unk0804769C *) arg)->unk21 = n;
    if (a2 == 0)
    {
      for (q = 0; q < n; q++)
      {
        list[q] = gUnknown_02028DD8[q];
      }

    }
    else
    {
      for (g = 0; g <= 0x64; g++)
      {
        for (q = 0; q < n; q++)
        {
          v = gUnknown_02028DD8[q];
          e = &gUnknown_08499594[gUnknown_03003F2C + v];
          if ((a2 == 1) && (e->hp != g))
          {
            continue;
          }
          if ((a2 == 2) && (e->fuel != g))
          {
            continue;
          }
          if (a2 == 3)
          {
            if (g == 0)
            {
              if (e->ammo != 0)
              {
                continue;
              }
              if (gUnknown_085D5ABC[e->type].unk11 == 0)
              {
                continue;
              }
            }
            else
              if (g == 0x64)
            {
              if (e->ammo != 0)
              {
                continue;
              }
              if (gUnknown_085D5ABC[e->type].unk11 != 0)
              {
                continue;
              }
            }
            else
              if (e->ammo != g)
            {
              continue;
            }
          }
          list[o] = v;
          o++;
        }

      }

    }
  }
  else
  {
    for (k = rank; k > 0; k--)
    {
      for (t = 0; t <= 0x18; t++)
      {
        if (gUnknown_081BA068[t] == k)
        {
          for (s = 1; s <= 0x3f; s++)
          {
            e = &gUnknown_08499594[gUnknown_03003F2C + s];
            if ((*e).type == t)
            {
              gUnknown_02028DD8[n] = s;
              n++;
            }
          }

        }
      }

    }

    gUnknown_02028DD8[n] = 0xff;
    ((struct Unk0804769C *) arg)->unk21 = n;
    if (a2 == 0)
    {
      for (q = 0; q < n; q++)
      {
        list[q] = gUnknown_02028DD8[q];
      }

    }
    else
    {
      for (g = 0x64; g >= 0; g--)
      {
        for (q = 0; q < n; q++)
        {
          v = gUnknown_02028DD8[q];
          e = &gUnknown_08499594[gUnknown_03003F2C + v];
          if ((a2 == 1) && (e->hp != g))
          {
            continue;
          }
          if ((a2 == 2) && (e->fuel != g))
          {
            continue;
          }
          if (a2 == 3)
          {
            if (g == 0)
            {
              if (e->ammo != 0)
              {
                continue;
              }
              if (gUnknown_085D5ABC[e->type].unk11 == 0)
              {
                continue;
              }
            }
            else
              if (g == 0x64)
            {
              if (e->ammo != 0)
              {
                continue;
              }
              if (gUnknown_085D5ABC[e->type].unk11 != 0)
              {
                continue;
              }
            }
            else
              if (e->ammo != g)
            {
              continue;
            }
          }
          list[o] = v;
          o++;
        }

      }

    }
  }
  o = 0;
  for (q = 0; q < n; q++)
  {
    v = list[q];
    if (v == 0xff)
    {
      continue;
    }
    e = &gUnknown_08499594[gUnknown_03003F2C + v];
    if (e->flags & 8)
    {
      continue;
    }
    gUnknown_02028DD8[o] = v;
    list[q] = 0xff;
    o++;
    if (!(e->flags & 0x10))
    {
      continue;
    }
    c1 = e->unk07 - gUnknown_03003F2C;
    c2 = e->unk08 - gUnknown_03003F2C;
    for (q2 = 0; q2 < n; q2++)
    {
      if ((list[q2] != c1) && (list[q2] != c2))
      {
        continue;
      }
      gUnknown_02028DD8[o] = list[q2];
      e = &gUnknown_08499594[gUnknown_03003F2C + gUnknown_02028DD8[o]];
      list[q2] = 0xff;
      o++;
      if (!(e->flags & 0x10))
      {
        continue;
      }
      d1 = e->unk07 - gUnknown_03003F2C;
      d2 = e->unk08 - gUnknown_03003F2C;
      for (q3 = 0; q3 < n; q3++)
      {
        if ((list[q3] == d1) || (list[q3] == d2))
        {
          gUnknown_02028DD8[o] = list[q3];
          list[q3] |= 0xff;
          o++;
        }
      }

    }

  }

  gUnknown_02028DD8[o] = 0xff;
}




