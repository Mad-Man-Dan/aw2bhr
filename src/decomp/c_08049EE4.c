#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049EE4.
 * sub_08049EE4 @ 0x08049EE4
 */

#include "hardware.h"

void DefeatFlow_HideDisplay(void)
{
    SetDispEnable(0, 0, 0, 0, 0);
}
asm(".global sub_08049EE4\n.thumb_set sub_08049EE4, DefeatFlow_HideDisplay\n");
