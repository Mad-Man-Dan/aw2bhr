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
    u64 pair;
    u16 lo;
    u16 x;
    u16 y;
    u16 z;

    pair = a4.unk00;
    pair &= 0xFFFF0000;
    pair >>= 16;
    lo = a4.unk00;
    z = a4.unk04;
    x = (((u32)(a1 & 0x1FF) << 16) | ((u32)pair << 16)) >> 16;
    y = (a2 & 0xFF) | lo;
    PutSpriteExt(a5, x, y, (u16 *)a3, z);
}

