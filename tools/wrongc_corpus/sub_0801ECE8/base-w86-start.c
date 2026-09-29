#include "global.h"

int sub_0801ECE8(s16 a1, int a2, int a3, int a4, long long a5, volatile int a6)
{
    long long v;
    int t;

    t = a6;
    gUnknown_0200ED20[gUnknown_03002510].unk00 = a2;
    gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
    gUnknown_0200ED20[gUnknown_03002510].unk04 = a4;
    gUnknown_0200ED20[gUnknown_03002510].unk08 = 0;
    v = a5 & ~0x2000;
    v &= ~0x1000;
    gUnknown_0200ED20[gUnknown_03002510].unk0c = v;
    gUnknown_0200ED20[gUnknown_03002510].unk0a = t;

    if (sub_0801A718(&gUnknown_0200ED20[gUnknown_03002510], a1) == -1)
        return 1;

    gUnknown_03002510++;
    return 0;
}
