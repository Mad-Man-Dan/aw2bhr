#include "global.h"

/* Lays out one unit's info panel: the portrait, two text rows, an optional
 * third, and a per-terrain icon.
 *
 * gTextTable IS a real table, not an agbcc -fforce-addr pool word: it is
 * indexed by a computed subscript (`lsls #2; adds`) BEFORE anything is
 * dereferenced. The words at 0x0812A110 and 0x0812A114 are the opposite case --
 * they hold 0x08499578 and 0x084C20C0, i.e. they are private address constants
 * for gUnknown_08499578 and gUnknown_084C20C0 -- so both are spelled honestly
 * here and neither is declared.
 *
 * `lsrs r0,#0x1f; adds; asrs #1` after sub_08014CEC is a SIGNED divide by two,
 * not a shift, and the result is centred with `w / 2 - 0x50`. The `lsls #0xd;
 * asrs #0x10` that follows is a `/ 8` fused with an (s16) cast, where the
 * unbiased `(a + 0x38) / 8` further down is unsigned because a is u8.
 *
 * `t` has to be a u32 local: the ROM spills `a + 0x38` and reloads it before
 * the second divide, and across the spill combine has lost the range fact that
 * makes the divide unsigned. The two `(a + 0x50) / 8` uses are recomputed from
 * the parameter, so they keep it. */

void sub_08046914(u8 a, u8 b)
{
    u8 *gfx;
    u32 t;
    u16 i;

    gfx = gTextTable[gUnknown_085D583C[b].nameIndex];
    sub_080149C0((s16)((a - (sub_08014CEC(gfx) / 2 - 0x50) + 4) / 8), 1,
                 gUnknown_08499578, gfx, 0x8000, 0);

    t = a + 0x38;
    sub_08014A5C(t / 8, 3, gUnknown_08499578, 0x960, 0x8000, 0);

    if (gUnknown_02028DD4 == 0)
        sub_08014A5C((a + 0x50) / 8, 3, gUnknown_08499578, 0x969, 0x8000, 0);

    if (sub_08026C6C(b) == 0)
        sub_08014A5C((a + 0x50) / 8, 5, gUnknown_08499578, 0x969, 0x8000, 0);
    else
        sub_08014B0C((a + 0x60) / 8, 5, gUnknown_08499578, sub_08026C6C(b),
                     0x8000, 0);

    for (i = 0; i <= 2; i++)
    {
        if (gUnknown_085D5ABC[gUnknown_084C20C0[i]].repairTable[b] != 0)
        {
            sub_08014A5C(t / 8, 7, gUnknown_08499578, 0x961, 0x8000, 0);
            break;
        }
    }

    sub_08013AEC();
}
