#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804B180.
 * sub_0804B180 @ 0x0804B180
 */

union Attr01Word
{
    u32 raw;
    struct { u32 low:28; u32 hFlip:1; u32 high:3; } bits;
};
union Attr2Word
{
    u32 raw;
    struct { u32 tileNum:10; u32 priority:2; u32 paletteNum:4; u32 high:16; } bits;
};
static inline u32 set_h(u32 raw, u32 val)
{
    union Attr01Word u;
    u.raw = raw;
    u.bits.hFlip = val;
    return u.raw;
}
static inline u32 set_pal(u32 raw, u32 val)
{
    union Attr2Word u;
    u.raw = raw;
    u.bits.paletteNum = val;
    return u.raw;
}
static inline u32 set_tile(u32 raw, u32 val)
{
    union Attr2Word u;
    u.raw = raw;
    u.bits.tileNum = val;
    return u.raw;
}
static inline u32 set_prio(u32 raw, u32 val)
{
    union Attr2Word u;
    u.raw = raw;
    u.bits.priority = val;
    return u.raw;
}

void FigureSprite_Init(void)
{
    struct UnkVec v;

    v = GetSlotSpriteAttrs(gUnknown_03001FBC);
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    v.unk00 = set_h(v.unk00, 1 ^ gUnknown_0300453C);
    v.unk04 = set_pal(v.unk04, gUnknown_08551D0C[0][gUnknown_0300453C * 3 + 2]);
    v.unk04 = set_tile(v.unk04, gUnknown_08551D18[gUnknown_0300453C]);
    v.unk04 = set_prio(v.unk04, gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C]);

    SetSlotSpriteAttrs(gUnknown_03001FBC, v);
    SetSlotSpritePosition(gUnknown_03001FBC,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
}
asm(".global sub_0804B180\n.thumb_set sub_0804B180, FigureSprite_Init\n");
