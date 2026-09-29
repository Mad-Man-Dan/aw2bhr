#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E050.
 * sub_0804E050 @ 0x0804E050
 */

void FigureTileHook_Ship(s16 a, u16 *p)
{
    struct OamData oam;
    u16 d;
    u16 e;
    u16 side;
    u16 slot;

    CopySlotSpriteAttrs(a, (struct UnkVec *)&oam);
    d = (p[2] - oam.tileNum) & 0x3ff;
    side = gUnknown_03001470[a].unk30;
    slot = gUnknown_03001470[a].unk34;
    e = d >> 6;
    p[2] = (p[2] & 0xfc00) + oam.tileNum;

    if (d != gUnknown_03001470[a].unk28
        && a == gUnknown_02029A10[side].entries[slot].unk18)
    {
        gUnknown_03001470[a].unk28 = d;
        CopyFigurePose800(side, slot, e);
    }
}
asm(".global sub_0804E050\n.thumb_set sub_0804E050, FigureTileHook_Ship\n");
