#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085168.
 * sub_08085168 @ 0x08085168
 */

void CoInfoScreen_DrawProfileAbilityPages(s16 *p)
{
    CoInfoScreen_DrawArmyIcons();
    sub_08043B60(0x20, 0x28, 0x82AC, 3);
    DrawOamObject(gPlayers[p[0x33]].teamColor + 0x3D, 8, 0x28, 0, 1);

    if (IsPlayerAliveAndActive(p[0x33]) != 0)
        DrawArmyCoPanel(0x98, 0x70, p[0x33]);
}
asm(".global sub_08085168\n.thumb_set sub_08085168, CoInfoScreen_DrawProfileAbilityPages\n");
