#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E3E8.
 * sub_0801E3E8 @ 0x0801E3E8
 */

/* MATCHED in wave 42 (W42-G). Two changes took the inherited wave-40 draft from
 * 92.0% to a match; the second came from decomp-permuter, which that draft's
 * note asked for and which had never been run.
 *
 * 1. `t = t & 0xFF;` IS ITS OWN STATEMENT (92.0% -> 97.5%). The ROM computes
 *    the trailing mask BEFORE the `ldr` of 0xffffff00 yet still makes the
 *    `orrs` destination the `d & ~0xFF` register, and the wave-40 note
 *    concluded no operand order gives both -- correct, because it is not an
 *    operand-order question. Masking `t` in place as a separate statement emits
 *    it first (two-address `ands r2,r0` on t's own register) and leaves the OR
 *    with `d & ~0xFF` as its FIRST operand, which is what picks the
 *    destination. Note this is the opposite of the sibling PutObjectListToLoOam, where
 *    the whole attribute word is one expression with no local at all -- the two
 *    functions genuinely differ, so do not carry either rule across.
 * 2. The last 5 bytes were a pure allocation tie: the ROM gives the
 *    callee-saved r5 to the gUnknown_03002520 pool word and coalesces `dst`
 *    with IT, while the draft gave r5 to `a`. The permuter found it in ~2,000
 *    iterations, and the fix is the split below -- `dst = gUnknown_03002520;`
 *    then `dst = dst + (a * 4);` as two statements, which creates the base
 *    pseudo before `a`'s use. DO NOT re-join these two lines.
 *    Ruled out by hand first, each byte-identical to the unsplit form: binding
 *    the base to a SEPARATE local (`base = gUnknown_03002520; dst = base + a*4;`
 *    -- copy-propagated away), and `&gUnknown_03002520[a * 4]`.
 */
int PutObjectListToOamShadow(int a, int b, int c, u16 *data, int idx)
{
  u16 *dst;
  int n;
  int i;
  int d;
  int t;
  n = *(data++);
  if ((a + n) > 0x80)
  {
    return 1;
  }
  dst = gUnknown_03002520;
  dst = dst + (a * 4);
  for (i = 0; i < n; i++)
  {
    d = *(data++);
    t = (d & 0xFF) + c;
    t = t & 0xFF;
    *(dst++) = (d & (~0xFF)) | t;
    if (d & 0x100)
    {
      d = *(data++);
      d = (d & 0xC1FF) | gUnknown_0808F0B8[idx];
    }
    else
    {
      d = *(data++);
    }
    t = (d & 0x1FF) + b;
    *(dst++) = (d & (~0x1FF)) | (t & 0x1FF);
    *dst = *(data++);
    dst = dst + 2;
  }

  return 0;
}
asm(".global sub_0801E3E8\n.thumb_set sub_0801E3E8, PutObjectListToOamShadow\n");
