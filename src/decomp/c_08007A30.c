#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007A30.
 * sub_08007A30 @ 0x08007A30
 */

/*
 * sub_08007A30 -- slide the two overlay markers toward their next position and
 * draw them.
 *
 * gActiveMap->overlayX and overlayY are screen positions in 1/16 of a pixel.
 * Each frame an eighth of the remaining distance to the target is added, so
 * they ease in. gActiveMap->overlayState runs the two stages:
 *
 *   state 0:    X eases toward 0x730 and is stopped at 0x750, which also moves
 *               to state 0xA; Y eases toward 0x9C0, stopped at 0x9A0.
 *   state 0xA:  X eases toward 0x7A0 and is stopped at 0x780, which moves to
 *               state 0x14 and arms a 30-frame timer; Y eases toward 0x950,
 *               stopped at 0x970.
 *   state 0x14: count the timer down; at zero go back to state 0.
 *
 * Whatever the state, both positions are then shifted down to whole pixels,
 * have bit 10 set, and are handed to PutOamHi: object 0x64 with the sprite
 * data at gUnknown_08488880, object 0x86 with gUnknown_08488888.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `a`, `b` and `raw` are pinned to r3, r5 and r0. The original holds the
 *     two eased values in those registers across both switch arms and the
 *     tail; left to itself the compiler chooses others.
 *   - overlayY is reached by stepping the `pair` pointer on from &overlayX
 *     rather than by name, and the sign extension at the end is written out as
 *     a shift left by 16 and an arithmetic shift right by 20.
 */

void sub_08007A30(void)
{
  register int a asm("r3");
  register int b asm("r5");
  u8 t;
  struct ActiveMap *p;
  s16 *pair;
  p = gActiveMap;
  switch (p->overlayState)
  {
    case 0:
      a = p->overlayX;
      a += (0x730 - a) >> 3;
      if (a <= 0x750)
    {
      a = 0x750;
      p->overlayState = 0xA;
    }
      pair = &gActiveMap->overlayX;
      *pair = a;
      pair++;
      b = *pair;
      b += (0x9C0 - b) >> 3;
      if (b > 0x99F)
    {
      b = 0x9A0;
    }
      *pair = b;
      break;

    case 0xA:
      a = p->overlayX;
      a += (0x7A0 - a) >> 3;
      if (a > 0x77F)
    {
      a = 0x780;
      p->overlayState = 0x14;
      gActiveMap->overlayTimer = 0x1E;
    }
      pair = &gActiveMap->overlayX;
      *pair = a;
      pair++;
      b = *pair;
      b += (0x950 - b) >> 3;
      if (b <= 0x970)
    {
      b = 0x970;
    }
      *pair = b;
      break;

    case 0x14:
      t = p->overlayTimer;
      p->overlayTimer = t - 1;
      if (((s8) t) <= 0)
    {
      gActiveMap->overlayState = 0;
    }
      break;

  }

  {
    register int raw asm("r0");
    struct ActiveMap *tail = gActiveMap;
    raw = (u16)tail->overlayX;
    raw <<= 16;
    a = raw >> 20;
    raw = (u16)tail->overlayY;
    raw <<= 16;
    b = raw >> 20;
  }
  a |= 0x400;
  PutOamHi(0x64, a, gUnknown_08488880, 0);
  b |= 0x400;
  PutOamHi(0x86, b, gUnknown_08488888, 0);
}
