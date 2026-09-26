#include "global.h"

/* PARKED at 376/380 bytes. The shape is settled -- prologue, both loops, every
 * guard, the call and the whole epilogue are instruction-for-instruction the
 * ROM, and the pool is in the ROM's order. THE ENTIRE RESIDUAL IS ONE `ldrb`.
 *
 * REMAINING DIFF, exactly: the ROM keeps the -fforce-addr ADDRESS of
 * gUnknown_03003F38 in r4 across the merge and RELOADS the byte at the late
 * test (`ldrb r4,[r4]`); this draft caches the byte itself in r4 at the earlier
 * test and compares against it directly, so the reload is missing. The other
 * differences the diff prints are all knock-on: losing 2 bytes brings the outer
 * loop's back-branch into short range, so the ROM's `bge`/`b` long-jump pair
 * collapses to one `blt` (-2 more), and every later offset shifts.
 *
 * WHY IT HAPPENS: the store `out->v = 0;` sits between the two reads and ought
 * to kill the cached load -- that is the c_0805A95C.c "a non-const global's MEM
 * does not survive the merge" mechanism, and the ROM behaves exactly that way.
 * agbcc here does NOT kill it, and the plausible reason is type-based aliasing:
 * the store is `s16` through a parameter pointer and the load is `u8`, so they
 * are treated as non-conflicting. If that is right, the fix is a spelling that
 * makes the store alias, not a register-allocation lever.
 *
 * RULED OUT BY MEASUREMENT (do not retry):
 *  - Binding the address, `f38 = &gUnknown_03003F38;`, and reading `*f38` at
 *    the two late sites -- the documented c_local workaround. It is STRICTLY
 *    WORSE: it merges the two force-addr reads with the ordinary-pool read as
 *    well, so gUnknown_0816D980 disappears from the pool entirely and the
 *    function loses 8 bytes instead of 4.
 *  - Leaving `off` unbound. That version recomputes the index at the late site
 *    as the ROM does, but then `cells = q + 0x1432` is emitted BEFORE the
 *    `ldrh`, costing an extra `mov rN,ip`. Binding `off` (this draft) fixes the
 *    ordering and the late site still recomputes, because the late test is
 *    written longhand as `props[*(u16 *)rowp + x]` -- that longhand is
 *    load-bearing and must stay.
 *
 * SETTLED AND WORTH KEEPING: gUnknown_0816D978/0816D97C/0816D980 are NOT
 * globals -- the ROM words hold 0x030040D8, 0x08499590 and 0x03003F38, the
 * addresses of real objects (checked in baserom.gba). Binding
 * `p = gUnknown_030040D8;` before `out->v = 0;` is what orders its pool word
 * ahead of gUnknown_030033EC's and hoists the pointer load above the store,
 * both of which the ROM does. Promotion would need
 * "rodata": ["0x0816D978", "0x0816D97C", "0x0816D980"].
 *
 * The record type is sub_0805A514's {u8 x; u8 y; s16 v;}, terminated with
 * 0xFFFF, same as its three siblings in this block. */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void sub_0805A0EC(void *out_)
{
  volatile u8 *new_var;
  int new_var3;
  struct Unk5A514Cell *out;
  struct Unit *u;
  struct Unk030040D8 *p;
  u8 *q;
  u8 *rows;
  u8 *rowp;
  u8 *cells;
  u8 *props;
  int x;
  int y;
  int t;
  int off;
  struct Unk030040D8 *new_var2;
  int key;
  out = out_;
  key = gUnknown_0857680F[gUnknown_030040D8->unk00];
  for (y = 0; y < (*((u16 *) (gUnknown_08499590 + 2))); y++)
  {
    for (x = 0; x < (*((u16 *) gUnknown_08499590)); x++)
    {
      if (((s8) gUnknown_03003340[y][x]) < 0)
      {
        continue;
      }
      q = gUnknown_08499590;
      t = y * 2;
      rows = q + 0x417a;
      rowp = rows + t;
      new_var3 = 0;
      off = (*((u16 *) rowp)) + x;
      cells = q + 0x1432;
      if ((cells[(*((u16 *) rowp)) + x] & 0xe0) != gUnknown_03004084)
      {
        continue;
      }
      if (gUnknown_085767F2[cells[(*((u16 *) rowp)) + x] & 0x1f] != key)
      {
        continue;
      }
      props = q + 0x12;
      if ((props[(*((u16 *) rowp)) + x] != 0) && (props[off] != gUnknown_03003F38))
      {
        continue;
      }
      new_var = (volatile u8 *) (&gUnknown_03003F38);
      if (props[off] == (*new_var))
      {
        u = &gUnknown_08499594[props[off]];
        if (u->unk0b == 5)
        {
          continue;
        }
      }
      p = gUnknown_030040D8;
      new_var2 = p;
      out->v = new_var3;
      if ((props[(*((u16 *) rowp)) + x] != 0) && (props[(*((u16 *) rowp)) + x] != gUnknown_03003F38))
      {
        out->v = out->v + (sub_08042D1C(gUnknown_030033EC, new_var2->unk00) * 5);
      }
      out->v += (s8) gUnknown_03003340[y][x];
      out->x = x;
      out->y = y;
      out++;
    }

  }

  out->v = 0xFFFF;
}
