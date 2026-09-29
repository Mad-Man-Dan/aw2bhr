#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08050E08.
 * sub_08050E08 @ 0x08050E08
 */

/* PARKED SIZE-EXACT at 284/284 bytes, 94.4% identical (16 bytes differ).
 * Wave 63, W63-F, improving the Wave-36 draft.
 * scratch against PlaceThirdEffectOnFigure, which is the same shape and is stuck on the
 * same residual.
 *
 * SETTLED, do not re-derive:
 *   - One u16 parameter. `tbl` first, then w/h, then k, then row -- that order
 *     gives the ROM's exact seven-word pool in the ROM's exact order, and the
 *     whole first half is instruction-for-instruction the ROM's.
 *   - gUnknown_085D81E8 HAD TO BE A STRUCT (declared in unknown-globals.h with
 *     the evidence). As a flat `u16 [][5][4]` the constant last subscript is
 *     folded into the base with `adds rBase,#4`, where the ROM keeps it in the
 *     `ldrh` displacement.
 *   - gUnknown_08553B18 is a real two-halfword ROM object, NOT a force-addr
 *     pool word: it sits exactly 4 bytes before gUnknown_08553B1C and is read
 *     `ldrh` off `base + a * 2`.
 *   - `gUnknown_03004580[a][2]` must be written out INLINE in all three places
 *     (both subscripts and the `== 2` test) and left to CSE. Binding it to a
 *     local makes it its own statement and reorders the whole row/k/c address
 *     group.
 *
 * Wave 63 settled the missing-sign-extension fact. Assigning each u16 memory
 * operand to an int local INSIDE the addition is the required source-order
 * barrier: it preserves ldrh, gives the operand SImode precision and keeps the
 * s16 local's explicit sign extension. This recovered all 12 missing bytes.
 *
 * THE REMAINING 16 BYTES are one address-allocation/order residual around the
 * cell lookup. The ROM keeps tbl in sl and h in r9, while this draft swaps
 * those two registers. It also computes k*4 before the gUnknown_03004580[a][2]
 * load and forms (k*4 + second*16) before adding row; this draft performs the
 * load before k*4 and associates row + second*16 first. A controlled byte-
 * pointer/cell spelling with `k *= 4` reached the desired source order but
 * changed the parameter/register allocation and did not improve the fixpoint.
 *
 * PRIOR WAVE RESIDUAL (now closed):
 *
 *   ROM   lsls r0,r4,#16 ; asrs r0,r0,#16 ; ldrh r1,[r1] ; adds r0,r0,r1
 *   here  ldrh r0,[r0]   ; adds r0,r0,r4
 *
 * MEASURED THIS WAVE, and it is a rule rather than a guess:
 *   - `s16 local + s16 local` KEEPS both extensions. The `x = x + w` arm here
 *     is already exact, which is what proves the local types are right.
 *   - `s16 local + u16 memory` DROPS the extension: agbcc narrows the whole
 *     addition to HImode, so reading the promoted (zero-extended) pseudo is
 *     free. That is arm 1 and both call arguments -- all three residuals.
 *   - `int` locals with an explicit `(s16)` cast at each use do NOT restore it;
 *     the cast folds for the same reason and the `ldrh` on the way in becomes
 *     `ldrsh` as well. Measured: 51.8%, -28. (Retyping struct Unk08553B1CPt to
 *     u16 to keep the `ldrh` was part of that test and was reverted.)
 *   - `u16` locals with an explicit `(s16)` cast fold identically -- wave 34
 *     recorded this for PlaceThirdEffectOnFigure and it replicates here.
 * So the operand had to change, not x; the inline int assignment is that
 * spelling. Do not spend another wave on casts around x. */
void PlaceThirdEffectOnWholeUnit(u16 a)
{
  struct Unk08553B1CPt (*tbl)[3][4];
  struct Unk08553B1CPt (*row)[4];
  int k;
  s16 x;
  s16 y;
  s16 w;
  s16 h;
  int mem_x;
  s16 *new_var;
  int mem_y;
  tbl = gUnknown_08553B1C[gUnknown_020296B0[a].unk1c];
  w = gUnknown_085D81E8[gUnknown_03004580[a][1]][gUnknown_020296B0[a].unk18].unk00[a];
  h = (mem_y = gUnknown_085D81E8[gUnknown_03004580[a][1]][gUnknown_020296B0[a].unk18].unk04);
  k = gUnknown_08562128[gUnknown_03004580[gUnknown_0300453C][4]];
  if (k == 4)
  {
    k = 1;
  }
  row = tbl[gUnknown_03004580[a][1]];
  new_var = &((struct Unk08553B1CPt *) (((u8 *) row) + ((k * (sizeof(struct Unk08553B1CPt))) + (gUnknown_03004580[a][2] * 0x10))))->unk00;
  x = *new_var;
  y = ((struct Unk08553B1CPt *) (((u8 *) row) + ((k * (sizeof(struct Unk08553B1CPt))) + (gUnknown_03004580[a][2] * 0x10))))->unk02;
  if (gUnknown_03004580[a][2] == 2)
  {
    x = x + (mem_x = gUnknown_08553B18[a]);
  }
  else
  {
    x = x + w;
    y = y + h;
  }
  SetEffectScreenPosition(a, gUnknown_03001FBC, x + (mem_x = gUnknown_02029A10[a].entries[0].x), y + (mem_y = gUnknown_02029A10[a].entries[0].y));
}
asm(".global sub_08050E08\n.thumb_set sub_08050E08, PlaceThirdEffectOnWholeUnit\n");
