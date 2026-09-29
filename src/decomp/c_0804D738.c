#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804D738.
 * sub_0804D738 @ 0x0804D738, sub_0804D818 @ 0x0804D818
 */

/* One of the F088 trio (sub_0804D738 / sub_0804D818 / sub_0804E134): the same
 * sprite-attribute setter as sub_0804D928, minus the two zeroing stores and the
 * SetSlotSpriteHook continuation, and with its own tile base. The three members are
 * byte-identical apart from that immediate. */

void sub_0804D738(void)
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
    oam.tileNum = gUnknown_0300453C * 0x100 + 0x10;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
}

/* One of the F088 trio (sub_0804D738 / sub_0804D818 / sub_0804E134): the same
 * sprite-attribute setter as sub_0804D928, minus the two zeroing stores and the
 * SetSlotSpriteHook continuation, and with its own tile base. The three members are
 * byte-identical apart from that immediate. */

void sub_0804D818(void)
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
