#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C8C8.
 * sub_0804C8C8 @ 0x0804C8C8, sub_0804C99C @ 0x0804C99C
 */

void SubmarinePart_StreamHook(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 f;
    u16 side;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    e = d / 0x30;
    f = d % 0x30;
    side = gUnknown_03001470[a].unk30;
    p[2] = (p[2] & 0xfc00) + f + oam.tileNum;

    if (a == gUnknown_02029668[side][4] && d != gUnknown_03001470[a].unk28)
        RegisterDataMove((u8 *)gUnknown_08552FB0[side] + e * 0x600,
                     (void *)(0x06010A00 + (side << 13)), 0x600);
}
asm(".global sub_0804C8C8\n.thumb_set sub_0804C8C8, SubmarinePart_StreamHook\n");

void SpawnLanderFigure(u16 a)
{
    u16 t;
    int n;

    gUnknown_0300453C = a;
    gUnknown_0300451C = gUnknown_08552148[a];

    t = gUnknown_03004580[a][0];

    SpawnWholeFigure(a);

    Decompress((u8 *)gUnknown_08557CFC[t][0], gUnknown_08552FB0[a]);

    n = gUnknown_08552178[a][4] * 7;

    gUnknown_02029668[a][4] = sub_08015410(gUnknown_0855339C, 1,
        gUnknown_08557CFC[t][2], gUnknown_08557CFC[t][1], n);
}
asm(".global sub_0804C99C\n.thumb_set sub_0804C99C, SpawnLanderFigure\n");
