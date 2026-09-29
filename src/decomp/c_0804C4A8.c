#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C4A8.
 * sub_0804C4A8 @ 0x0804C4A8
 */

void SpawnSubmarineFigure(u16 a)
{
    u16 t;
    u16 u;
    int n;

    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];

    t = gUnknown_03004580[a][0];
    u = gUnknown_03004580[a][4];

    SpawnWholeFigure(a);

    Decompress((u8 *)gUnknown_08557680[gUnknown_08562128[u]][0], gUnknown_08552FB0[a]);

    n = gUnknown_08552178[a][4] * 7;

    gUnknown_02029668[a][4] = sub_08015410(gUnknown_0855333C, 1,
        gUnknown_08557680[t][2], gUnknown_08557680[t][1], n);
}
asm(".global sub_0804C4A8\n.thumb_set sub_0804C4A8, SpawnSubmarineFigure\n");
