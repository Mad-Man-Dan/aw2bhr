#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801914C.
 * sub_0801914C @ 0x0801914C
 */

/*
 * sub_0801914C -- script command: repaint a palette from the current node.
 *
 * Skipped while gUnknown_03002514 is 1. The node's .unk08 is a palette number,
 * standing in as 5 when it is 0; sub_0802D5CC gets that number less one, and
 * gUnknown_03002F08.unk00. The cursor then steps one node on and TRUE comes
 * back, so the dispatcher runs the next command in the same frame.
 * sub_080188D0 next door does the same repaint, but reaches its stand-in value
 * with two calls instead of a preset.
 *
 * Why the C looks odd: `...unk04->unk08 += 0;` adds nothing to the member, but
 * it is needed: it makes the compiler read the member where the original
 * does. Without it the output no longer matches. The stand-in value is a
 * preset `v = 5` that the test overwrites, not a `?:`; with a `?:` the compiler
 * folds the `- 1` into both arms and loses the shared subtraction.
 */
bool8 sub_0801914C(s16 a)
{
  int w;
  int v;
  if (gUnknown_03002514 != 1)
  {
    w = (s16) gUnknown_0200C528[a].unk04->unk08;
    v = 5;
    if (w != 0)
    {
      v = w;
    }
    gUnknown_0200C528[a].unk04->unk08 += 0;
    sub_0802D5CC(v - 1, gUnknown_03002F08.unk00);
  }
  gUnknown_0200C528[a].unk04++;
  return 1;
}
