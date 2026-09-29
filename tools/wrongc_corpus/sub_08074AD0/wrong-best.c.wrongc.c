#include "global.h"

#include "proc.h"

extern s8 *gUnknown_081CC4D4;

struct Unk8074AD0Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 filler_34[0x38 - 0x34];
    /* 0x38 */ s16 unk38;
    /* 0x3a */ s16 unk3a;
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 unk40;
};

void sub_08074AD0(struct Unk8074AD0Proc *proc)
{
    s8 step;
    int dx;
    int dy;
    int dist;
    int i;

    step = 1;

    dx = proc->unk2c - proc->unk30;
    if (dx < 0)
        dx = -dx;

    dy = proc->unk2e - proc->unk32;
    if (dy < 0)
        dy = -dy;

    if (dx > dy)
    {
        proc->unk40 = 1;
        proc->unk38 = dx;
    }
    else
    {
        proc->unk40 = 0;
        proc->unk38 = dy;
    }

    dist = proc->unk38;
    i = 0;

    while (dist - (step >> 1) >= 0)
    {
        dist -= step >> 1;
        gUnknown_081CC4D4[i] = step >> 1;

        if (step <= 0xf)
            step = step + 1;

        i++;
    }

    gUnknown_081CC4D4[i] = dist;
    proc->unk3c = i;
    proc->unk3a = proc->unk38;
}
