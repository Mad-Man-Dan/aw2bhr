#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804F3C8.
 * sub_0804F3C8 @ 0x0804F3C8
 */

/* MATCHED -- wave 80 (W80-B), byte-exact. The three `.rodata` words are the
 * -fforce-addr address constants for &gUnknown_03001FBC, &gUnknown_02029A10
 * and &gUnknown_020296B0 (ROM symbols gUnknown_081360AC/B0/B4), carried by
 * promotion.
 *
 * Parked since wave 37, at 94.1% since wave 77 with "two register names".
 * Three edits closed it, and all three are the ORIGINAL REUSING ONE SCRATCH
 * VARIABLE (`f`) where the draft had a fresh local per value:
 *  - `f = gUnknown_020296B0[c].unk1e[k] + k * 5;` -- the sum goes into f, the
 *    same pseudo as the first region's f, so it is global-by-flow and lands
 *    in r6 in both places; the addends in THIS order (the address is expanded
 *    first, then k * 5, then the load -- the ROM's instruction order); k is
 *    its own global pseudo (r4) and is what SpawnThirdEffectAndProjectile receives. The
 *    draft's `u16 n` and `(f = k) * 5` were two pseudos where the ROM has
 *    one, and every earlier swap of the addends kept `(f = k)` and therefore
 *    kept a copy (that is the 78.2% waves 77 and 80 both measured).
 *  - `f = gUnknown_020296B0[c].unk0c[k]; if (f != 0xff)` -- the same variable
 *    again (ROM `ldrh r6, [r0]; cmp r6, #0xff`).
 *  - `int v; v = e1->unk20; if ((v & 0xf) == 1)` instead of the read-site
 *    volatile cast. `(u16 & 0xf)` is shortened to HImode, so a reloaded value
 *    is used through a paradoxical SUBREG that local_alloc will not tie
 *    (`ands r0, r1` under every volatile spelling); an int-typed read is a
 *    `(zero_extend (mem))` that cse does not forward the store into, and it
 *    ties (`ands r1, r0`). See TCopterFigure_Loop's header; measured on a
 *    six-variant probe and on both twins.
 * Kept from waves 37/77: entry-first operand order in dst->x/y, the u16
 * `new_var` group stride, `[c * 10 + f * 2]`, the byte-offset-first `p`, the
 * gUnknown_085D6C88 argument expression shared with c_0804FCA4.c. */
void BomberFigure_Loop(void)
{
  struct Unk02029A10 *e1;
  struct Unk02029A10 *entry;
  struct Unk02029A10 *dst;
  u16 c;
  u16 e;
  u16 f;
  unsigned short new_var;
  int v;
  u16 k;
  s16 w;
  u16 *p;
  u16 *row;
  c = gUnknown_03001470[gUnknown_03001FBC].unk30;
  e = gUnknown_03001470[gUnknown_03001FBC].unk34;
  e1 = (struct Unk02029A10 *) (((e * (sizeof(struct Unk02029A10))) + (c * (sizeof(struct Unk02029A10Group)))) + ((u8 *) gUnknown_02029A10));
  e1->unk20 += gUnknown_02029B94[c][e];
  v = e1->unk20;
  if ((v & 0xf) == 1)
  {
    if ((++e1->unk22) == 5)
    {
      e1->unk22 = 0;
    }
    f = e1->unk22;
    dst = (struct Unk02029A10 *) (((f * (sizeof(struct Unk02029A10))) + (c * (sizeof(struct Unk02029A10Group)))) + ((u8 *) gUnknown_02029A10));
    new_var = sizeof(struct Unk02029A10Group);
    dst->x = ((struct Unk02029A10 *) (((gUnknown_08552148[c] * (sizeof(struct Unk02029A10))) + (c * new_var)) + ((u8 *) gUnknown_02029A10)))->x + gUnknown_08553524[(c * 10) + (f * 2)];
    dst->y = ((struct Unk02029A10 *) (((gUnknown_08552148[c] * (sizeof(struct Unk02029A10))) + (c * new_var)) + ((u8 *) gUnknown_02029A10)))->y + gUnknown_08553524[((c * 10) + (f * 2)) + 1];
    SpawnDebrisEffect(c, f);
  }
  k = gUnknown_020296B0[c].unk18;
  f = gUnknown_020296B0[c].unk1e[k] + k * 5;
  if (gUnknown_03004508 == f)
  {
    f = gUnknown_020296B0[c].unk0c[k];
    if (f != 0xff)
    {
      struct Unk02029A10 *q;
      SpawnThirdEffectAndProjectile(c, k);
      q = (struct Unk02029A10 *) (((e * (sizeof(struct Unk02029A10))) + (c * (sizeof(struct Unk02029A10Group)))) + ((u8 *) gUnknown_02029A10));
      q->unk1e++;
      PlayMusicOrSfx(gUnknown_085D6C88[gUnknown_03004580[c][1]].unk0c[gUnknown_03004580[c][2] - 1][gUnknown_020296B0[c].unk1a & 1]);
      gUnknown_020296B0[c].unk1a++;
    }
    else
    {
      struct Unk02029A10 *r;
      r = (struct Unk02029A10 *) ((c * (sizeof(struct Unk02029A10Group))) + ((u8 *) gUnknown_02029A10));
      if ((r->unk18 != (-1)) && IsSlotScriptActiveAt(r->unk18))
      {
        sub_080156E8(r->unk18, gUnknown_02029BA8[c].unk04);
      }
    }
    gUnknown_020296B0[c].unk18++;
  }
  StepFigureSlide(c, e);
  w = StepFigureHitFlash2(c, e, gUnknown_03001FBC);
  p = *((u16 **) ((c * (sizeof(u16 *))) + ((u8 *) gUnknown_084C3F78)));
  entry = (struct Unk02029A10 *) (((e * (sizeof(struct Unk02029A10))) + (c * (sizeof(struct Unk02029A10Group)))) + ((u8 *) gUnknown_02029A10));
  entry->x += gUnknown_08553B28[c][w];
  entry->y -= gUnknown_085644D4[(row = gUnknown_02028E5C[c])[1]];
  StepBomberBobOrKnock(c, e, gUnknown_03001FBC);
  SetSlotSpritePosition(gUnknown_03001FBC, entry->x, entry->y - (*p));
}
asm(".global sub_0804F3C8\n.thumb_set sub_0804F3C8, BomberFigure_Loop\n");
