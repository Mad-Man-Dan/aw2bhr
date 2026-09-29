#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805DCD4.
 * sub_0805DCD4 @ 0x0805DCD4
 */

#include "map.h"
/* Picks the best cell for the current unit (first by the
 * gUnknown_03003340 score, falling back to gMap->unk376A), spends one of the
 * unit's top-two-bit charges and calls AiPublishAction with the resulting
 * direction.
 *
 * `dir` is a u8 local: with an int the ROM's argument setup order (dir loaded
 * last) is not reproduced. The unit's top-two-bit charge counter at +0x09 is
 * a bitfield, viewed through Unk5DCD4Unit because struct Unk030040D8 still
 * models that byte as unk07[2]. */
struct Unk5DCD4Unit
{
    u8 filler_00[0x09];
    u8 unk09_0 : 6;
    u8 unk09_6 : 2;
};

void AiDeliberateDrop(void)
{
  int bx;
  int by;
  int best;
  int tx;
  int ty;
  u8 dir;
  int x;
  int y;
  struct Unk802C57C pt;
  bx = 0;
  by = 0;
  best = 0;
  tx = -1;
  ty = 0;
  dir = 0;
  if ((((struct Unk5DCD4Unit *)gUnknown_030040D8)->unk09_6 != 0) && ((gUnknown_030040D8->unk00 == 7) || (gUnknown_030040D8->unk00 == 0x14)))
  {
    GenerateUnitMovementMap(gUnknown_030040D8);
    MapMarkHalo(0x79);
    for (y = 0; y < gMap->height; y++)
    {
      for (x = 0; x < gMap->width; x++)
      {
        if (((((((s8) gUnknown_03003340[y][x]) >= 0) && (IsCellCapturableByCurrentArmy(x, y) == 1)) && (gMap->unit[gMap->rowOffset[y] + x] == 0)) && (((s8) gUnknown_03003340[y][x]) > best)) && (AiPickDropCellBeside(x, y, (u16 *)&pt) == 0))
        {
          bx = x;
          by = y;
          tx = pt.unk00;
          ty = pt.unk02;
          best = (s8) gUnknown_03003340[y][x];
        }
      }

    }

    if (tx > 0)
    {
      gUnknown_084995A0[gMap->property[gMap->rowOffset[ty] + tx]].unk03[0]++;
      ((struct Unk5DCD4Unit *)gUnknown_030040D8)->unk09_6--;
      if (bx != tx)
      {
        dir = (u8) ((tx - bx) + 3);
      }
      else
        if (by != ty)
      {
        dir = (u8) ((by - ty) + 2);
      }
      AiPublishAction(tx, ty, 8, dir, 0);
    }
    else
    {
      best = 0;
      tx = -1;
      for (y = 0; y < gMap->height; y++)
      {
        for (x = 0; x < gMap->width; x++)
        {
          if ((((((s8) gUnknown_03003340[y][x]) >= 0) && (gMap->unk376A[gMap->rowOffset[y] + x] > best)) && (gMap->unit[gMap->rowOffset[y] + x] == 0)) && (AiPickDropCellBeside(x, y, (u16 *)&pt) == 0))
          {
            bx = (x = x);
            by = y;
            tx = pt.unk00;
            ty = pt.unk02;
            best = gMap->unk376A[gMap->rowOffset[y] + x];
          }
        }

      }

      if (tx > 0)
      {
        ((struct Unk5DCD4Unit *)gUnknown_030040D8)->unk09_6--;
        if (bx != tx)
        {
          dir = (u8) ((tx - bx) + 3);
        }
        else
        {
          if (by != ty)
          {
            dir = (u8) ((by - ty) + 2);
          }
        }
        AiPublishAction(tx, ty, 8, dir, 0);
      }
    }
  }
}
asm(".global sub_0805DCD4\n.thumb_set sub_0805DCD4, AiDeliberateDrop\n");
