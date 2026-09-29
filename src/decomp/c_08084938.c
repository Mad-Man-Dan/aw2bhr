#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084938.
 * sub_08084938 @ 0x08084938
 */

/* AreAllCosSelectable's shape run twice back to back over two disjoint id ranges
 * against HasMapBeenPlayed's bitset. The two loops are not symmetric in the
 * assembly -- the first reaches the shared `movs r0, #0` with `beq` and the
 * second skips it with `bne` -- because jump.c cross-jumped the two identical
 * `return FALSE;` tails onto the second one, which is where the block happened
 * to land. Nothing in the source distinguishes them.
 *
 * The `lsls r0, r0, #0x18` at both call sites is what re-typed HasMapBeenPlayed's
 * return from `int` to `u8`; see include/unknown-functions.h. */

bool8 HaveAllMapsBeenPlayed(void)
{
    int i;

    for (i = 1; i <= 0x6b; i++)
        if (!HasMapBeenPlayed(i))
            return FALSE;
    for (i = 0x6c; i <= 0x89; i++)
        if (!HasMapBeenPlayed(i))
            return FALSE;
    return TRUE;
}
asm(".global sub_08084938\n.thumb_set sub_08084938, HaveAllMapsBeenPlayed\n");
