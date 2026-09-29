#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804F658.
 * sub_0804F658 @ 0x0804F658
 */

/* MATCHED (wave 87, W87-B) -- see work/FlyerFigure_Init/W87-notes.md.
 * Three constructs closed a 68-wave "register allocation only" park:
 *   1. oam.tileNum = g * 0x100 (W86-E's twin find, kept);
 *   2. the (s16)v + unk04 arithmetic written through a `static inline`
 *      helper -- semantically identical, inlined away, but it re-cuts the
 *      live ranges and is what puts the 0xb4 stride and `pos` in the ROM's
 *      registers.  Found by the permuter, which invents these helpers;
 *   3. the two-bit guard reads *(u8 *)&gUnknown_03004504 TWICE, inline at
 *      each test, rather than binding it to a local -- which is what makes
 *      the ROM's `ands r0,r1` write the constant's register.  This REFUTES
 *      this park's settled claim that only the single-local spelling gives
 *      the ROM's two separate mask tests.
 * The element address is bound with & and read through ->, as the sibling
 * BomberFigure_Init does; the row-subscript form emits `adds r0,r0,r1`. */

struct UnkPosPair
{
    u16 x;
    u16 y;
};
static inline int inline_fn(int arg0, u16 arg1)
{
    return ((s16)arg0) + arg1;
}

void FlyerFigure_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;
    struct UnkPosPair (*pos)[10];
    struct UnkPosPair *row1;
    struct UnkPosPair *row2;
    int v;
    int flags;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);
    gUnknown_03001470[gUnknown_03001FBC].unk28 = gUnknown_0300451C << 3;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;
    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100;
    prio = gUnknown_085523A4[gUnknown_0300453C ^ gUnknown_0300450C];
    oam.priority = prio;
    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);
    pos = (struct UnkPosPair (*)[10])GetFigurePositionTable(
        gUnknown_085D6A48[gUnknown_03004580[gUnknown_0300453C][1]][0],
        gUnknown_03004580[gUnknown_0300453C][3]);
    v = gUnknown_085644E0[gUnknown_0300453C][gUnknown_0300450C];
    ;
    if ((*((u8 *) (&gUnknown_03004504))) & 1)
    {
        if (!((*((u8 *) (&gUnknown_03004504))) & 0x40))
            v = 0;
    }
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 =
        (row1 = &pos[gUnknown_03004580[gUnknown_0300453C][5]]
            [gUnknown_0300453C * 5 + gUnknown_0300451C])->x;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x =
        inline_fn(v, gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04);
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 =
        (row2 = &pos[gUnknown_03004580[gUnknown_0300453C][5]]
            [gUnknown_0300453C * 5 + gUnknown_0300451C])->y;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;
    StartFigureEntrySlide(gUnknown_0300453C, gUnknown_0300451C,
                 gUnknown_0855214C[gUnknown_0300453C], 0x32);
    SetSlotSpritePosition(gUnknown_03001FBC,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x,
                 gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
}
asm(".global sub_0804F658\n.thumb_set sub_0804F658, FlyerFigure_Init\n");
