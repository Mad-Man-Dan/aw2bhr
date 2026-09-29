#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029978.
 * sub_08029978 @ 0x08029978, sub_08029A48 @ 0x08029A48
 */

int ResupplyUnitAmmo(struct Unit *p, u8 a2)
{
    int acc;
    u16 v;

    acc = 0;
    v = p->hp != 0
        ? (Div(p->hp - 1, 10) + 1) * gUnknown_085D5ABC[p->type].unk08
        : 0;

    while (p->ammo != gUnknown_085D5ABC[p->type].maxAmmo)
    {
        if (a2)
        {
            if (gPlayers[gUnknown_030033EC].funds < v)
                break;

            SubtractPlayerFunds(gUnknown_030033EC, v);
        }

        acc += v;
        p->ammo++;
    }

    return acc;
}
asm(".global sub_08029978\n.thumb_set sub_08029978, ResupplyUnitAmmo\n");

int ResupplyUnitFuel(struct Unit *p, u8 a2)
{
    int acc;
    u16 v;

    acc = 0;
    v = p->hp != 0 ? Div(p->hp - 1, 10) + 1 : 0;

    while (p->fuel != gUnknown_085D5ABC[p->type].maxFuel)
    {
        if (a2)
        {
            if (gPlayers[gUnknown_030033EC].funds < v)
                break;

            SubtractPlayerFunds(gUnknown_030033EC, v);
        }

        acc += v;
        p->fuel++;
    }

    return acc;
}
asm(".global sub_08029A48\n.thumb_set sub_08029A48, ResupplyUnitFuel\n");
