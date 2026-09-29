#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062AE4.
 * sub_08062AE4 @ 0x08062AE4
 */

/* Tallies, for each 4x4-cell block of the map, how many cells IsTerrainOwnedByUnitsTeam
 * accepts (a) versus how many carry a nonzero top-three-bit terrain flag (b),
 * and stores the ratio as a percentage in gUnknown_0202DAD8[j][i].unk28. The
 * source is permuter output, kept because it is what the bytes require. */
void AiCalcPropertyControlPerBlock(void)
{
  int i;
  int j;
  int x;
  int y;
  int a;
  int b;
  int off;
  int v;
  for (j = 0; j <= 9; j++)
  {
    for (i = 0; i <= 0xb; i++)
    {
      a = 0;
      b = 0;
      for (y = j * 4; y < ((j * 4) + 4); y++)
      {
        for (x = i * 4; x < ((i * 4) + 4); x++)
        {
          ;
          if (gUnknown_085767D5[gMap->terrain[gMap->rowOffset[y] + x] & 0x1f] != 0)
          {
            gUnknown_0202DAD8[j][i].unk2a++;
            if (IsTerrainOwnedByUnitsTeam(gUnknown_03003F2C, gMap->terrain[gMap->rowOffset[y] + x]) == 1)
            {
              a++;
            }
            else
            {
              off = gMap->rowOffset[y] + x;
              if ((gMap->terrain[off] & 0xe0) != 0)
              {
                b++;
              }
            }
          }
        }

      }

      if ((((s16) gUnknown_0202DAD8[j][i].unk2a) != 0) && ((a + b) != 0))
      {
        if (a > b)
        {
          v = (((a * 100) / ((s16) gUnknown_0202DAD8[j][i].unk2a)) / 6) - 1;
        }
        else
        {
          v = ((((b * 100) / ((s16) gUnknown_0202DAD8[j][i].unk2a)) / 6) - 1) | 0x8000;
        }
        gUnknown_0202DAD8[j][i].unk28 = v;
      }
    }

  }

}
asm(".global sub_08062AE4\n.thumb_set sub_08062AE4, AiCalcPropertyControlPerBlock\n");
