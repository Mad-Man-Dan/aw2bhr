#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080270B8.
 * sub_080270B8 @ 0x080270B8, sub_080270F0 @ 0x080270F0, sub_08027118 @ 0x08027118
 */

bool8 IsAnyArmyDefeatedByCaptureLimit(void)
{
    int i;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled != 0 && (gPlayers[i].unk13 & 0x20))
            return TRUE;
    }

    return FALSE;
}
asm(".global sub_080270B8\n.thumb_set sub_080270B8, IsAnyArmyDefeatedByCaptureLimit\n");

u8 GetFirstHumanArmy(void)
{
    int i;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled == 1)
            return i;
    }

    return 0;
}
asm(".global sub_080270F0\n.thumb_set sub_080270F0, GetFirstHumanArmy\n");

void ClearTeammateDefeats(void)
{
    int i;
    int j;

    for (i = 1; i <= 4; i++)
    {
        if (IsPlayerAliveAndActive(i))
        {
            for (j = 1; j <= 4; j++)
            {
                if (j != i && gPlayers[j].aiControlled != 0
                    && gPlayers[j].team == gPlayers[i].team)
                    gPlayers[j].defeated = 0;
            }
        }
    }
}
asm(".global sub_08027118\n.thumb_set sub_08027118, ClearTeammateDefeats\n");
