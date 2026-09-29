#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059C60.
 * sub_08059C60 @ 0x08059C60
 */

struct Unk59C60Cell
{
    /* 0x00 */ u8 x;
    /* 0x01 */ u8 y;
    /* 0x02 */ s16 v;
};

void AiListHuntTargets(void *a1)
{
    struct Unk59C60Cell *out;
    struct Unit *p;
    int i;
    int j;
    int k;
    int x;
    int y;
    u16 z;

    out = a1;
    k = (gUnknown_03003F38 >> 6) + 1;
    for (i = 0; i < 4; i++)
    {
        if (!((gPlayers[gUnknown_030033EC].unk2c >> i) & 1))
            continue;
        for (j = i * 64; j < i * 64 + 64; j++)
        {
            p = &gUnits[j];
            if (p->type == 0)
                continue;
            if ((s8)gUnknown_03003340[p->y][p->x] <= 0)
                continue;
            if (p->type == 0x18 && !IsUnitVisibleToCurrentTeam(j))
                continue;
            if (!IsCellVisibleToArmy(gUnknown_030033EC, p->x, p->y))
                continue;
            x = GetCoAdjustedBaseDamage(gPlayers[k].co, gPlayers[k].coMode,
                             gUnknown_030040D8->unk00, p->type, 0);
            y = GetCoAdjustedBaseDamage(gPlayers[k].co, gPlayers[k].coMode,
                             gUnknown_030040D8->unk00, p->type, 1);
            if (x < y)
                z = GetCoAdjustedBaseDamage(gPlayers[k].co, gPlayers[k].coMode,
                                 gUnknown_030040D8->unk00, p->type, 1);
            else
                z = GetCoAdjustedBaseDamage(gPlayers[k].co, gPlayers[k].coMode,
                                 gUnknown_030040D8->unk00, p->type, 0);
            if (z <= 0x31)
                continue;
            out->x = p->x;
            out->y = p->y;
            out->v = (s8)gUnknown_03003340[p->y][p->x];
            out++;
        }
    }
    out->v = 0xffff;
}
asm(".global sub_08059C60\n.thumb_set sub_08059C60, AiListHuntTargets\n");
