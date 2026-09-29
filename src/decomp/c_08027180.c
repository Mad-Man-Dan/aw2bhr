#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027180.
 * sub_08027180 @ 0x08027180
 */

void ResetPlayerUnitsLost(int a1)
{
    gPlayers[a1].unitsLost = 0;
}
asm(".global sub_08027180\n.thumb_set sub_08027180, ResetPlayerUnitsLost\n");
