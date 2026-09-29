#include "global.h"
#include "map.h"

/*
 * sub_08022618 -- draw the 2 x 2 status icon for the unit at map (x, y).
 *
 * Does nothing if y is below the map. The icon goes into the tilemap buffer
 * gUnknown_08499580 at the tile's screen position (x and y relative to the
 * camera, wrapped to 16 x 16). If the tile has no unit, unk234A is 0 there,
 * sub_0802571C rejects the unit, or the unit's flags bit 2 is set,
 * sub_080225CC(x, y) draws the tile instead. Otherwise three corners get tile
 * 0x360 and the top-right gets 0xA33C plus a mark: bit 0 for ammo at or below
 * a third of the maximum, bit 1 for fuel at or below a third.
 *
 * Why the C looks odd:
 *   - `id` is assigned inside the first test, and ax/by are masked and then
 *     doubled in separate statements. Both change the compiled code.
 *   - This draft does not match yet; see data/parked.json.
 */
inline static u16 inline_fn(u16 *arg0, u16 arg1)
{
  return arg0[arg1];
}

inline static u16 inline_fn2(int arg0)
{
  return (u16) arg0;
}

void sub_08022618(u16 x, u16 y)
{
  struct Unit *rec;
  u16 *tile;
  s16 id;
  s16 flags;
  int new_var;
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
  by = (ax *= 2);
  off = inline_fn(((struct Map *) gUnknown_08499590)->rowOffset, y) + x;
  flags = (id = ((struct Map *) gUnknown_08499590)->unitUnk[off]);
  new_var = (flags == 0) || (((struct Map *) gUnknown_08499590)->unk234A[off] == 0);
  if ((new_var || (!sub_0802571C(id))) || (gUnknown_08499594[id].flags & 4))
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
