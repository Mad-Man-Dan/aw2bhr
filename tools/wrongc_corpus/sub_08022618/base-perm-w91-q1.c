#include "global.h"
#include "map.h"

/* PARKED, wave 91 (W91-A): size-exact 400/400, 29.0%, register allocation
 * only. Analysis and the measured variants are in work/sub_08022618/NOTES.md.
 * The map through struct Map members, `id` assigned INSIDE the first test,
 * `flags` its own s16, ax/by in two steps, and the two tables named directly
 * (gUnknown_08499580 / gUnknown_085D5ABC) instead of through the force-addr
 * words gUnknown_080909A8 / gUnknown_080909AC. */

static inline u16 inline_fn(u16 *arg0, u16 arg1)
{
  return arg0[arg1];
}

static inline u16 inline_fn2(int arg0)
{
  return (u16) arg0;
}

void sub_08022618(u16 x, u16 y)
{
  struct Unit *rec;
  u16 *tile;
  s16 id;
  s16 flags;
  int ax;
  int by;
  int off;
  int cap;
  if (y >= ((struct Map *) gUnknown_08499590)->height)
  {
    return;
  }
  ax = (x - ((struct Map *) gUnknown_08499590)->camX) & 0xF;
  by = (y - ((struct Map *) gUnknown_08499590)->camY) & 0xF;
  ax *= 2;
  by *= 2;
  off = inline_fn(((struct Map *) gUnknown_08499590)->rowOffset, y) + x;
  flags = (id = ((struct Map *) gUnknown_08499590)->unitUnk[off]);
  if ((((flags == 0) || (((struct Map *) gUnknown_08499590)->unk234A[off] == 0)) || (!sub_0802571C(id))) || (gUnknown_08499594[id].flags & 4))
  {
    sub_080225CC(x, y);
  }
  else
  {
    rec = &gUnknown_08499594[id];
    tile = (gUnknown_08499580 + inline_fn2(ax)) + (inline_fn2(by) * 32);
    tile[0] = 0x360;
    flags = 0;
    cap = gUnknown_085D5ABC[rec->type].maxAmmo;
    if ((cap != 0) && (rec->ammo <= Div(cap, 3)))
    {
      flags = 1;
    }
    cap = gUnknown_085D5ABC[rec->type].maxFuel;
    if ((cap != 0) && (rec->fuel <= Div(cap, 3)))
    {
      flags += 2;
    }
    if (flags != 0)
    {
      ((gUnknown_08499580 + ((u16) ax)) + (inline_fn2(by) * 32))[1] = flags + 0xA33C;
    }
    else
    {
      ((gUnknown_08499580 + inline_fn2(ax)) + (inline_fn2(by) * 32))[1] = 0x360;
    }
    tile = (gUnknown_08499580 + inline_fn2(ax)) + (inline_fn2(by) * 32);
    tile[0x21] = 0x360;
    tile[0x20] = 0x360;
  }
}
