#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044560.
 * sub_08044560 @ 0x08044560
 */

void CopActivatePowerOfMoney(void)
{
    gPlayers[gUnknown_030033EC].tempFirepower = Div(gPlayers[gUnknown_030033EC].funds, 300);
    gPlayers[gUnknown_030033EC].tempDefense = 10;
}
asm(".global sub_08044560\n.thumb_set sub_08044560, CopActivatePowerOfMoney\n");
