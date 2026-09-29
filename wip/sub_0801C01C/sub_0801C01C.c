#include "global.h"

/* WAVE 71: a mutable DImode local is the first source mechanism found for the
 * unique zero-shift residual.  pair >>= 16 preserves gcc's two-word shift and
 * emits the ROM's zero << 16 path; the 32-bit mask plus distributed x shifts
 * also recover the 0xFFFF0000 pool word and high-half a1 expression.  This is
 * the strongest configured draft so far (108/116, 27.6%).  The remaining
 * eight bytes are allocation/lifetime: the ROM keeps a1, a3 and the shifted
 * low half in r8/sl/r9, while this spelling uses only r8. */
void sub_0801C01C(u16 a1, u16 a2, void *a3, struct UnkVec a4, int a5)
{
  int new_var;
  unsigned long long new_var4;
  u64 pair;
  u16 lo;
  u16 x;
  u16 y;
  u16 z;
  long long new_var2;
  int new_var3;
  new_var = 0xFFFF0000;
  pair = a4.unk00;
  pair &= new_var;
  if (1)
  {
    pair >>= 16;
  }
  lo = a4.unk00;
  new_var4 = a4.unk04;
  z = new_var4;
  new_var2 = pair;
  new_var = (((u32) new_var2) << 6) << 10;
  new_var3 = (((u32) (a1 & 0x1FF)) << 16) | new_var;
  x = new_var3 >> 16;
  y = a2 & 0xFF;
  y = y | lo;
  PutSpriteExt(a5, x, y, (u16 *) a3, z);
}

