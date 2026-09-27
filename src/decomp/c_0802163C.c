#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802163C.
 * sub_0802163C @ 0x0802163C
 */

/* Loads map <a> into gUnknown_08499590: for the 0xB4..0xBF range it forwards to
 * sub_08021750 (the "rebuild from the tile table already in the map" path), and
 * otherwise it seeds the header from the gUnknown_03003F68 blob, copies the
 * name, and fills tile[]/plane[] from the blob's halfword tile stream.
 *
 * The guard is agbcc's own range test: `a >= 0xb4 && a <= 0xbf` goes through
 * fold's build_range_check and comes out as `(unsigned)(a - 0xb4) <= 0xb`,
 * which is the ROM's `subs r0,#0xb4 / cmp r0,#0xb / bhi`.  The parameter is an
 * `int` (unknown-functions.h already says so), so the subtraction happens in
 * SImode and there is no truncation -- contrast sub_08021810, whose copy of the
 * same test runs on a u8 member and therefore carries `lsls #0x18; lsrs #0x18`.
 *
 * gUnknown_03003F68's blob is u8 width, u8 height, then the halfword tile
 * stream at +2; the `ldrh r4,[r0,#2]` with the index already scaled is the +2
 * folded into the load displacement, i.e. a member array and not `+ 1` on a
 * bare u16 pointer. */

void sub_0802163C(int a)
{
    int x;
    int y;
    u16 t;

    if (a >= 0xb4 && a <= 0xbf)
    {
        sub_08021750(a);
        return;
    }

    LoadMapData(a);

    gMap->width =
        gUnknown_03003F68->width;
    gMap->height =
        gUnknown_03003F68->height;
    gMap->scrollX = 0;
    gMap->scrollY = 0;
    gMap->unk08 = 0;
    gMap->unk0a = 0;
    gMap->camX = 0;
    gMap->camY = 0;
    gMap->unk10 = 0;

    CopyString(gMap->unk421a,
                 sub_08024944(a));
    gMap->unk4233 = sub_0802490C(a);

    sub_080215FC();

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            t = gUnknown_03003F68->tile[
                    gMap->rowOffset[y] + x];
            gMap->tile[
                gMap->rowOffset[y] + x] = t;
            gMap->terrain[
                gMap->rowOffset[y] + x] =
                gUnknown_0849959C[t];
        }
    }

    sub_0802481C();
}
