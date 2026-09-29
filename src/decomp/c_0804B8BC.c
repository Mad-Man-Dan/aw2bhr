#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B8BC.
 * sub_0804B8BC @ 0x0804B8BC
 */

void InitPanelScrollDrivers(u16 a, u16 b)
{
    int c;
    u16 i;
    u8 pa;
    u8 pb;

    c = 0;
    pa = gUnknown_08555850[a].unk02;
    pb = gUnknown_08555850[b].unk02;

    for (i = 0; i < 2; i++)
    {
        gUnknown_02029690[i].unk0a = 0;
        gUnknown_02029690[i].unk0c = 0;
        gUnknown_02029690[i].unk00 = 1;
        gUnknown_02029690[i].unk08 = 0;
        gUnknown_02029690[i].unk0e = 0;
    }

    if (gUnknown_0300450C == 0)
        c = (pb + pa * 3) * 2;
    if (gUnknown_0300450C == 1)
        c = (u16)((pa + pb * 3) * 2 + 0x12);

    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004504.bit4)
            gUnknown_02029690[0].unk04 = gUnknown_085557C0[c];
        else
            gUnknown_02029690[0].unk04 = gUnknown_08555720;
        if (gUnknown_03004504.bit5)
            gUnknown_02029690[1].unk04 = gUnknown_085557C0[c + 1];
        else
            gUnknown_02029690[1].unk04 = gUnknown_08555720;
    }
    else
    {
        gUnknown_02029690[0].unk04 = gUnknown_085557C0[c];
        gUnknown_02029690[1].unk04 = gUnknown_085557C0[c + 1];
    }

    sub_0804BA64(0, pa);
    sub_0804BA64(1, pb);

    if (gUnknown_085D6A48[gUnknown_03004580[0][1]][0] == 9)
        gUnknown_02029690[0].unk04 = gUnknown_08555720;
    if (gUnknown_085D6A48[gUnknown_03004580[1][1]][0] == 9)
        gUnknown_02029690[1].unk04 = gUnknown_08555720;
    if (gUnknown_085D6A48[gUnknown_03004580[0][1]][0] == 0xa)
        gUnknown_02029690[0].unk04 = gUnknown_08555720;
    if (gUnknown_085D6A48[gUnknown_03004580[1][1]][0] == 0xa)
        gUnknown_02029690[1].unk04 = gUnknown_08555720;
    if (gUnknown_085D6A48[gUnknown_03004580[0][1]][0] == 0xe)
        gUnknown_02029690[0].unk04 = gUnknown_08555720;
    if (gUnknown_085D6A48[gUnknown_03004580[1][1]][0] == 0xe)
        gUnknown_02029690[1].unk04 = gUnknown_08555720;
}
asm(".global sub_0804B8BC\n.thumb_set sub_0804B8BC, InitPanelScrollDrivers\n");
