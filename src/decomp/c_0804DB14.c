#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804DB14.
 * sub_0804DB14 @ 0x0804DB14
 */

void sub_0804DB14(u16 a)
{
    u16 A, B;
    u16 off;
    int n;

    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];
    A = gUnknown_03004580[a][0];
    B = gUnknown_03004580[a][4];
    SpawnWholeFigure(a);
    off = (a << 13) + 0x800;
    n = gUnknown_08552178[a][2] * 7;
    gUnknown_02029668[a][2] = sub_08015410(gUnknown_08553444, 1,
                                           gUnknown_08557B58[A][2],
                                           gUnknown_08557B58[A][1], n);
    CpuFastSet(gUnknown_08557B58[gUnknown_08562128[B]][0],
               (void *)(0x06010000 + off), 0x400);
    Decompress(gUnknown_08557B94[A][0], gUnknown_08552FB0[a]);
    n = gUnknown_08552178[a][4] * 7;
    gUnknown_02029668[a][4] = sub_08015410(gUnknown_08553474, 1,
                                           gUnknown_08557B94[A][2],
                                           gUnknown_08557B94[A][1], n);
}
