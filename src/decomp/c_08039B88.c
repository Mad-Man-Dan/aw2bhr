#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039B88.
 * sub_08039B88 @ 0x08039B88
 */

#include "proc.h"

/* LinkWaitMapTransferProcs's predicate, returned instead of acted on: 1 when either
 * script has a live instance. The `movs r0,#0` arm sits INLINE and the `movs
 * r0,#1` out of line, which is the `!= 0` spelling of the test -- writing it
 * as `Proc_Find(a) != 0 || Proc_Find(b) != 0` swaps the two blocks, because
 * do_jump rewrites an `== 0` comparison by exchanging its labels. */

int IsSparkleEffectActive(void)
{
    if (Proc_Find(gUnknown_0849D84C) == 0 && Proc_Find(gUnknown_0849D874) == 0)
        return 0;

    return 1;
}
asm(".global sub_08039B88\n.thumb_set sub_08039B88, IsSparkleEffectActive\n");
