#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804F18C.
 * sub_0804F18C @ 0x0804F18C
 */

/* MATCHED (wave 87, W87-B) -- see work/BomberFigure_Init/W87-notes.md.
 * Permuter win from a hand-built 99.3% draft; body kept verbatim as the
 * permuter emitted it (wave-83 rule: do not tidy a permuter win).  Three
 * constructs closed a 68-wave "register allocation only" park:
 *   1. oam.tileNum = g * 0x100 (W86-E's twin find, kept);
 *   2. the (s16)v + X arithmetic written through a `static inline` helper --
 *      semantically identical and inlined away, but it re-cuts the live
 *      ranges and is what puts the 0xb4 stride and `pos` in the ROM's
 *      registers.  decomp-permuter invents these helpers itself;
 *   3. the two-bit guard reads *(u8 *)&gUnknown_03004504 TWICE, inline at
 *      each test, instead of binding it to `flags`.  That is what makes the
 *      ROM's `ands r0,r1` write the CONSTANT's register.  This REFUTES this
 *      park's settled claim that six spellings were probed and only the
 *      single-local one gives the ROM's two separate mask tests.
 * `int flags;` is now unused and the stray `;` is where its assignment was;
 * both are left exactly as the permuter produced them. */

struct UnkPosPair
{
    u16 x;
    u16 y;
};
static inline int inline_fn(int arg0, u16 arg1)
{
    return ((s16)arg0) + arg1;
}

void BomberFigure_Init(void)
{
  struct OamData oam;
  u16 pal;
  struct UnkPosPair *pos;
  struct UnkPosPair *e1;
  struct UnkPosPair *e2;
  int v;
  int flags;
  CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *) (&oam));
  gUnknown_03001470[gUnknown_03001FBC].unk28 = gUnknown_0300451C * 8;
  gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
  gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
  gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
  oam.hFlip = gUnknown_0300453C ^ 1;
  pal = gUnknown_08551D0C[gUnknown_0300453C][0];
  oam.paletteNum = pal;
  oam.tileNum = gUnknown_0300453C * 0x100;
  oam.priority = 2;
  gUnknown_0300454C[0] = oam.priority;
  SetSlotSpriteAttrs(gUnknown_03001FBC, *((struct UnkVec *) (&oam)));
  pos = (struct UnkPosPair *) GetFigurePositionTable(gUnknown_085D6A48[gUnknown_03004580[gUnknown_0300453C][1]][0], gUnknown_03004580[gUnknown_0300453C][3]);
  v = gUnknown_085644E0[gUnknown_0300453C][gUnknown_0300450C];
  ;
  if ((*((u8 *) (&gUnknown_03004504))) & 1)
  {
    if (!((*((u8 *) (&gUnknown_03004504))) & 0x40))
    {
      v = 0;
    }
  }
  gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 = (e1 = &pos[(gUnknown_0300453C * 5) + gUnknown_0300451C])->x;
  gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x = inline_fn(v, gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04);
  gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 = (e2 = &pos[(gUnknown_0300453C * 5) + gUnknown_0300451C])->y;
  gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;
  StartFigureEntrySlide(gUnknown_0300453C, gUnknown_0300451C, 0, 0x32);
  SetSlotSpritePosition(gUnknown_03001FBC, inline_fn(v, gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x), gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
}
asm(".global sub_0804F18C\n.thumb_set sub_0804F18C, BomberFigure_Init\n");
