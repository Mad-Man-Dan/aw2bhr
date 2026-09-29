#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08062330.
 * sub_08062330 @ 0x08062330
 */

struct Unk62330Cell
{
    /* 00 */ u8 x;
    /* 01 */ u8 y;
    /* 02 */ s16 v;
};

void AiSeedInterestCandidates(void)
{
    struct PlayerStruct *base;
    struct PlayerStruct *u;
    struct Unk62330Cell *out;
    int army;
    short cx;
    int cy;
    int i;
    int dx;
    int dy;

    army = gUnknown_030033EC;
    base = gPlayers;
    u = (struct PlayerStruct *)(army * 0x3c + (int)base);
    cx = u->hqX & 0x7f;
    cy = u->hqY & 0x7f;
    out = (struct Unk62330Cell *)gUnknown_03003F20;

    i = 0;
    while (gUnknown_084995A0[i].unk00 != 0xff)
    {
        if (gUnknown_084995A0[i].unk00 != 0x13)
        {
            out->x = gUnknown_084995A0[i].unk01;
            out->y = gUnknown_084995A0[i].unk02;

            dx = out->x - cx;
            if (dx < 0)
                dx = -dx;

            dy = out->y - (u16)cy;
            if (dy < 0)
                dy = -dy;

            out->v = dx + dy;
            out++;
        }
        i++;
    }
    out->v = 0xFFFF;
}
asm(".global sub_08062330\n.thumb_set sub_08062330, AiSeedInterestCandidates\n");
