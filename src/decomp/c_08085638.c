#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085638.
 * sub_08085638 @ 0x08085638, sub_080856A0 @ 0x080856A0
 */

int GetMovementBonusIcon(int a1, int a2)
{
    switch (GetCoMovementBonus(gPlayers[a1].co, gPlayers[a1].coMode, a2))
    {
    case -1:
        return 0x95;
    case -2:
        return 0x96;
    case -3:
        return 0x97;
    default:
        return 0x98;
    case 2:
        return 0x99;
    case 3:
        return 0x9a;
    }
}
asm(".global sub_08085638\n.thumb_set sub_08085638, GetMovementBonusIcon\n");

int GetRangeBonusIcon(int a1, int a2)
{
    switch (GetCoRangeBonus(gPlayers[a1].co, gPlayers[a1].coMode, a2))
    {
    case -1:
        return 0x95;
    case -2:
        return 0x96;
    case -3:
        return 0x97;
    default:
        return 0x98;
    case 2:
        return 0x99;
    case 3:
        return 0x9a;
    }
}
asm(".global sub_080856A0\n.thumb_set sub_080856A0, GetRangeBonusIcon\n");
