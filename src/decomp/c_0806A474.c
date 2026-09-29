#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A474.
 * sub_0806A474 @ 0x0806A474
 */

#include "proc.h"

/* `pop {r1}` rather than `pop {r0}` says the result is live, and the body is
 * the boolean shape (`cmp; beq; movs r0,#1`) rather than the two-constant
 * `if`/`return` pair, which would have materialised the zero as well.
 * Same predicate as IsMeteorImpactRunning over gUnknown_0858168C. */
int IsIntroRunning(void)
{
    return Proc_Find(ProcScr_IntroT3) != 0;
}
asm(".global sub_0806A474\n.thumb_set sub_0806A474, IsIntroRunning\n");
