#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A8C0.
 * sub_0805A8C0 @ 0x0805A8C0
 */

int AiIsNearEnemyHq(u16 x, u16 y)
{
    int i;
    int dx;
    u16 dy;

    for (i = 0; i < 4; i++)
    {
        if ((gPlayers[gUnknown_030033EC].unk2c >> i) & 1)
        {
            if (DoesArmyHaveHq(i + 1))
            {
                dx = x - gPlayers[i + 1].hqX + 2;
                dy = y - gPlayers[i + 1].hqY + 2;

                if ((u16)dx <= 4 && (s16)dy >= 0 && (s16)dy <= 4)
                    return 1;
            }
        }
    }

    return 0;
}
asm(".global sub_0805A8C0\n.thumb_set sub_0805A8C0, AiIsNearEnemyHq\n");
