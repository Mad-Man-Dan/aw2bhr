#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E778.
 * sub_0805E778 @ 0x0805E778
 */

void AiSeekRepairProperty(void)
{
  union Unk802C57CBuf v;
  int new_var;
  struct Unk03003338 *p;
  int save;
  int i;
  save = gUnknown_03004784[1];
  p = gUnknown_03003F20;
  gUnknown_03004784[1] = 0x64;
  for (i = 0; i < 2; i++)
  {
    SetWorkingMapPlane(gMap->move);
    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03, gUnknown_030040D8->unk00, 0x78, 1);
    sub_0805A0EC(p);
    if (1)
    {
      v.pos.unk00 = 0x270F;
      AiPopLastNearestCandidate(p, (u16 *) (&v));
      if (v.pos.unk00 == 0x270F)
      {
        break;
      }
    }
    gUnknown_03004784[1] = (new_var = 0, save);
    if (((s8 *) gUnknown_03003340[v.pos.unk02])[v.pos.unk00] <= GetUnitMovementBudget((struct Unit *) gUnknown_030040D8))
    {
      AiPublishAction(v.spos.unk00, v.spos.unk02, 2, 0, new_var);
    }
    else
    {
      AiAdvanceToward(&v);
    }
    gUnknown_03004784[1] = 0;
  }

  gUnknown_03004784[1] = save;
}
asm(".global sub_0805E778\n.thumb_set sub_0805E778, AiSeekRepairProperty\n");
