#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016F38.
 * sub_08016F38 @ 0x08016F38
 */

/* The save block: the state sub_08016F38 writes out and sub_08017208
 * (src/decomp/c_08017208.c) reads back in. Both files carry their own copy of
 * this declaration. SaveBlkRec is one entry of the map-difference list at
 * 0x0bb8 -- a column, a row and a tile number. */

struct SaveBlkRec
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
};
struct SaveBlk
{
    /* 0x0000 */ u16 unk0000;
    /* 0x0002 */ u16 unk0002;
    /* 0x0004 */ struct Unk802C57C unk0004;
    /* 0x0008 */ struct Unit unk0008;
    /* 0x0014 */ u8 unk0014[0x140 - 0x14];
    /* 0x0140 */ u8 unk0140[0x48];
    /* 0x0188 */ struct Unit unk0188[4 * 51];
    /* 0x0b18 */ int unk0b18[4];
    /* 0x0b28 */ u8 filler_0b28[0xb98 - 0xb28];
    /* 0x0b98 */ struct Unk03002F08 unk0b98;
    /* 0x0ba0 */ void (*unk0ba0)(void);
    /* 0x0ba4 */ bool8 (*unk0ba4)(void);
    /* 0x0ba8 */ u32 unk0ba8;
    /* 0x0bac */ u8 unk0bac;
    /* 0x0bad */ u8 filler_0bad[1];
    /* 0x0bae */ u16 unk0bae;
    /* 0x0bb0 */ u16 unk0bb0;
    /* 0x0bb2 */ u16 unk0bb2;
    /* 0x0bb4 */ u16 unk0bb4;
    /* 0x0bb6 */ u16 unk0bb6;
    /* 0x0bb8 */ struct SaveBlkRec unk0bb8[(0xd28 - 0xbb8) / 4];
    /* 0x0d28 */ struct Unk02028360 unk0d28[16];
    /* 0x0da8 */ u8 unk0da8[4];
};

/*
 * sub_08016F38 -- write the running game state into the save block.
 *
 * The block is the RAM area at gUnknown_02000000, laid out as struct SaveBlk
 * above. sub_08017208 (src/decomp/c_08017208.c) reads back everything written
 * here.
 *
 *   1. Record the flag `a1` in .unk0bac, then copy the loose globals the block
 *      holds: four scalars, the play state gPlaySt (0x48 bytes), the unit at
 *      gUnknown_03004490, four ints from gUnknown_030033F4, one struct and the
 *      two callbacks at .unk0ba0 and .unk0ba4.
 *   2. Copy the map's width, height, scroll position and .unk10.
 *   3. Unless the map ID is one of 0xb4..0xbf, store the map as a difference
 *      from its pristine form: sub_080247A4 loads the map's original tiles at
 *      gUnknown_03003F68, and every live tile that differs from the original
 *      is appended to .unk0bb8 as a (column, row, tile) record. A record whose
 *      tile is 0xffff terminates the list, and sub_0802481C releases the
 *      loaded original.
 *   4. Copy five 0x3c-byte records from gUnknown_02023284 to offset 0x14, the
 *      four armies' 51 units each out of gUnknown_02022684 (which leaves 64
 *      units of room per army), and sixteen gUnknown_02028360 entries.
 *   5. sub_08045700 fills in the last four bytes.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The tile comparison splits the byte offset out as its own statement
 *     (`off = idx * 2;`) while the live map stays an array reference. Folding
 *     the multiply into the comparison swaps which register holds the index
 *     and which holds the constants.
 *   - `asm volatile ("" : "=r" (k));` emits no instruction; it makes the
 *     compiler choose the original's register for the copy just below it.
 *     Leave it in. It also stops tools/permute.py running on this function.
 */
void sub_08016F38(u8 a1)
{
  struct SaveBlk *p = (struct SaveBlk *) gUnknown_02000000;
  s16 i;
  s16 j;
  s16 k;
  s16 x;
  s16 y;
  int idx;
  int off;
  if (a1)
  {
    p->unk0bac = 1;
  }
  else
  {
    p->unk0bac = 0;
  }
  p->unk0ba8 = gUnknown_03001FD4;
  p->unk0004 = gUnknown_030033E4;
  p->unk0000 = gUnknown_03004080;
  asm volatile ("" : "=r" (k));
  p->unk0002 = gUnknown_030033EC;
  sub_0808B6E8(p->unk0140, &gPlaySt, 0x48);
  p->unk0008 = *((struct Unit *) gUnknown_03004490);
  for (i = 0; i < 4; i++)
  {
    p->unk0b18[i] = gUnknown_030033F4[i];
  }

  p->unk0b98 = gUnknown_03002F08;
  p->unk0ba0 = gUnknown_03002F20;
  p->unk0ba4 = gUnknown_03001FF0;
  p->unk0bae = gMap->width;
  p->unk0bb0 = gMap->height;
  p->unk0bb2 = gMap->scrollX;
  p->unk0bb4 = gMap->scrollY;
  p->unk0bb6 = gMap->unk10;
  if ((gPlaySt.mapID < 0xb4) || (gPlaySt.mapID > 0xbf))
  {
    sub_080247A4(gPlaySt.mapID);
    k = 0;
    for (y = 0; y < gMap->width; y++)
    {
      for (x = 0; x < gMap->height; x++)
      {
        idx = gMap->rowOffset[x] + y;
        off = idx * 2;
        if (gMap->tile[idx] != *(u16 *)((u8 *)gUnknown_03003F68 + off + 2))
        {
          p->unk0bb8[k].unk02 = gMap->tile[idx];
          p->unk0bb8[k].unk00 = y;
          p->unk0bb8[k].unk01 = x;
          k++;
        }
      }

    }

    p->unk0bb8[k].unk02 = 0xffff;
    sub_0802481C();
  }
  for (i = 0; i < 5; i++)
  {
    sub_0808B6E8((((u8 *) p) + (i * 0x3c)) + 0x14, &gUnknown_02023284[i * 0x3c], 0x3c);
  }

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 51; j++)
    {
      p->unk0188[(i * 51) + j] = gUnknown_02022684[(i * 64) + j];
    }

  }

  for (i = 0; i < 16; i++)
  {
    p->unk0d28[i] = gUnknown_02028360[i];
  }

  sub_08045700(p->unk0da8);
}
