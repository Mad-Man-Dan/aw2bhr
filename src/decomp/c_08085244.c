#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085244.
 * sub_08085244 @ 0x08085244
 */

void CoInfoScreen_DrawUnitBonusPage(s16 *p)
{
    DrawOamObject(0x67, 0xBD, 0x98, 0, 1);
    CoInfoScreen_DrawUnitBonusGrid(p, p[0x33]);
    CoInfoScreen_DrawArmyIcons();
    sub_08043B60(0x78, 8, 0x82AC, 3);
    PutCoMinimugSprite(0x10D0, 0x18, 0x62B8, 5);
}
asm(".global sub_08085244\n.thumb_set sub_08085244, CoInfoScreen_DrawUnitBonusPage\n");
