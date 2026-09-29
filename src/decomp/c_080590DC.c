#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080590DC.
 * sub_080590DC @ 0x080590DC
 */

/* MATCHED in Wave 63 under the configured profile (264/264 bytes and
 * relocations exact). The allocation lever is reusing `j` to materialize the
 * packed 0x270F sentinel before assigning it to v.raw. That keeps the later
 * outer/inner loop counters in the ROM's r2/r3 order. Found by the bounded
 * permuter pass at iteration 28; do not simplify the sentinel assignment
 * without re-running try_match. */

void sub_080590DC(void *a1)
{
  union Unk802C57CBuf v;
  union Unk802C57CBuf *in;
  int best;
  s8 t;
  int i;
  int j;
  in = a1;
  best = 0x78;
  j = (v.raw & 0xFFFF0000) | 0x270F;
  v.raw = j;
  gUnknown_030013EC(in->pos.unk00, in->pos.unk02, 0x10, 7, 0);
  for (i = 0; i < gMap->height; i++)
  {
    for (j = 0; j < gMap->width; j++)
    {
      if (((s8) gUnknown_03003340[i][j]) < 0)
      {
        continue;
      }
      if (((s8) gUnknown_03003340[i][j]) >= best)
      {
        continue;
      }
      switch (gMap->terrain[gMap->rowOffset[i] + j] & 0x1f)
      {
        case TERRAIN_SEA:

        case TERRAIN_PORT:

        case TERRAIN_SHOAL:
          t = gUnknown_03003340[i][j];
          best = t;
          v.raw = (i << 16) | ((u16) j);
          break;

      }

    }

  }

  if (v.pos.unk00 != 0x270F)
  {
    AiAdvanceToward(&v);
  }
  else
  {
    AiFallbackMove();
  }
}
