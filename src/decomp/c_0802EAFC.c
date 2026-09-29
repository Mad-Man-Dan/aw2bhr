#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EAFC.
 * sub_0802EAFC @ 0x0802EAFC
 */

void SioResetPollingState(void)
{
    gUnknown_03000570 = 0;
    gUnknown_03000564 &= ~3;
    gUnknown_03000568 = 0;
    gUnknown_03000574 = 0;
}
asm(".global sub_0802EAFC\n.thumb_set sub_0802EAFC, SioResetPollingState\n");
