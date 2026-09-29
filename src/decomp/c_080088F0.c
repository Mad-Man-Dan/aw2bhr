#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080088F0.
 * sub_080088F0 @ 0x080088F0
 */

void DesignRoomCountArmyUnits(void)
{
    gActiveMap->army1UnitCount = CountArmyUnits(1);
    gActiveMap->army2UnitCount = CountArmyUnits(2);
    gActiveMap->army3UnitCount = CountArmyUnits(3);
    gActiveMap->army4UnitCount = CountArmyUnits(4);
}
asm(".global sub_080088F0\n.thumb_set sub_080088F0, DesignRoomCountArmyUnits\n");
