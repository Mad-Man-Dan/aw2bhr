#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802E010.
 * sub_0802E010 @ 0x0802E010, sub_0802E130 @ 0x0802E130
 */

/*
 * sub_0802E010 -- lay out a picture's tiles in the BG1 tilemap.
 *
 * The picture is already in tile memory as a run of consecutive tiles; this
 * builds the tilemap that shows it. gUnknown_03003F68 points at the picture's
 * size: byte 0 is the width and byte 1 the height, in half-tile units (the
 * loops draw width/2 columns and height/2 rows).
 *
 *   1. Clear all 0x400 entries (32 x 32) of the BG1 tilemap buffer.
 *   2. Left part, up to 16 columns: the entry at (row, col) gets tile
 *      row * 16 + col, with palette 5 (0x5000 is palette 5 << 12).
 *   3. If the width is 32 or more, columns 16 and up take their tiles from
 *      the next block: tile 0x100 + row * 16 + col.
 *   4. Flag BG1 for copying to VRAM (BG_EnableSyncBG1 is BG_EnableSyncByMask(2)).
 *
 * sub_0802E130 below is the same routine for BG0.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `half` is assigned three times, the last one as `>> (half = 1)` inside
 *     a loop condition. With fewer assignments the compiler drops the (u16)
 *     cast on the second loop's bound, or reuses half's register for the
 *     subtraction, and the output no longer matches.
 *   - Leave the `row` and `k` temporaries and the `for` headers as written.
 *     They were settled by testing; tidying them loses the match.
 */

void sub_0802E010(void)
{
  u8 n;
  int half;
  u16 *row;
  u16 i;
  u16 j;
  n = ((u8 *) gUnknown_03003F68)[0];
  if (n > 0x20)
  {
    n = 0x20;
  }
  for (i = 0, half = n >> 1; i <= 0x3FF; i++)
  {
    gBG1TilemapBuffer[i] = 0;
  }

  for (i = 0; i < ((u16) half); i++)
  {
    for (j = 0; j < ((((u8 *) gUnknown_03003F68)[1] >> 1) << 4); j += 0x10)
    {
      gBG1TilemapBuffer[(j * 2) + i] = (j + i) + 0x5000;
    }

  }

  if (((u8 *) gUnknown_03003F68)[0] > 0x1F)
  {
    for (i = 0, half = (((u8 *) gUnknown_03003F68)[0] - n); i < ((((u8 *) gUnknown_03003F68)[0] - n) >> (half = 1)); i++)
    {
      for (j = 0; j < ((((u8 *) gUnknown_03003F68)[1] >> 1) << 4); j += 0x10)
      {
        int k;
        k = j * 2;
        row = gBG1TilemapBuffer;
        row[(i + k) + 0x10] = (i + (j + 0x100)) + 0x5000;
      }

    }
  }
  BG_EnableSyncBG1();
}

/*
 * sub_0802E130 -- same as sub_0802E010, but for BG0: fills gBG0TilemapBuffer
 * and flags BG0 for copying (BG_EnableSyncBG0 is BG_EnableSyncByMask(1)). The
 * notes above apply here unchanged.
 */

void sub_0802E130(void)
{
  u8 n;
  int half;
  u16 *row;
  u16 i;
  u16 j;
  n = ((u8 *) gUnknown_03003F68)[0];
  if (n > 0x20)
  {
    n = 0x20;
  }
  for (i = 0, half = n >> 1; i <= 0x3FF; i++)
  {
    gBG0TilemapBuffer[i] = 0;
  }

  for (i = 0; i < ((u16) half); i++)
  {
    for (j = 0; j < ((((u8 *) gUnknown_03003F68)[1] >> 1) << 4); j += 0x10)
    {
      gBG0TilemapBuffer[(j * 2) + i] = (j + i) + 0x5000;
    }

  }

  if (((u8 *) gUnknown_03003F68)[0] > 0x1F)
  {
    for (i = 0, half = (((u8 *) gUnknown_03003F68)[0] - n); i < ((((u8 *) gUnknown_03003F68)[0] - n) >> (half = 1)); i++)
    {
      for (j = 0; j < ((((u8 *) gUnknown_03003F68)[1] >> 1) << 4); j += 0x10)
      {
        int k;
        k = j * 2;
        row = gBG0TilemapBuffer;
        row[(i + k) + 0x10] = (i + (j + 0x100)) + 0x5000;
      }

    }
  }
  BG_EnableSyncBG0();
}
