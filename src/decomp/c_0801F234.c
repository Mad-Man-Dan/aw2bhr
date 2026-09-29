#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F234.
 * sub_0801F234 @ 0x0801F234
 */

/*
 * LoadTilePoolGraphic -- load graphic `a` into the next free tiles of its tile pool.
 *
 * GetTilePoolForGraphic picks the pool entry for `a`; the entry keeps a list of loaded
 * graphics (unk08[]) and a count (unk05). The next list slot already holds the
 * first free tile number. The graphic is gUnknown_0848B780[a].unk00 by .unk01
 * tiles; its data (from GetGraphicSourceAddress) is copied with CpuFastSet to the pool's
 * VRAM base (unk00) plus 32 bytes per tile. The slot then records `a`, the
 * count goes up, and the following slot gets the next free tile number.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The copy size is held in a `short` (it is at most 0x7FE0), the
 *     destination in a `u8 *`, the tile number is copied into a second `u16`,
 *     and the width is read through a separate `dims` pointer. Each of these
 *     splits a value the compiler would otherwise share, which gives the
 *     original's register choice.
 */
void LoadTilePoolGraphic(int a)
{
  struct Unk0200F920 *e;
  int i;
  short wordCount;
  u16 tile;
  const struct Unk0848B780 *dims;
  u32 n;
  u16 tileCopy;
  u8 *dst;
  i = GetTilePoolForGraphic(a);
  e = &gUnknown_0200F920[i];
  tile = e->unk08[e->unk05].unk00;
  dims = gUnknown_0848B780 + a;
  tileCopy = tile;
  n = (*dims).unk00 * gUnknown_0848B780[a].unk01;
  CpuFastSet(GetGraphicSourceAddress(a, i), dst = ((u8 *) e->unk00) + ((tileCopy & 0x3FF) * 32), (wordCount = (n & 0x3FF) * 32) / 4);
  e->unk08[e->unk05].unk02 = a;
  e->unk05++;
  e->unk08[e->unk05].unk00 = tileCopy + n;
}
asm(".global sub_0801F234\n.thumb_set sub_0801F234, LoadTilePoolGraphic\n");
