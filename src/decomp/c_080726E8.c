#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080726E8.
 * sub_080726E8 @ 0x080726E8
 */

/* Blits a w-by-h block of tilemap entries into a 32-wide BG tilemap at
 * (x, y), adding `base` to each entry and clipping to the 32x32 screen block.
 * `flip` mirrors the block horizontally, which also toggles each entry's
 * HFLIP bit (0x400).
 *
 * Why the C looks odd: the flipped loop steps `ix` through a separate `nx`
 * copy (`ix = nx`) rather than `ix++`. That stops the compiler treating `ix`
 * as a simple counter and strength-reducing the two addresses, which the
 * original does not do. */
void TmCopyRectClipped(u16 *map, int x, int y, u16 base, int w, int h, const u16 *src, u8 flip)
{
  const u16 *p = src;
  int ix;
  int iy;
  int nx;
  int ux;
  if (flip)
  {
    for (iy = 0; iy < h; iy++)
    {
      ix = 0;
      while (ix < w)
      {
        ux = x + ix;
        nx = ix + 1;
        if ((((unsigned) ux) < 0x20) && (((unsigned) (y + iy)) < 0x20))
        {
          *((map + (x + ix)) + ((y + iy) * 0x20)) = (*((p + (w - nx)) + (iy * 0x20)) + base) ^ 0x400;
        }
        ix = nx;
      }
    }
  }
  else
  {
    for (iy = 0; iy < h; iy++)
    {
      for (ix = 0; ix < w; ix++)
      {
        if ((((unsigned) (x + ix)) < 0x20) && (((unsigned) (y + iy)) < 0x20))
        {
          *((map + (x + ix)) + ((y + iy) * 0x20)) = (*((p + ix) + (iy * 0x20))) + base;
        }
      }
    }
  }
}
asm(".global sub_080726E8\n.thumb_set sub_080726E8, TmCopyRectClipped\n");
