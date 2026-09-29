#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BE40.
 * sub_0803BE40 @ 0x0803BE40, sub_0803BE5C @ 0x0803BE5C
 */

#include "proc.h"

/* Three statements: CloseTopMenu (whose `int` result is discarded -- nothing
 * re-narrows it and `pop {r0}` makes this void), then start the
 * gUnknown_0849BC98 proc on tree 3 through sub_0803433C, then raise
 * gUnknown_03002F1C. That last flag is the same one DebugEntry_Init sets, so the
 * two are alternative entries into the same request.
 * `pop {r0}; bx r0` -> void. */

void sub_0803BE40(void)
{
    CloseTopMenu();
    sub_0803433C(PROC_TREE_3);
    gUnknown_03002F1C = 1;
}

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` behind it. Nothing about the signature is recoverable;
 * void(void) is the weakest model. See SoundMainLoopNoOp in
 * src/decomp/c_0803B3C8.c. */

void sub_0803BE5C(void)
{
}
