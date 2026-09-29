#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C870.
 * sub_0802C870 @ 0x0802C870
 */

int IntelMenu_UnitUsability(void)
{
  s16 i;
  int idx;
  int o;
  struct Unit **pu;
  u16 *pg;
  i = 1;
  pu = &gUnits;
  pg = &gUnknown_03003F2C;
  idx = (*pg) + i;
  goto test;
  body:
  i++;

  if (i > 0x32)
  {
    goto out;
  }
  idx = (*pg) + i;
  test:
  o = idx * (sizeof(struct Unit));

  if ((idx = ((struct Unit *) (((u8 *) (*pu)) + o))->type) == 0)
  {
    goto body;
  }
  out:
  if (i == 0x33)
  {
    return 2;
  }

  return 0;
}
asm(".global sub_0802C870\n.thumb_set sub_0802C870, IntelMenu_UnitUsability\n");
