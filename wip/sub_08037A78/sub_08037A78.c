#include "global.h"

/* PARKED, wave 50 (W50-L). 27.6% identical, candidate 264 bytes vs 268 (-4),
 * 3 try_match attempts. The whole tail -- the AND quad, the OR quad, the inner
 * increment and the two loop tests -- is byte-exact; the entire residual is in
 * the two loop preheaders. See work/sub_08037A78/NOTES.md for the full diff and
 * the axes already ruled out.
 *
 * Remaining diff, exactly:
 *   1. The ROM computes the map pointer INSIDE the outer loop's condition and
 *      spills it AFTER the guard branch (`ldr r1,[r2]; ldrh r3,[r1,#2]; cmp;
 *      bge; str r1,[sp,#12]`). Writing `map = gUnknown_08499590;` as its own
 *      statement makes agbcc store-then-reload it instead (one extra insn).
 *   2. Same shape one level in: the ROM's inner-loop guard reads the OUTER
 *      pointer from its spill slot and only THEN loads the second pointer into
 *      ip, i.e. the guard precedes the load. Any source spelling puts the
 *      assignment before the guard.
 *   3. Consequences of (1)/(2), not independent facts: the ROM hoists
 *      `map + 0x12` into [sp,#8] and leaves `gUnknown_030032E0 + y * 2` to be
 *      rematerialised in the loop; this candidate does the opposite, and its
 *      frame is 16 bytes where the ROM's is 20. */

struct Map37A78
{
    /* 0x0000 */ u16 unk00;
    /* 0x0002 */ u16 unk02;
    /* 0x0004 */ u8 filler_04[0x12 - 0x04];
    /* 0x0012 */ u8 unk12[0x417a - 0x12];
    /* 0x417a */ u16 unk417a[1];
};

void sub_08037A78(int a1)
{
  const u16 *maskTable;
  struct Map37A78 *outerMap;
  u16 *rowTable;
  struct Map37A78 *map;
  struct Map37A78 *p;
  u16 *dst;
  u8 (*cellRow)[0x417a - 0x12];
  const u16 *src;
  int x;
  int y;
  u8 v;
  y = 0;
  if (y < (map = (struct Map37A78 *) gUnknown_08499590)->unk02)
  {
    outerMap = map;
    maskTable = gUnknown_0849D534;
    do
    {
      x = 0;
      if (x < outerMap->unk00)
      {
        p = (struct Map37A78 *) gUnknown_08499590;
        rowTable = gUnknown_030032E0;
        do
        {
          cellRow = &p->unk12;
          v = (*cellRow)[p->unk417a[y] + x];
          if (v != 0)
          {
            src = ((const u16 *) gUnknown_080A0F38) + gUnknown_08582E74[gUnknown_030040F8[((v >> 3) >> 3) + 1] + 0x12];
            dst = (((u16 *) a1) + gUnknown_03004010[x]) + rowTable[y];
            dst[0] &= maskTable[0];
            dst[2] &= maskTable[1];
            dst[4] &= maskTable[2];
            dst[6] &= maskTable[3];
            dst[0] |= src[0];
            dst[2] |= src[2];
            dst[4] |= src[4];
            dst[6] |= src[6];
          }
        }
        while ((++x) < p->unk00);
      }
    }
    while ((++y) < outerMap->unk02);
  }
}
