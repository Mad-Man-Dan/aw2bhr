#include "global.h"

/*
 * sub_0802216C -- write a 2 x 2 block of tilemap entries at dst.
 *
 * dst points at the top-left entry; the second row is 32 entries further on.
 * Every entry is a base tile number (t, from gUnknown_0809097C) plus an offset.
 * The player index is a3 >> 6, with 0x100 meaning "use gUnknown_03003F2C".
 * r comes from sub_080261A4 for that index. The offsets depend on whether a3 is
 * 0 or 0x80 (most entries then get 0x400 added) and on the bottom row's conditions:
 * sub_0802706C, a4, a5, a7 for the left entry; a8 and a table lookup on a6 for
 * the right one.
 *
 * Why the C looks odd: the sums are spelled in the order the original compiler
 * needs. `(u16)(t + 0x6C)` is a no-op in value but keeps the compiler from
 * merging that sum with its neighbours.
 */
void sub_0802216C(u16 *dst, u8 a2, u16 a3, u8 a4, u8 a5, u16 a6, u16 a7, u8 a8)
{
    u16 t;
    u16 r;

    if (a3 == 0x100)
    {
        a3 = gUnknown_03003F2C;
        t = gUnknown_0809097C[0];
    }
    else
    {
        t = gUnknown_0809097C[(a3 >> 6) + 1];
    }

    if (a4 != 0)
        a4 = 1;

    r = sub_080261A4((a3 >> 6) + 1, a2);

    if (a3 == 0 || a3 == 0x80)
    {
        dst[0] = t + (r + 1) + 0x400;
        dst[1] = r + t + 0x400;

        if (sub_0802706C(a2, gUnknown_030033EC, (a3 >> 6) + 1))
            dst[0x20] = t + 0x79;
        else if (a4)
            dst[0x20] = t + 0x76;
        else if (a5)
            dst[0x20] = t + 0x77;
        else if (a7)
            dst[0x20] = t + 0x78;
        else
            dst[0x20] = t + (r + 3) + 0x400;

        if (!(gPlayers[(a3 >> 6) + 1].turnState & 2) && a8)
            dst[0x21] = t + 0x7A;
        else if (gUnknown_08090986[a6] < 0)
            dst[0x21] = t + (r + 0x402);
        else
            dst[0x21] = a6 + (u16)(t + 0x6C);
    }
    else
    {
        dst[0] = r + t;
        dst[1] = t + (r + 1);

        if (sub_0802706C(a2, gUnknown_030033EC, (a3 >> 6) + 1))
            dst[0x20] = t + 0x79;
        else if (a4)
            dst[0x20] = t + 0x76;
        else if (a5)
            dst[0x20] = t + 0x77;
        else if (a7)
            dst[0x20] = t + 0x78;
        else
            dst[0x20] = t + (r + 2);

        if (!(gPlayers[(a3 >> 6) + 1].turnState & 2) && a8)
            dst[0x21] = t + 0x7A;
        else if (gUnknown_08090986[a6] < 0)
            dst[0x21] = t + (r + 3);
        else
            dst[0x21] = a6 + (u16)(t + 0x6C);
    }
}
