#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080564B8.
 * sub_080564B8 @ 0x080564B8
 */

/* Wave 52, W52-B.  MATCHED (permuter, 159 iterations, from a 95.5% hand draft).
 *
 * Two spellings carry the whole thing, both measured -- see the wave-52
 * chapters in docs/agbcc-codegen.md:
 *  - The fill loops MUST use the flat `a2 * 0x6c + i * 2 + (char *)symbol`
 *    form, not `gUnknown_02029816[a2][i]`.  The subscript form creates the
 *    symbol's pseudo FIRST, so LICM emits its `ldr` before the `muls` in both
 *    preheaders; the ROM has it after.  Worth 12 bytes over two preheaders.
 *  - The `+=` target keeps the MEMBER form.  gUnknown_02029816 IS
 *    gUnknown_02029808 + 0xe (wave 48, W48-G), but the ROM carries a second
 *    pool word for gUnknown_02029808 and derives the row with a runtime
 *    `adds r2,#0xe` -- cse.c's use_related_value.  Writing that flat as
 *    `(char *)gUnknown_02029808 + 0xe` FOLDS the constant into the pool word
 *    (`.word gUnknown_02029808+0xe`) and is wrong.
 *  - `new_var` is the permuter's contribution: binding `(u16)(i - 1)` to its
 *    own local is what moves the tail's `ldr` off the top of the block.
 *
 * NOT a twin of sub_08056638 despite adjacency and a shared exemplar: 64
 * instructions against 70, wholly different bodies.  What they share is the
 * 0x6c record, already fully described in include/unknown-globals.h.
 */

void SetDeathBatchSizes(u16 a1, u16 a2, u16 a3)
{
  u16 new_var;
  u16 i;
  if (a3 > a1)
  {
    for (i = 0; i < a1; i++)
    {
      *((u16 *) (((a2 * 0x6c) + (i * 2)) + ((char *) gUnknown_02029816))) = 1;
    }

    new_var = (u16) (i - 1);
    gUnknown_02029808[a2].unk0e[new_var] += a3 - a1;
  }
  else
  {
    for (i = 0; i < a3; i++)
    {
      *((u16 *) (((a2 * 0x6c) + (i * 2)) + ((char *) gUnknown_02029816))) = 1;
    }

  }
}
asm(".global sub_080564B8\n.thumb_set sub_080564B8, SetDeathBatchSizes\n");
