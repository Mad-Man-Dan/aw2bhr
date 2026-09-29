#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074BDC.
 * sub_08074BDC @ 0x08074BDC, sub_08074C1C @ 0x08074C1C
 */

/* The camera-relative twin of GetWorldMapCameraXCentered: the same `x - 0x50` clamped into
 * [0, 0xc0], but applied only when x falls outside the 0x50..0xa0 band around
 * the scroll origin, and otherwise passing the origin itself back.
 *
 * `gUnknown_081CC4D8` is NOT a global.  The ROM word there holds 0x0202FDFC --
 * checked against baserom.gba -- so it is agbcc's own `-fforce-addr` address
 * constant for &gUnknown_0202FDFC, and GetWorldMapCameraYKeepInBand's `gUnknown_081CC4DC` is
 * the private second copy of the very same address.  The honest spelling names
 * the global and lets the build place the pool word.
 *
 * `v` is seeded from the origin and the SECOND test re-reads it, because the
 * first block may already have overwritten `v`; that is why the ROM has two
 * `ldrsh` off one address register rather than one.
 */
int GetWorldMapCameraXKeepInBand(int x)
{
    int v = gUnknown_0202FDFC.unk00;

    if (v + 0x50 > x)
    {
        v = x - 0x50;

        if (v < 0)
            v = 0;
    }

    if (gUnknown_0202FDFC.unk00 + 0xa0 < x)
    {
        v = x - 0x50;

        if (v > 0xc0)
            v = 0xc0;
    }

    return v;
}
asm(".global sub_08074BDC\n.thumb_set sub_08074BDC, GetWorldMapCameraXKeepInBand\n");

/* GetWorldMapCameraXKeepInBand on the other axis: unk02 instead of unk00, and 0x28 / 0x78 /
 * 0x60 instead of 0x50 / 0xa0 / 0xc0 -- the same 0x50-vs-0x28 and 0xc0-vs-0x60
 * pairing GetWorldMapCameraXCentered and GetWorldMapCameraYCentered already show.
 */
int GetWorldMapCameraYKeepInBand(int x)
{
    int v = gUnknown_0202FDFC.unk02;

    if (v + 0x28 > x)
    {
        v = x - 0x28;

        if (v < 0)
            v = 0;
    }

    if (gUnknown_0202FDFC.unk02 + 0x78 < x)
    {
        v = x - 0x28;

        if (v > 0x60)
            v = 0x60;
    }

    return v;
}
asm(".global sub_08074C1C\n.thumb_set sub_08074C1C, GetWorldMapCameraYKeepInBand\n");
