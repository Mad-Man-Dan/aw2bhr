#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E4CC.
 * sub_0804E4CC @ 0x0804E4CC
 */

void BattleshipPart2_StreamHook(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 f;
    u16 side;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    e = d >> 6;
    f = d & 0x3f;
    side = gUnknown_03001470[a].unk30;
    p[2] = (p[2] & 0xfc00) + f + oam.tileNum;

    if (a == gUnknown_02029668[side][4] && d != gUnknown_03001470[a].unk28)
        RegisterDataMove((u8 *)gUnknown_08552FB0[side] + (e << 11),
                     (void *)(0x06011800 + (side << 13)), 0x800);
}
asm(".global sub_0804E4CC\n.thumb_set sub_0804E4CC, BattleshipPart2_StreamHook\n");
