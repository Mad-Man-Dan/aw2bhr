#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A6DC.
 * sub_0805A6DC @ 0x0805A6DC
 */

/* Wave 52, W52-B.  MATCHED.
 *
 * DO NOT DELETE `u->y += 0;`.  It is a dead store and it is LOAD-BEARING.
 * See the wave-52 chapter in docs/agbcc-codegen.md: without it agbcc hoists
 * both gUnits and the gUnknown_03003F2C bound out of the loop,
 * check_dbra_loop reverses the loop into a countdown, and the function comes
 * out 4 bytes short.  prescan_loop records the store, invariant_p then refuses
 * every MEM, and the store itself is deleted again before output -- so the
 * barrier costs zero bytes.  The out-pointer's type is NOT the lever: tried as
 * `struct Unk5A514Cell *` (48.1%, -4) and as the plain `u8 *` below (52.9%,
 * size-exact); neither blocks the hoist.
 *
 * Consumes the same {u8 x; u8 y; s16 v;} 4-byte record c_0805A514.c produces
 * and c_0805A744.c walks, terminated with v == 0xFFFF, and returns how many it
 * wrote as a gUnknown_03003F20-relative element count.  `new_var` is needed:
 * storing 0xFFFF directly costs an extra `adds r0,r1,#0`.
 */

int AiListLandersWithFreeBerth(u8 *out)
{
  int new_var;
  struct Unit *u;
  int i;
  for (i = gUnknown_03003F2C; i < (gUnknown_03003F2C + 0x40); i++)
  {
    u = &gUnits[i];
    u->y += 0;
    if (u->type != 0x17)
    {
      continue;
    }
    if ((u->unk09 >> 6) > 1)
    {
      continue;
    }
    *out = u->x;
    *(out + 1) = u->y;
    *((u16 *) (out + 2)) = 0;
    out += 4;
  }

  new_var = 0xFFFF;
  *((u16 *) (out + 2)) = new_var;
  return (out - ((u8 *) gUnknown_03003F20)) >> 2;
}
asm(".global sub_0805A6DC\n.thumb_set sub_0805A6DC, AiListLandersWithFreeBerth\n");
