#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E134.
 * sub_0804E134 @ 0x0804E134
 */

/* One of the F088 trio (CruiserPart1Alt_Init / CruiserPart1_Init / BattleshipPart1_Init): the same
 * sprite-attribute setter as CruiserPart2_Init, minus the two zeroing stores and the
 * SetSlotSpriteHook continuation, and with its own tile base. The three members are
 * byte-identical apart from that immediate. */

void BattleshipPart1_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100 + 0x40;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
}
asm(".global sub_0804E134\n.thumb_set sub_0804E134, BattleshipPart1_Init\n");
