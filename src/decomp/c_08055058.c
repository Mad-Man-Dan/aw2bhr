#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08055058.
 * sub_08055058 @ 0x08055058
 */

u16 LoadDeathEffectArt(u16 a, u16 b, u16 c, u16 d)
{
    u16 v[2];
    u16 side;
    u16 off;
    u16 n;
    u16 r;
    u16 i;
    u16 j;

    side = gUnknown_0300450C;
    v[0] = gUnknown_02029808[0].unk30[0];
    v[1] = gUnknown_02029808[1].unk30[0];
    for (i = 0; i < 5; i++) {
        gUnknown_02029808[side].unk3a[i] = 0x200;
        gUnknown_02029808[side ^ 1].unk3a[i] = 0x200;
    }
    off = 0x4000;
    n = gUnknown_08552D80[v[side ^ 1]].unk00;
    if (n != 0)
        CpuFastSet(gUnknown_08552D80[v[side ^ 1]].unk08, (void *)(0x06010000 + off), n / 4);
    r = (n >> 5) + 0x200;
    if (c == 2) {
        off += n;
        r = off >> 5;
        n = gUnknown_08552D80[d].unk00;
        if (n != 0)
            CpuFastSet(gUnknown_08552D80[d].unk08, (void *)(0x06010000 + off), n / 4);
        for (j = 0; j < 2; j++) {
            for (i = 0; i < 5; i++) {
                if (gUnknown_02029808[j].unk30[i] == 2)
                    gUnknown_02029808[j].unk3a[i] = 0x200;
                else
                    gUnknown_02029808[j].unk3a[i] = r;
            }
        }
        r = (off >> 5) + (n >> 5);
    } else if (v[side] != v[side ^ 1]) {
        off += n;
        n = gUnknown_08552D80[v[side]].unk00;
        if (n != 0)
            CpuFastSet(gUnknown_08552D80[v[side]].unk08, (void *)(0x06010000 + off), n / 4);
        for (i = 0; i < 5; i++)
            gUnknown_02029808[side].unk3a[i] = r;
        r = (off >> 5) + (n >> 5);
    }
    return r;
}
asm(".global sub_08055058\n.thumb_set sub_08055058, LoadDeathEffectArt\n");
