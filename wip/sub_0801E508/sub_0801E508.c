#include "global.h"

/*
 * sub_0801E508 -- copy a list of sprite pieces into the OAM buffer, rotated
 * and scaled by one entry of gUnknown_0200F720.
 *
 * a4 points at a count followed by that many records of three halfwords; a1 is
 * the first OAM slot to fill, (a2, a3) the screen offset and a5 selects the
 * entry: e[0] and e[1] are the x and y scale, e[2] the angle. Each record's
 * x and y are turned into a float, rotated with sub_0808B91C and sub_0808B710
 * (cosine and sine of the angle), divided by the scale and offset. Pieces
 * flagged 0x300 use the full-size offset from sub_0801E3B4; the others use
 * half of it. Returns 1 if the list does not fit in the 128 OAM slots.
 *
 * Why the C looks odd: `angle` holds e[2] for three of the four rotation calls
 * in the full-size branch and the rest read e[2] again. The compiler only
 * produces the original code with exactly this mix.
 */
int sub_0801E508(int a1, int a2, int a3, u16 *a4, int a5)
{
    u16 *p;
    int n;
    int z;
    u16 *dst;
    s16 *e;
    int w;
    int u;
    int v;
    int xr;
    int yr;
    int du;
    int dw;
    float fz;
    float fv;
    float fx;
    float fy;
    s16 angle;

    n = *a4;
    p = a4 + 1;

    if (a1 + n > 0x80)
        return 1;

    dst = &gUnknown_03002520[a1 * 4];

    if (n == 0)
        return 0;

    e = (s16 *)&gUnknown_0200F720[a5];

    do
    {
        w = *p++;

        v = w & 0xFF;

        if (v & 0x80)
            v |= ~0xFF;

        if (w & 0x100)
            u = (*p++ & 0xC1FF) | gUnknown_0808F0B8[a5];
        else
            u = *p++;

        z = u & 0x1FF;

        if (z & 0x100)
            z |= ~0x1FF;

        angle = e[2];

        if ((w & 0x300) == 0x300)
        {
            z += sub_0801E3B4(u);
            v += sub_0801E3B4(w);

            du = sub_0801E3B4(u);
            fz = (float)z;
            fx = fz * sub_0808B91C((float)angle) / (float)e[0];
            fv = (float)v;
            fy = fv * sub_0808B710((float)angle) / (float)e[0];
            xr = (int)(fx + fy + (float)a2 - (float)du);

            dw = sub_0801E3B4(w);
            fz = (float)(-z);
            fx = fz * sub_0808B710((float)angle) / (float)e[1];
            fy = fv * sub_0808B91C((float)e[2]) / (float)e[1];
            yr = (int)(fx + fy + (float)a3 - (float)dw);
        }
        else
        {
            z += sub_0801E3B4(u) / 2;
            v += sub_0801E3B4(w) / 2;

            du = sub_0801E3B4(u);
            fz = (float)z;
            fx = fz * sub_0808B91C((float)e[2]) / (float)e[0];
            fv = (float)v;
            fy = fv * sub_0808B710((float)e[2]) / (float)e[0];
            xr = (int)(fx + fy + (float)a2 - (float)(du / 2));

            dw = sub_0801E3B4(w);
            fz = (float)(-z);
            fx = fz * sub_0808B710((float)e[2]) / (float)e[1];
            fy = fv * sub_0808B91C((float)e[2]) / (float)e[1];
            yr = (int)(fx + fy + (float)a3 - (float)(dw / 2));
        }

        *dst++ = (w & ~0xFF) | (yr & 0xFF);
        *dst++ = (u & ~0x1FF) | (xr & 0x1FF);
        *dst = *p++;
        dst += 2;
    } while (--n != 0);

    return 0;
}





