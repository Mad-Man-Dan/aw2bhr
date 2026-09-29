#include "global.h"
#include "map.h"

void sub_080607E8(void)
{
  struct Unk02028360 *p;
  struct Unit *u;
  int band;
  int a;
  int b;
  int i;
  long long new_var;
  int c;
  u8 d;
  band = gUnknown_03004080 & 0x1f;
  p = sub_0803E354(7);
  if (p == 0)
  {
    return;
  }
  a = p->unk00;
  b = p->unk01 + 4;
  i = 0;
 do { loop: c = gMap->unit[gMap->rowOffset[b] + (a + i)]; if (c == 0) { d = ((u8 *) gFactoryUnitSchedule)[(band * 3) + i]; new_var = a + i; if (d != 0) { u = sub_08025CC8(new_var, (char) b, d); u->unk09 = 0; u->unk0a = c; u->unk0b = sub_08061178(d - 1); } } } while (0);
  i++;
  if (2 >= i)
  {
    goto loop;
  }
}
