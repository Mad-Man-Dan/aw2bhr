#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805521C.
 * LoadThirdEffectArt @ 0x0805521C, LoadProjectileArt @ 0x08055288, sub_0805530C @ 0x0805530C, sub_08055374 @ 0x08055374
 */

#include "hardware.h"

u16 LoadThirdEffectArt(u16 a, u16 b, u16 c, u16 tile)
{
    struct Unk020296B0 *p;
    u16 len;
    const void *src;
    u16 dst;

    p = &gUnknown_020296B0[a];
    p->unk00 = tile;
    dst = (u16)(tile * 0x20);
    len = gUnknown_085D70A8[b][c].unk00;
    src = gUnknown_085D70A8[b][c].unk04;

    if (len != 0)
        CpuFastSet(src, (void *)(0x06010000 + dst), len / 4);
    else
        p->unk00 = 0;

    tile += len >> 5;
    return tile;
}

u16 LoadProjectileArt(u16 a, u16 b, u16 c, u16 d, u16 tile)
{
    u16 *row;
    u16 len;
    const void *src;
    u16 dst;

    row = gUnknown_085D6A48[b];

    if (row[10] == 0 || d != 1)
        return tile;

    gUnknown_02029710[a].unk00 = tile;
    dst = (u16)(tile * 0x20);
    len = gUnknown_08553D80.unk00[row[10]][c].unk00;
    src = gUnknown_08553D80.unk00[row[10]][c].unk04;

    if (len != 0)
        CpuFastSet(src, (void *)(0x06010000 + dst), len / 4);

    tile += len >> 5;
    return tile;
}

u16 sub_0805530C(u16 a, u16 b, u16 c, u16 tile)
{
    u16 len;
    const void *src;
    u16 dst;

    gUnknown_020298E0[a].unk00 = tile;
    dst = (u16)(tile * 0x20);
    len = gUnknown_085D7768[b][c].unk00;
    src = gUnknown_085D7768[b][c].unk04;

    if (len != 0)
        CpuFastSet(src, (void *)(0x06010000 + dst), len / 4);

    tile += len >> 5;
    return tile;
}

u16 sub_08055374(u16 a, u16 tile)
{
    u16 len;
    u16 dst;

    gUnknown_020297C0[0].unk00 = tile;
    gUnknown_020297C0[1].unk00 = tile;
    len = gUnknown_085D6EC8[a][0];
    dst = (u16)(gUnknown_020297C0[0].unk00 * 0x20);

    if (len != 0)
        CpuFastSet(*(const void **)&gUnknown_085D6EC8[a][2],
                   (void *)(0x06010000 + dst), len / 4);

    tile += len >> 5;
    return tile;
}

asm(".global sub_08055288\n.thumb_set sub_08055288, LoadProjectileArt\n");
asm(".global sub_0805521C\n.thumb_set sub_0805521C, LoadThirdEffectArt\n");
