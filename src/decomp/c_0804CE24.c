#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804CE24.
 * sub_0804CE24 @ 0x0804CE24
 */

void LanderPart_StreamHook(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 f;
    u16 side;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    e = d / 0x38;
    f = d % 0x38;
    side = gUnknown_03001470[a].unk30;
    p[2] = (p[2] & 0xfc00) + f + oam.tileNum;

    if (a == gUnknown_02029668[side][4] && d != gUnknown_03001470[a].unk28)
        RegisterDataMove((u8 *)gUnknown_08552FB0[side] + e * 0x700,
                     (void *)(0x06011000 + (side << 13)), 0x700);
}
asm(".global sub_0804CE24\n.thumb_set sub_0804CE24, LanderPart_StreamHook\n");
