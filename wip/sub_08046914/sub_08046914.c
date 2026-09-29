#include "global.h"

/* PARKED, wave 49 (W49-J), 356 bytes against 368 (-12).
 *
 * The SHAPE is settled and reads clean against the ROM: the call sequence, the
 * two constants 0x969/0x961, the doubled sub_08026C6C call in the else arm, the
 * `break` out of the three-iteration scan, and the (s16)-fused divide all line
 * up instruction for instruction.
 *
 * THE RESIDUAL IS REGISTER PRESSURE, and it is 12 bytes in one direction: this
 * draft holds MORE live values than the ROM, so it never spills. The ROM parks
 * `a + 0x38` in a STACK SLOT (`str r0,[sp,#8]`, reloaded before the second
 * divide) and keeps &gUnknown_08499578 -- the address of the -fforce-addr pool
 * word at 0x0812A110 -- in r8, reloading the pointer through it at every call
 * (`mov r3,r8; ldr r2,[r3]`). This draft instead keeps the pool word's own
 * address in sl AND the loaded pointer in r4, which is one value too many, and
 * `t` then stays in r7 instead of spilling. Per the wave-17 reading, the ROM
 * recomputing where the draft parks means the ORIGINAL had more locals here,
 * not fewer -- the next attempt should split `t` and the 0x8000/0 constants
 * rather than hunt for a spelling of the divides.
 *
 * MEASURED: `x = sub_08014CEC(gfx) / 2 - 0x50;` as its own statement is
 * REQUIRED -- written inline as `a - (.../2 - 0x50)` agbcc reassociates to
 * `(a + 0x50) - .../2` and hoists a `movs #80` before the call. Speculative
 * (u32) casts on `(a + 0x50) / 8` and `(a + 0x60) / 8` cost 8 bytes and were
 * removed; those two are already `lsrs` without help, unlike sub_08046778's.
 *
 * Wave 71: the target keeps the early gUnknown_08499578 value live while its
 * address-constant pseudo dies before the later direct-symbol loads.  Moving
 * that materialisation into a binding statement or into the first call's
 * argument was measured: both move the load, but rotate `a`, `b`, and `gfx`
 * away from the ROM allocation and shrink the frame to 8 bytes.  A later
 * volatile/cast spelling compiled byte-identically to this 356-byte fixpoint.
 */

/* Lays out one unit's info panel: the portrait, two text rows, an optional
 * third, and a per-terrain icon.
 *
 * gTextTable IS a real table, not an agbcc -fforce-addr pool word: it is
 * indexed by a computed subscript (`lsls #2; adds`) BEFORE anything is
 * dereferenced. The words at 0x0812A110 and 0x0812A114 are the opposite case --
 * they hold 0x08499578 and 0x084C20C0, i.e. they are private address constants
 * for gUnknown_08499578 and gUnknown_084C20C0 -- so both are spelled honestly
 * here and neither is declared.
 *
 * `lsrs r0,#0x1f; adds; asrs #1` after sub_08014CEC is a SIGNED divide by two,
 * not a shift, and the result is centred with `w / 2 - 0x50`. The `lsls #0xd;
 * asrs #0x10` that follows is a `/ 8` fused with an (s16) cast, where the
 * unbiased `(a + 0x38) / 8` further down is unsigned because a is u8.
 *
 * `t` has to be a u32 local: the ROM spills `a + 0x38` and reloads it before
 * the second divide, and across the spill combine has lost the range fact that
 * makes the divide unsigned. The two `(a + 0x50) / 8` uses are recomputed from
 * the parameter, so they keep it. */

void sub_08046914(u8 a, u8 b)
{
  u8 *gfx;
  int hiInit;
  int x;
  unsigned int aCopy;
  u16 *buf;
  long t;
  u16 i;
  int hi;
  int zero;
  gfx = gTextTable[gUnknown_085D583C[b].nameIndex];
  x = (sub_08014CEC(gfx) / 2) - 0x50;
  hiInit = 0x8000;
  hi = hiInit;
  zero = 0;
  buf = gUnknown_08499578;
  sub_080149C0((s16) (((a - x) + 4) / 8), 1, buf, gfx, hi, zero);
  t = a;
  t = t + 0x38;
  sub_08014A5C(t / 8, 3, gUnknown_08499578, 0x960, hi, zero);
  if (gUnknown_02028DD4 == 0)
  {
    sub_08014A5C((a + 0x50) / 8, 3, gUnknown_08499578, 0x969, hi, zero);
  }
  aCopy = a;
  if (sub_08026C6C(b) == 0)
  {
    sub_08014A5C((aCopy + 0x50) / 8, 5, gUnknown_08499578, 0x969, 0x8000, 0);
  }
  else
  {
    sub_08014B0C((unsigned short) ((a + 0x60) / 8), 5, gBG0TilemapBuffer, sub_08026C6C(b), 0x8000, 0);
  }
  for (i = 0; i <= 2; i++)
  {
    if (gUnknown_085D5ABC[gUnknown_084C20C0[i]].repairTable[b] != 0)
    {
      sub_08014A5C(t / 8, 7, gBG0TilemapBuffer, 0x961, 0x8000, 0);
      break;
    }
  }

  sub_08013AEC();
}



