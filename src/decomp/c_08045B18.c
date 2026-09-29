#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045B18.
 * sub_08045B18 @ 0x08045B18
 */

int MapEventCond_Army1CoPowerUsedOnce(void)
{
    if (GetCoPowerUseCount(1) == 1)
        return 1;

    return 0;
}
asm(".global sub_08045B18\n.thumb_set sub_08045B18, MapEventCond_Army1CoPowerUsedOnce\n");
