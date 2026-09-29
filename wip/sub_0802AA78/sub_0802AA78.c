#include "global.h"
#include "hardware.h"

/* The +2 and +4 must ride on the loaded BASE, not on the index. Offset-
 * compatible with c_0802AA14.c's `struct Tbl49A2A6` -- unk02 starts at the same
 * +2 there, only its declared LENGTH differs, and a declared length never
 * enters the address arithmetic. See that file's header comment for the
 * derivation: `gUnknown_0849A2A6[sel * 3 + 1]` folds the element into the index
 * and `gUnknown_0849A2A6 + 1` folds it into the relocation; only reaching the
 * array THROUGH a struct member gives the ROM's `adds rB, #2` / `adds rB, #4`
 * with `sel * 6` left as one shared subexpression.
 *
 * A `const s16 (*)[3]` cast -- the obvious 2-D spelling for a three-per-row
 * table -- does NOT work here: agbcc folds the row offset into the RELOCATION
 * (`.word gUnknown_0849A2A6+2`) and spends an extra -fforce-addr word on it. */
struct Tbl49A2A6
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02[1];
    /* 0x04 */ s16 unk04[0x100];
};

/* NOT MATCHED -- configured verdict: 89.3%, size EXACT (2356 bytes),
 * 253 bytes differ. The matcher first reports +0x90, a forced-address pool
 * relocation; the first instruction-region difference begins at +0x398
 * (first differing instruction byte +0x399). These pool differences are not
 * accepted as relocation-equivalent by the configured gate.
 *
 * The asymmetric +6 expression is deliberate. Reassociating only the
 * sub_0802BB74 arm improves pool-load order while avoiding the four-byte GCSE
 * shrink caused by reassociating both arms. The `t` and `zero` bindings are
 * independently verified allocation/lifetime levers. The ROM still
 * materialises signed `q` at the final merge with `lsls #16; asrs #16`; all
 * tested include-based `int qr`/`s16 q` spellings optimize that pair away.
 * See NOTES.md and docs/agbcc-codegen.md (wave 56, W56-Q). Do not seed from
 * the header-expanded best.c/best.json score; it is not this translation unit. */
