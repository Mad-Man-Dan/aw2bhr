#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804419C.
 * GetCoPowerStarCost @ 0x0804419C, GetCoPowerCost @ 0x080441D4, GetSuperCoPowerCost @ 0x08044208, IsCoPowerReady @ 0x0804423C, IsSuperCoPowerReady @ 0x08044280, IsCoPowerAvailable @ 0x080442AC
 */

int GetCoPowerStarCost(int a1)
{
    u16 pct;

    if (GetCoPowerUseCount(a1) > 9)
        pct = 200;
    else
        pct = GetCoPowerUseCount(a1) * 20 + 100;
    return Div(pct * 9000, 100);
}

asm(".global sub_0804419C\n.thumb_set sub_0804419C, GetCoPowerStarCost\n");

int GetCoPowerCost(int a1)
{
    return GetCoPowerStarCost(a1) * gUnknown_085D3DD0[gPlayers[a1].co].coPowerStars;
}

asm(".global sub_080441D4\n.thumb_set sub_080441D4, GetCoPowerCost\n");

int GetSuperCoPowerCost(int a1)
{
    return GetCoPowerStarCost(a1) * gUnknown_085D3DD0[gPlayers[a1].co].superCoPowerStars;
}

asm(".global sub_08044208\n.thumb_set sub_08044208, GetSuperCoPowerCost\n");

u8 IsCoPowerReady(int a1)
{
    int funds;

    if (IsCoPowerActive(a1))
        return 0;
    if (GetCoPowerCost(a1) == 0)
        return IsSuperCoPowerReady(a1);
    funds = GetCoPowerCharge(a1);
    if (funds >= GetCoPowerCost(a1))
        return 1;
    return 0;
}

asm(".global sub_0804423C\n.thumb_set sub_0804423C, IsCoPowerReady\n");

int IsSuperCoPowerReady(int a1)
{
    int funds;

    if (IsCoPowerActive(a1))
        return 0;
    funds = GetCoPowerCharge(a1);
    if (funds >= GetSuperCoPowerCost(a1))
        return 1;
    return 0;
}
asm(".global sub_08044280\n.thumb_set sub_08044280, IsSuperCoPowerReady\n");

int IsCoPowerAvailable(int a1)
{
    int funds;

    if (IsCoPowerActive(a1))
        return 0;
    if (GetCoPowerCost(a1) == 0)
        return 0;
    funds = GetCoPowerCharge(a1);
    if (funds >= GetCoPowerCost(a1))
        return 1;
    return 0;
}

asm(".global sub_080442AC\n.thumb_set sub_080442AC, IsCoPowerAvailable\n");
