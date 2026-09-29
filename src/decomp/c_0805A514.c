#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805A514.
 * sub_0805A514 @ 0x0805A514, sub_0805A5E0 @ 0x0805A5E0
 */

struct Unk5A514Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void AiListEmbarkBoundUnits(struct Unk5A514Cell *out)
{
    struct Unit *u;
    u16 pos[2];
    int i;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        u = &gUnits[i];
        if (u->type == 0)
            continue;
        if ((u->unk09 & 0x38) != 0x18)
            continue;
        if (gUnknown_03004730[i & 0x3f] != 0)
            continue;
        if ((s8)gUnknown_03003340[u->y][u->x] == -1)
            continue;
        if (AiPickShoalParkBeside(u->x, u->y, pos) != 1)
            continue;
        out->x = u->x;
        out->y = u->y;
        out->v = (s8)gUnknown_03003340[u->y][u->x];
        out++;
    }

    out->v = 0xFFFF;
}
asm(".global sub_0805A514\n.thumb_set sub_0805A514, AiListEmbarkBoundUnits\n");

void AiPickSupplyWard(int *out)
{
    struct Unit *p;
    int i;
    u16 best;
    int t;
    int v;

    best = 0x7FFF;
    *out = -1;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        p = &gUnits[i];
        if (p->type == 0)
            continue;
        if (gUnknown_084995A8[p->type] == 0)
            continue;
        if (p->flags & 8)
            continue;
        if ((p->unk09 & 7) != 1)
            continue;
        if (&gUnits[i] == (struct Unit *)gUnknown_030040D8)
            continue;
        if ((s8)gUnknown_03003340[p->y][p->x] < 0)
            continue;

        t = (u16)((5 - gUnknown_08576828[p->type]) * 16);
        v = p->fuel + t;

        if ((s16)v < (s16)best)
        {
            *out = p - gUnits;
            best = v;
        }
    }
}
asm(".global sub_0805A5E0\n.thumb_set sub_0805A5E0, AiPickSupplyWard\n");
