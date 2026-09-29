#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A8DC.
 * sub_0802A8DC @ 0x0802A8DC
 */

void sub_0802A8DC(int a1, int a2, int a3, int a4, int a5)
{
    int t;
    int k;
    int m;
    int n;

    t = sub_0803EED4(a1, a2);
    k = t & 0x1f;
    m = sub_080249C8(t);
    n = sub_08024984(t);

    sub_08011E54((void *)GetTerrainNameGraphic(k), (void *)0x06013940, 0x100);

    PutOamHi((a3 + gUnknown_0849A2A6[a5 * 3] + gUnknown_0849A284[0x14]) & 0x1ff,
                 (a4 + gUnknown_0849A284[0x15]) | 0x400,
                 (void *)gUnknown_0849A240,
                 0x11ca);

    if (gMap->unk234A[
            gMap->rowOffset[a2] + a1] == 0 && k != 8)
        m = 0;

    sub_08011E54((void *)sub_0802A880(k, n), (void *)0x06013CC0, 0x100);
    ApplyPaletteExt((u16 *)GetTerrainNamePalette(k, m), 0x2c0, 0x20);

    PutOamHi((a3 + gUnknown_0849A2A6[a5 * 3] + gUnknown_0849A284[0x12]) & 0x1ff,
                 ((a4 + gUnknown_0849A284[0x13]) & 0xff) | 0x400,
                 (void *)gUnknown_0849A1F0,
                 0x61e6);
}
