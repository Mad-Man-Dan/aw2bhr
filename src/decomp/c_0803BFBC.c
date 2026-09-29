#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BFBC.
 * FillMatchSettingsRecord @ 0x0803BFBC
 */

/* WAVE 87 (W87-E): **MATCHED**, 536/536 (`configured`). Parked waves 50, 62,
 * 66, 73, 80, 82, 83 -- the last five of them on "ONLY the two trailing-loop
 * PREHEADERS", i.e. the wave-50 tension: `k` in the for-init gives the right
 * loop BODY but a source-first preheader; `k` written inline gives the right
 * preheader ORDER but fold folds the +1 into the biv.
 *
 * THE TENSION IS FALSE. It only exists while SOME of the preheader values are
 * source statements and the rest are LICM hoists -- then the source ones are
 * emitted first as a block and the hoists after, and no ordering of the two
 * groups is the ROM's interleave. Make EVERY value in the preheader a source
 * statement, in the ROM's order, inside a hand-inverted loop (zero-trip `if`
 * plus `do/while`), and nothing is left for LICM to order:
 *
 *     i = 0;                                    <- movs r3,#0
 *     if (i <= 3)
 *     {
 *         dst = d->unk0d;                       <- mov  r4,sb   (cse: a copy)
 *         t   = gUnknown_03003FC0.unk02 - 0xb4; <- ldrb r1,[r1,#2] / subs #0xb4
 *         tbl = gUnknown_020280D4;              <- ldr  r6,=gUnknown_020280D4
 *         k   = t * 28 + 1;                     <- lsls/subs/lsls/adds r2,r0,#1
 *         do { ...; i++; } while (i <= 3);
 *     }
 *
 * The table's force-addr `ldr` sits BETWEEN the subtract half and the multiply
 * half of what wave 82 read as "ONE expression": it is not one expression in
 * the original, it is two statements with the table bind written between them.
 * Wave 82's "no statement boundary produces this" is refuted -- the boundary
 * that produces it is the one INSIDE the arithmetic, not around it.
 *
 * SECOND FACT, worth as much as the first: A POINTER-VARIABLE SUBSCRIPT AND AN
 * ARRAY SUBSCRIPT PUT THE TWO REGISTERS OF THE ADD IN OPPOSITE ORDER.
 * `tbl[i + k]` with `u8 *tbl` emits `adds r0,r6,r0` (pointer first); the ROM
 * and the old array spelling `gUnknown_020280D4[i + k]` emit `adds r0,r0,r6`
 * (index first). Binding the table to a pointer local to place its pool load
 * therefore costs the body's operand order -- 4 bytes, one insn in each of the
 * two loops. Casting the bind back to an integer sum restores it:
 *     dst[i] = *(u8 *)((i + k) + (int)tbl);
 * which keeps the index as the first operand while the bind still owns the
 * `ldr`. 94.8%% -> 98.7%% (staged preheaders) -> MATCH (index-first sum).
 *
 * PROMOTION NOTE: this match needs its .rodata pool word placed --
 *     "rodata": ["0x08091130"]   in this function's data/promoted.json entry,
 * then tools/split_rodata.py and tools/gen_lds.py before building. The four
 * reported reloc differences are all the same-address artefacts already
 * recorded in the park (gUnknown_03004002 = gUnknown_03003FC0+0x42,
 * gUnknown_03003FFD = +0x3d, gUnknown_085C77E0 = gUnknown_085C77DC+4).
 *
 * Everything settled in waves 73/80 is unchanged and still load-bearing: the
 * two if/else stores for `d->unk02` (cross-jumped into the ROM's arm order),
 * the int-typed switch subject with `case 0: default:` written first, and the
 * `do { d->unk08 = GetLoadedMapArmyCount(); d->unk15 = 0; } while (0);` wrapper that
 * puts d in r5 and &d->unk11 in r6. Do not tidy the wrapper away. */

void FillMatchSettingsRecord(void *a1)
{
  struct Unk08580934 *d = a1;
  int v;
  u8 i;
  u8 j;
  int k;
  int t;
  u8 *dst;
  u8 *tbl;
  d->unk00 = gPlaySt.fog;
  d->unk01 = gPlaySt.coAbilities;
  d->unk03 = gPlaySt.turnLimit;
  d->unk04 = gPlaySt.propertyFunds;
  d->unk07 = gPlaySt.captureLimit;
  if (gPlaySt.savingEnabled)
  {
    d->unk02 = 1;
  }
  else
  {
    d->unk02 = gUnknown_0200C420.unk0e;
  }
  if (gPlaySt.randomWeatherOn == 1)
  {
    v = 0;
  }
  else
  {
    v = gPlaySt.defaultWeather;
    switch (v)
    {
      case 0:

      default:
        v = 1;
        break;

      case 1:
        v = 3;
        break;

      case 2:
        break;

    }

  }
  d->unk06 = v;
  d->unk17 = BuildUnlockedCoList();
  d->unk18 = GetUnlockedCoList();
  for (i = 0; i <= 3; i++)
  {
    d->unk09[i] = gPlaySt.aiControlled[i + 1];
    d->unk11[i] = gPlaySt.unk42[i + 1];
  }

  if (d->unk2c == 1)
  {
    d->unk02 = gPlaySt.animOpts;
    for (i = 0; i <= 3; i++)
    {
      d->unk20[i] = gPlayers[i + 1].co;
    }

    for (i = 0; i <= 3; i++)
    {
      d->unk0d[i] = gPlayers[i + 1].teamColor;
    }

 do { d->unk08 = GetLoadedMapArmyCount(); d->unk15 = 0; } while (0);
    d->unk16 = 0x80;
  }
  else
  {
    for (i = 0; i <= 3; i++)
    {
      for (j = 0; gUnknown_020288A0[j] != 0xFF; j++)
      {
        if (gUnknown_020288A0[j] == gPlaySt.co[i + 1])
        {
          d->unk1c[i] = j;
          break;
        }
      }

    }

    if (((u8) (gPlaySt.mapID + 0x4c)) <= 0xb)
    {
      i = 0;
      if (i <= 3)
      {
        dst = d->unk0d;
        t = gPlaySt.mapID - 0xb4;
        tbl = gUnknown_020280D4;
        k = t * 28 + 1;
        do
        {
          dst[i] = *(u8 *) ((i + k) + (int) tbl);
          i++;
        } while (i <= 3);
      }

    }
    else
    {
      i = 0;
      if (i <= 3)
      {
        dst = d->unk0d;
        k = gPlaySt.mapID * 0x5c;
        tbl = ((u8 *) gUnknown_085C77DC) + 4;
        do
        {
          dst[i] = *(u8 *) ((i + k) + (int) tbl);
          i++;
        } while (i <= 3);
      }

    }
    d->unk08 = GetMapArmyCount(gPlaySt.mapID);
    LoadMapAndCountProperties(&d->unk15, &d->unk16);
  }
}
asm(".global sub_0803BFBC\n.thumb_set sub_0803BFBC, FillMatchSettingsRecord\n");
