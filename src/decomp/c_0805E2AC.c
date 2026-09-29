#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805E2AC.
 * sub_0805E2AC @ 0x0805E2AC
 */

void AiMoveLanderToNearestPort(void)
{
  union Unk802C57CBuf v;
  u16 best;
  int t;
  int n;
  int i;
  int nextBest;
  int hi;
  u32 lo;
  int xv;
  void (**fn)(int, int, int, int, int);
  best = 0x7FFF;
  v.raw = 0x270F | (v.raw & 0xFFFF0000);
  n = AiBuildPropertyList();
  for (i = 0; i < n; i++)
  {
    if (gUnknown_085766E4[i].unk02 != 6)
    {
      continue;
    }
    fn = &gUnknown_030013EC;
    (*fn)(gUnknown_085766E4[i].unk00, gUnknown_085766E4[i].unk01, 0x17, 0x78, 0);
    t = (s8) gUnknown_03003340[gUnknown_030040D8->unk03][gUnknown_030040D8->unk02];
    nextBest = t << 16;
    if (t >= ((s16) best))
    {
      continue;
    }
    best = ((u32) nextBest) >> 16;
    xv = *((volatile u8 *) (&gUnknown_085766E4[i].unk00));
    lo = v.raw & 0xFFFF0000;
    lo |= xv;
    v.raw = lo;
    hi = (*((volatile u8 *) (&gUnknown_085766E4[i].unk01))) << 16;
    lo &= 0x0000FFFF;
    lo |= hi;
    v.raw = lo;
  }

  if (v.pos.unk00 == 0x270F)
  {
    AiFallbackMove();
  }
  SetWorkingMapPlane(gMap->danger);
  fn = &gUnknown_030013EC;
  (*(&gUnknown_030013EC))(v.pos.unk00, v.raw >> 16, 0x17, 0x78, 0);
  AiAdvanceTowardUnseeded(&v);
}
asm(".global sub_0805E2AC\n.thumb_set sub_0805E2AC, AiMoveLanderToNearestPort\n");
