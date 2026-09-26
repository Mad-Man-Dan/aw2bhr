#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080785FC.
 * ClearArmyCount @ 0x080785FC
 */

void ClearArmyCount(void)
{
    gUnknown_03005944 = 0;
}

asm(".global sub_080785FC\n.thumb_set sub_080785FC, ClearArmyCount\n");
