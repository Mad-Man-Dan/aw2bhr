#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A168.
 * sub_0801A168 @ 0x0801A168
 */

/*
 * CloseTopMenu -- stop the option-list script.
 *
 * sub_0801537C ends whichever gUnknown_03001470 slot is running
 * gUnknown_0848A42C -- the script CreateMenu starts -- and returns that slot
 * index, or -1. This function passes the value straight on.
 *
 * Both this function and sub_0801537C return int and not s8. The compiler
 * re-narrows a narrow-returning callee's result at the call site, which adds an
 * instruction the original does not have. The argument for the int is written up
 * on sub_0801537C's declaration in include/unknown-functions.h.
 */
int CloseTopMenu(void)
{
    return sub_0801537C(gUnknown_0848A42C);
}
asm(".global sub_0801A168\n.thumb_set sub_0801A168, CloseTopMenu\n");
