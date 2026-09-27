#include "global.h"

/* Reads the save flash chip's ID.
 *
 * It sends the chip's enter-ID-mode command sequence, waits, reads the device
 * and maker codes through a tiny read routine that sub_0808AD6C copies onto
 * the stack, sends the exit sequence, waits again, and returns
 * (device << 8) | maker.
 *
 * Why the C looks odd:
 *  - Each delay loop jumps into the middle of its own body, so one store
 *    serves both the initial value and every decrement. Writing the loop the
 *    natural way emits a separate initialising store and is 4 bytes longer.
 *  - `ptest` points at the same counter as `p`; the second loop's exit test
 *    reads through it. It changes nothing at runtime and only affects which
 *    register the compiler picks.
 */

u16 sub_0808AAF4(void)
{
  u16 buf[0x20];
  u8 (*readFlash1)(u8 *);
  u16 flashId;
  int v;
  vu16 i;
  vu16 *p;
  vu16 *ptest;
  sub_0808AD6C(buf);
  readFlash1 = (u8 (*)(u8 *)) (((s32) buf) + 1);
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xAA;
  *((vu8 *) (0x0E000000 + 0x2AAA)) = 0x55;
  *((vu8 *) (0x0E000000 + 0x5555)) = 0x90;
  p = &i;
  v = 20000;
  goto store1;
  body1:
  v = (*p) - 1;

  store1:
  *p = v;

  if ((*p) != 0)
  {
    goto body1;
  }
  flashId = readFlash1((u8 *) (0x0E000000 + 1)) << 8;
  flashId |= readFlash1((u8 *) 0x0E000000);
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xAA;
  *((vu8 *) (0x0E000000 + 0x2AAA)) = 0x55;
  *((vu8 *) (0x0E000000 + 0x5555)) = 0xF0;
  p = &i;
  v = 20000;
  ptest = &(*p);
  goto store2;
  body2:
  v = (*p) - 1;

  store2:
  *p = v;

  if ((*ptest) != 0)
  {
    goto body2;
  }
  return flashId;
}
