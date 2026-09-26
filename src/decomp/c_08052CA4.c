#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052CA4.
 * sub_08052CA4 @ 0x08052CA4
 */

#include "hardware.h"

/* sub_08052154's sibling and the widest member of the src/decomp/c_08051DE0.c
 * cluster. Read that file's header first. It differs from sub_08052154 in
 * exactly three places: priority 2 rather than 1, the full runtime
 * gUnknown_08552D80 row (indexed by gUnknown_02029808[..].unk30[..], the same
 * row the exemplar uses) on BOTH x and y rather than a constant row on x only,
 * and the extra sub_08052E04 call ahead of sub_08015608.
 *
 * sub_08052E04's third argument is dead in the callee and every caller still
 * passes an explicit 0 -- see include/unknown-functions.h. Dropping it from the
 * prototype changes nothing the callee can see and breaks this call site. */
void sub_08052CA4(void)
{
    struct OamData oam;
    int tile;
    u16 x;
    u16 y;

    sub_0801566C(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.paletteNum = 8;
    tile = gUnknown_02029808[gUnknown_0300453C].unk3a[gUnknown_02029808[gUnknown_0300453C].unk2e];
    oam.tileNum = tile;
    oam.priority = 2;

    x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk02[gUnknown_0300453C];
    y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y
        + gUnknown_08552D80[gUnknown_02029808[gUnknown_0300453C].unk30[gUnknown_0300451C]].unk06;

    sub_08052E04(gUnknown_0300453C, gUnknown_0300451C, 0);
    sub_08015608(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    sub_08050528(gUnknown_0300453C, gUnknown_03001FBC, x, y);
}
