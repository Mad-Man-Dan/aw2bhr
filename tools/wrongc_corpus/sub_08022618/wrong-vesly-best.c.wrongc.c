#include "global.h"

/* PARKED at 392 of 400 bytes (wave 15, C). Structure, control flow, both `.LC`
 * pool spellings and every relocation are right; the candidate is 8 bytes SHORT
 * and the missing instructions are all zero-extensions agbcc elides here and
 * did not elide in the ROM.
 *
 * `ax` and `by` are `int` with an explicit `(u16)` cast at each use, NOT `u16`
 * locals -- that one change is worth 16 of the 24 bytes this was short before.
 * A `u16` local is a promoted SImode pseudo, so reading it costs nothing and
 * combine then folds the `* 2` of the definition into the `* 2` of the pointer
 * scaling and emits a single `lsls #2`; the ROM keeps `lsls rN, ax, #0x10` once
 * and `lsrs rM, rN, #0xf` at every use, which only the explicit cast produces.
 * A STRAIGHT-LINE probe cannot tell the two spellings apart -- both fold there
 * -- so this has to be measured in the real function.
 *
 * What is still missing is the same effect on `id`: the ROM stores it with
 * `lsls #0x10; lsrs #0x10` after the `ldrb` and this draft elides that pair.
 * `int id` with `(u16)`/`(s16)` casts fixes those 4 bytes and costs more
 * elsewhere (it re-enables the ax/by folding), so it is not the answer either;
 * the two want different spellings of the same idea and I did not find one that
 * holds both. Ruled out: old_agbcc (identical size), and ax/by as unused
 * parameters overwritten on entry (identical output -- flow deletes the dead
 * incoming copy and the promotion survives). */

void sub_08022618(u16 x, u16 y)
{
  u16 ** const *pp;
  int new_var;
  const struct UnitType * const *pp2;
  struct Unit *rec;
  u8 *p;
  u8 *rows;
  u8 *q;
  u16 *tile;
  s16 id;
  s16 flags;
  int ax;
  int by;
  u16 new_var2;
  int off;
  int t;
  int cap;
  p = gUnknown_08499590;
  if (y >= (*((u16 *) (p + 2))))
  {
    return;
  }
  ax = ((x - (*((u16 *) (p + 0xC)))) & 0xF) * 2;
  by = ((y - (*((u16 *) (p + 0xE)))) & 0xF) * 2;
  t = y * 2;
  rows = p + 0x417A;
  off = (*((u16 *) (rows + t))) + x;
  q = p + 0x51A;
  id = q[off];
  q = p + 0x234A;
  new_var = (id == 0) || (q[off] == 0);
  if ((new_var || (!sub_0802571C(id))) || (gUnknown_08499594[id].flags & 4))
  {
    sub_080225CC(x, y);
  }
  else
  {
    rec = &gUnknown_08499594[id];
    pp = &gUnknown_080909A8;
    tile = ((*(*pp)) + ((u16) ax)) + (((u16) by) * 32);
    tile[0] = 0x360;
    flags = 0;
    pp2 = &gUnknown_080909AC;
    cap = (*pp2)[rec->type].maxAmmo;
    if ((cap != 0) && (rec->ammo <= Div(cap, 3)))
    {
      flags = 1;
    }
    cap = (*pp2)[rec->type].maxFuel;
    if ((cap != 0) && (rec->fuel <= Div(cap, 3)))
    {
      flags += 2;
    }
    if (flags != 0)
    {
      new_var2 = (u16) by;
      (((*(*pp)) + ((u16) ax)) + (new_var2 * 32))[1] = flags + 0xA33C;
    }
    else
    {
      (((*(*pp)) + ((u16) ax)) + (new_var2 * 32))[1] = 0x360;
    }
    tile = ((*(*pp)) + ((u16) ax)) + (new_var2 * 32);
    tile[0x21] = 0x360;
    tile[0x20] = 0x360;
  }
}
