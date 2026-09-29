#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044534.
 * sub_08044534 @ 0x08044534
 */

void CopActivateStandardBoost(void)
{
    gPlayers[gUnknown_030033EC].tempFirepower = 0;
    gPlayers[gUnknown_030033EC].tempDefense = 10;
}
asm(".global sub_08044534\n.thumb_set sub_08044534, CopActivateStandardBoost\n");
