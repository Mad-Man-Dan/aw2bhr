#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025EF0.
 * sub_08025EF0 @ 0x08025EF0, CanTransportCarry @ 0x08025F74, sub_08025FC0 @ 0x08025FC0
 */

bool8 CanTransportCarryUnitId(int a1, int a2)
{
    u8 *t;
    u8 *u;

    if ((a1 & 0xc0) != (a2 & 0xc0))
        return FALSE;

    t = gUnknown_085D5ABC[gUnits[a1].type].transportTable;

    if (t == NULL)
        return FALSE;

    u = t + 1;

    if (u[gUnits[a2].type] == 0)
        return FALSE;

    if (t[0] == 1 && gUnits[a1].unk07 != 0)
        return FALSE;

    if (t[0] == 2 && gUnits[a1].unk08 != 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_08025EF0\n.thumb_set sub_08025EF0, CanTransportCarryUnitId\n");

bool8 CanTransportCarry(struct Unit *a1, u8 a2)
{
    u8 *t;
    u8 *u;

    t = gUnknown_085D5ABC[a1->type].transportTable;

    if (t == NULL)
        return FALSE;

    u = t + 1;

    if (u[a2] == 0)
        return FALSE;

    if (t[0] == 2 && a1->unk08 != 0)
        return FALSE;

    if (t[0] == 1 && a1->unk07 != 0)
        return FALSE;

    return TRUE;
}

asm(".global sub_08025F74\n.thumb_set sub_08025F74, CanTransportCarry\n");

bool8 CanJoinUnits(struct Unit *a1, struct Unit *a2)
{
    if (a1->type != a2->type)
        return FALSE;

    if (((a1 - gUnits) & 0xc0) != ((a2 - gUnits) & 0xc0))
        return FALSE;

    if (a1->unk07 != 0)
        return FALSE;

    if (a2->unk07 != 0)
        return FALSE;

    if (a2->hp != 0 && Div(a2->hp - 1, 10) == 9)
        return FALSE;

    return TRUE;
}
asm(".global sub_08025FC0\n.thumb_set sub_08025FC0, CanJoinUnits\n");
