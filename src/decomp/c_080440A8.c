#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080440A8.
 * SpendCoPowerCharge @ 0x080440A8, AddCoPowerCharge @ 0x080440E0
 */

void SpendCoPowerCharge(int a1, int a2)
{
    int v;

    switch (a2) {
    default:
    case 1:
        v = GetCoPowerCost(a1);
        break;
    case 2:
        v = GetSuperCoPowerCost(a1);
        break;
    }
    v = GetCoPowerCharge(a1) - v;
    if (v < 0)
        v = 0;
    SetCoPowerCharge(a1, v);
}

asm(".global sub_080440A8\n.thumb_set sub_080440A8, SpendCoPowerCharge\n");

void AddCoPowerCharge(int a1, int a2)
{
    int v;

    if (gPlaySt.coPowersEnabled == 0)
        return;
    if (IsCoPowerActive(a1))
        return;
    v = gPlayers[a1].coCharge;
    if (v < 0)
        return;
    if (v + a2 > GetSuperCoPowerCost(a1))
        gPlayers[a1].coCharge = GetSuperCoPowerCost(a1);
    else
        gPlayers[a1].coCharge = gPlayers[a1].coCharge + a2;
}
asm(".global sub_080440E0\n.thumb_set sub_080440E0, AddCoPowerCharge\n");