void sub_0802AA78(void)
{
  struct Unit *unit;
  int zero;
  struct Unk02028360 *obj;
  u8 z;
  s16 k;
  int yIndex;
  s16 *tblUnk04;
  s16 q;
  u16 x;
  s16 y;
  s16 cx;
  int cy;
  int v;
  u16 sel;
  int acc;
  s16 accv;
  u8 army;
  u16 t;
  int n2;
  sub_0802B868();
  x = gUnknown_03003130.unk10;
  y = gUnknown_03003130.unk11;
  v = sub_0802B6C8(x, y);
  unit = &gUnknown_08499594[((struct Map *) gUnknown_08499590)->unit[((struct Map *) gUnknown_08499590)->rowOffset[y] + x]];
  army = ((unit - gUnknown_08499594) >> 6) + 1;
  cx = *((u16 *) (&gUnknown_03003130.unk0c));
  if ((*((s16 *) (&gUnknown_03003130.unk0c))) <= gUnknown_0849A2BE[0])
  {
    cy = gUnknown_0849A2BE[1];
    sel = 0;
  }
  else
  {
    cy = gUnknown_0849A2BE[3];
    sel = 1;
  }
  if ((*((s16 *) (&gUnknown_03002F90))) == 1)
  {
    int n;
    n = gUnknown_0300309C + 0xa0;
    cy = (u16) (cy - n);
  }
  sub_0802A8DC((s16) x, (s16) y, cx, (s16) cy, (s16) sel);
  k = sub_0803EED4((s16) x, (s16) y) & 0x1f;
  sub_0802BAFC((gUnknown_0849A2A6[((s16) sel) * 3] + cx) + gUnknown_0849A284[0x18], cy + gUnknown_0849A284[0x19], (s16) Div(gUnknown_085D583C[k].defense * 10, 10));
  acc = 0;
  switch (k)
  {
    case 6:

    case 8:

    case 10:

    case 11:

    case 14:

    case 20:
      acc = 0x14;
      break;

  }

  if (((u16) v) != 0)
  {
    sub_0802AA14(unit->type, cx, (s16) cy, (s16) sel);
    yIndex = 1;
    ApplyPaletteExt((u16 *) (gUnknown_0810E6E0 + ((gPlayers[army].teamColor - 1) * 0x20)), 0x3e0, 0x20);
    sub_0802B91C(((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3] + cx) + gUnknown_0849A284[0]) & 0x1ff, cy + gUnknown_0849A284[yIndex], unit->type, army, unit->unk07 | unit->unk08, unit->unk05_3, unit->flags, 0);
    if (gUnknown_085D5ABC[unit->type].maxAmmo != 0)
    {
      sub_0801C7DC(gUnknown_081243C4, 3, 1, ((cx + ((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3]) + gUnknown_0849A284[4]) & 0x1ff, ((gUnknown_0849A284[5] + ((s16) cy)) & 0xff) | 0x400, 0x1352, 0);
    }
    else
    {
      sub_0801C7DC(gUnknown_081243C4, 4, 1, ((cx + ((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3]) + gUnknown_0849A284[4]) & 0x1ff, ((gUnknown_0849A284[5] + ((s16) cy)) & 0xff) | 0x400, 0x1352, 0);
    }
    if ((!sub_08026F28(gUnknown_030033EC, ((unit - gUnknown_08499594) >> 6) + 1)) && (sub_08043050(((unit - gUnknown_08499594) >> 6) + 1) & 1))
    {
      sub_0802BB74((cx + ((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3]) + gUnknown_0849A284[6], cy + gUnknown_0849A284[7]);
    }
    else
    {
      q = cx;
      q = (q + ((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3]) + gUnknown_0849A284[6];
      sub_0802BAFC(q, cy + gUnknown_0849A284[7], (unit->hp != 0) ? ((s16) (Div(unit->hp - 1, 10) + 1)) : (0));
    }
    sub_0802BAFC((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3] + cx) + gUnknown_0849A284[8], cy + gUnknown_0849A284[9], unit->fuel);
    if (gUnknown_085D5ABC[unit->type].maxAmmo != 0)
    {
      sub_0802BAFC((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk02[((s16) sel) * 3] + cx) + gUnknown_0849A284[0xa], cy + gUnknown_0849A284[0xb], unit->ammo);
    }
    z = sub_0802706C(unit->type, gUnknown_030033EC, ((unit - gUnknown_08499594) >> 6) + 1);
    if (z != 0)
    {
      if (gUnknown_085D5ABC[unit->type].transportTable[0] == 1)
      {
        sub_0802B8C4(((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0xc]) & 0x1ff, cy + gUnknown_0849A284[0xd], 1);
      }
      else
      {
        sub_0802B8C4(((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0xe]) & 0x1ff, cy + gUnknown_0849A284[0xf], 2);
        sub_0802B8C4(((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0x10]) & 0x1ff, cy + gUnknown_0849A284[0x11], 1);
      }
    }
    else
      if (unit->unk08 != 0)
    {
      struct Unit *e1;
      struct Unit *e2;
      tblUnk04 = ((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk04;
      e1 = &gUnknown_08499594[unit->unk07];
      sub_0802B91C(((tblUnk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0xe]) & 0x1ff, cy + gUnknown_0849A284[0xf], e1->type, army, e1->unk07 | e1->unk08, 0, 0, 1);
      e2 = &gUnknown_08499594[unit->unk08];
      sub_0802B91C(((tblUnk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0x10]) & 0x1ff, cy + gUnknown_0849A284[0x11], e2->type, army, e2->unk07 | e2->unk08, 0, 0, 2);
    }
    else
      if (unit->unk07 != 0)
    {
      struct Unit *e1;
      t = unit->unk07;
      e1 = &gUnknown_08499594[t];
      sub_0802B91C(((((struct Tbl49A2A6 *) gUnknown_0849A2A6)->unk04[((s16) sel) * 3] + cx) + gUnknown_0849A284[0xc]) & 0x1ff, cy + gUnknown_0849A284[0xd], e1->type, army, e1->unk07 | e1->unk08, 0, 0, 1);
    }
    acc = (u16) (acc - unit->unk05_3);
  }
  sub_0802B4D4(cx, (s16) cy, (s16) sel);
  sub_0802B3AC(cx, (s16) cy, (s16) sel);
  gDispIo.disp_ct.win0_enable = 1;
  gUnknown_03002B40 = 0;
  gUnknown_03002B4C = 0;
  n2 = gUnknown_0849A2B2[(s16) sel][(u16) v] + 1;
  t = cx + n2;
  if (((s16) t) > 0xef)
  {
    gUnknown_03002B40 = 0xef;
  }
  else
    if (((s16) t) > 0)
  {
    gUnknown_03002B40 = t;
  }
  t = ((cx + 1) + gUnknown_0849A2B2[(s16) sel][(u16) v]) + gUnknown_0849A2A0[(u16) v];
  zero = 0;
  if (((s16) t) > 0xef)
  {
    gUnknown_03002B4C = 0xef;
  }
  else
    if (((s16) t) > 0)
  {
    gUnknown_03002B4C = t;
  }
  gUnknown_03002EFC = cy;
  gUnknown_03002B44 = cy + 0x38;
  gUnknown_030030A4.bits.win0_enable_bg0 = 1;
  gUnknown_030030A4.bits.win0_enable_bg1 = 1;
  gUnknown_030030A4.bits.win0_enable_bg2 = 1;
  gUnknown_030030A4.bits.win0_enable_bg3 = 1;
  gUnknown_030030A4.bits.win0_enable_obj = 1;
  gUnknown_030030A4.bits.win0_enable_blend = 1;
  gUnknown_030030DC.bits.win0_enable_bg0 = 1;
  gUnknown_030030DC.bits.win0_enable_bg1 = 1;
  gUnknown_030030DC.bits.win0_enable_bg2 = 1;
  gUnknown_030030DC.bits.win0_enable_bg3 = 1;
  gUnknown_030030DC.bits.win0_enable_obj = 1;
  gUnknown_030030DC.bits.win0_enable_blend = 0;
  q = 0;
  obj = sub_0803DF54((s16) x, (s16) y);
  zero = (s16) x;
  if (obj != 0)
  {
    q = obj->unk04;
  }
  if (gUnknown_020288B4[((struct Map *) gUnknown_08499590)->rowOffset[(s16) y] + zero] != 0)
  {
    q = gUnknown_020288B4[((struct Map *) gUnknown_08499590)->rowOffset[(s16) y] + zero];
  }
  if (((s16) q) != 0)
  {
    sub_0801C7DC(gUnknown_081243C4, 2, 1, ((cx + gUnknown_0849A2A6[((s16) sel) * 3]) + gUnknown_0849A284[0x16]) & 0x1ff, ((((s16) cy) + gUnknown_0849A284[0x17]) & 0xff) | 0x400, 0x1352, 0);
    if ((((u32) gGameClock) % 0x1e) > 9)
    {
      sub_0802BAFC((gUnknown_0849A2A6[((s16) sel) * 3] + cx) + gUnknown_0849A284[0x1a], cy + gUnknown_0849A284[0x1b], q);
    }
  }
  else
  {
    accv = acc;
    if (accv != 0)
    {
      sub_0801C7DC(gUnknown_081243C4, 1, 1, ((cx + gUnknown_0849A2A6[((s16) sel) * 3]) + gUnknown_0849A284[0x16]) & 0x1ff, ((((s16) cy) + gUnknown_0849A284[0x17]) & 0xff) | 0x400, 0x1352, 0);
      if ((((u16) (accv - 1)) > 0x12) || ((((u32) gGameClock) % 0x1e) > 9))
      {
        sub_0802BAFC((gUnknown_0849A2A6[((s16) sel) * 3] + cx) + gUnknown_0849A284[0x1a], cy + gUnknown_0849A284[0x1b], accv);
      }
    }
    else
    {
      sub_0801C7DC(gUnknown_081243C4, 0, 1, ((cx + gUnknown_0849A2A6[((s16) sel) * 3]) + gUnknown_0849A284[0x16]) & 0x1ff, ((((s16) cy) + gUnknown_0849A284[0x17]) & 0xff) | 0x400, 0x1352, 0);
    }
  }
}



