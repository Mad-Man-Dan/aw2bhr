#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E7A8.
 * sub_0804E7A8 @ 0x0804E7A8
 */

/* MATCHED, wave 90 (W90-A). The x/y assignments are dead but load-bearing:
 * cse hands the call's two narrow arguments the def-site `lsl #16`, which is
 * what puts both `lsls` before arg 1's `ldrsh` and both `asrs` after it, as
 * in the ROM. Without them (the old inline-only draft) the same-address
 * pool/sl-r9 residual of waves 79-87 comes back. Full record:
 * work/TankFigure_Loop/W87-notes.md. */
#include "global.h"

void TankFigure_Loop(void)
{
  struct Unk02029A10 *entry;
  u16 *p1, *p2, *row;
  u16 c, e, t, w;
  s16 x, y;
  c = gUnknown_03001470[gUnknown_03001FBC].unk30;
  e = gUnknown_03001470[gUnknown_03001FBC].unk34;
  t = sub_080156C4(gUnknown_03001FBC);
  StepFigureSlide(c, e);
  w = StepFigureHitFlash(c, e, gUnknown_03001FBC);
  p1 = gUnknown_084C3F70[c];
  p2 = gUnknown_084C3F78[c];
  if (t == 0x1C)
  {
    sub_0803B48C(gUnknown_085D6C88[gUnknown_03004580[c][1]].unk0c[gUnknown_03004580[c][2] - 1][gUnknown_020296B0[c].unk1a & 1]);
    gUnknown_020296B0[c].unk1a++;
  }
  entry = (struct Unk02029A10 *) (((e * (sizeof(struct Unk02029A10))) + (c * (sizeof(struct Unk02029A10Group)))) + ((u8 *) gUnknown_02029A10));
  entry->x += gUnknown_08553B28[c][w];
  entry->y -= gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];
  x = entry->x - (*p1);
  y = entry->y - (*p2);
  SetSlotSpritePosition(gUnknown_03001FBC, entry->x - (*p1), entry->y - (*p2));

}
asm(".global sub_0804E7A8\n.thumb_set sub_0804E7A8, TankFigure_Loop\n");
