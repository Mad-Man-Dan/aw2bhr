#include "global.h"

u8 sub_080611D8(void *arg)
{
    u16 *out;
    u16 none;
    int i;
    int best;
    u8 t;
    u8 r;

    out = arg;
    i = 0;
    best = 0;
    none = 9999;
    out[0] = none;

    if (gUnknown_030046C0.unk07 != 5)
    {
        t = gUnknown_085D5ABC[gUnknown_030046C0.unk06].deployLocation;

        if ((gUnknown_030046C0.unk06 == 20 && sub_08061308(t, 3, out) == 1)
            || (gUnknown_030046C0.unk06 == 23 && sub_08061308(t, 4, out) == 1)
            || sub_08061308(t, 0, out) == 1
            || (gUnknown_030046C0.unk06 <= 2 && sub_08061308(0x10, 1, out) == 1)
            || (gUnknown_0857680F[gUnknown_030046C0.unk06] != 2
                && sub_08061308(t, 2, out) == 1))
            goto alt;
    }

    while (gUnknown_085766E4[i].unk00 != 0xff)
    {
        if (gUnknown_085766E4[i].unk03 <= 0xfd
            && gUnknown_0857680F[gUnknown_030046C0.unk06] == gUnknown_085766E4[i].unk02)
        {
            out[0] = gUnknown_085766E4[i].unk00;
            out[1] = gUnknown_085766E4[i].unk01;
            best = i;
            break;
        }
        i++;
    }

    if (out[0] == 9999)
        return 0;

    gUnknown_085766E4[best].unk03 = 0xfe;
    return 1;

alt:
    r = sub_08061668(out);
    if (r != 0)
        return 1;

    return 0;
}
