#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025D60.
 * sub_08025D60 @ 0x08025D60, BuyUnit @ 0x08025E08
 */

void DestroyUnitAndCargo(int a1)
{
    struct Unit *p;
    struct Unit *q;
    int v;

    p = &gUnits[a1];

    NoteFlaggedUnitRemoved(p);
    IncrementPlayerUnitsLost((a1 >> 6) + 1);

    if (p->unk07 != 0)
    {
        q = &gUnits[p->unk07];

        if (q->hp != 0)
            v = Div(q->hp - 1, 10) + 1;
        else
            v = 0;

        sub_08025B24(q, v);
        DestroyUnitAndCargo(p->unk07);
    }

    if (p->unk08 != 0)
    {
        q = &gUnits[p->unk08];

        if (q->hp != 0)
            v = Div(q->hp - 1, 10) + 1;
        else
            v = 0;

        sub_08025B24(q, v);
        DestroyUnitAndCargo(p->unk08);
    }

    p->type = 0;
}
asm(".global sub_08025D60\n.thumb_set sub_08025D60, DestroyUnitAndCargo\n");

/* Named per Xenesis's AW2 Subroutine List: "Costs for unit bought". The old
 * BuyUnit symbol is kept as a linker alias below so every other unit
 * keeps resolving it unchanged. */
void *BuyUnit(int a1, int a2, int a3)
{
    int cost;
    void *r;

    cost = GetUnitCostWithCoBonus(gUnknown_030033EC, a3) * 10;

    if (gPlayers[gUnknown_030033EC].funds < cost)
        return NULL;

    r = CreateExhaustedUnitAt(a1, a2, a3);

    if (r == NULL)
        return NULL;

    SubtractPlayerFunds(gUnknown_030033EC, cost);

    return r;
}

asm(".global sub_08025E08\n.thumb_set sub_08025E08, BuyUnit\n");
