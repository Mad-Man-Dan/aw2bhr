#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012A84.
 * sub_08012A84 @ 0x08012A84
 */

/* sub_08012A54's twin: the same two lines, but it CALLS EnableHBlankInterrupt directly
 * instead of registering it into the gUnknown_03002FA0 list. That is the whole
 * difference, and it is visible only as `bl EnableHBlankInterrupt` where the sibling
 * has `ldr r0,=EnableHBlankInterrupt; bl QueueVBlankCallback`. */
void sub_08012A84(void *handler)
{
    SetIRQHandler(1, handler);
    UpdateInterruptEnable(2, 2);
    EnableHBlankInterrupt();
}
