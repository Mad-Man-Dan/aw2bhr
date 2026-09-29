#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080078D4.
 * sub_080078D4 @ 0x080078D4
 */

void DesignRoomSetUnitArmy(s8 a)
{
    gActiveMap->unitArmy = a;
}
asm(".global sub_080078D4\n.thumb_set sub_080078D4, DesignRoomSetUnitArmy\n");
