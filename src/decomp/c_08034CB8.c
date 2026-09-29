#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034CB8.
 * sub_08034CB8 @ 0x08034CB8
 */

void sub_08034CB8(void)
{
    SetInfoBoxMode(0);
    RunMapEventsAfterTurnSupply();
    gUnknown_030032D8 = 0xb;
}
