#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804EEFC.
 * sub_0804EEFC @ 0x0804EEFC
 */

/* Rebuilds the OBJ attributes for the current gUnknown_03001470 slot and then
 * reseeds the entry's position from a ROM table of x/y pairs. Same family as
 * CruiserPart2_Init / BattleshipPart2_Init -- see "Large functions" in
 * docs/agbcc-codegen.md for the `* 0x100` and the `pal`/`prio` temporaries.
 *
 * The two `e1 = &...` / `e2 = &...` bindings are load-bearing rather than
 * decorative: with a bare `pos[i].x` agbcc folds the index shift into the
 * address add and canonicalises the commutative operands the other way round,
 * giving `adds r1, r1, r0` where the ROM has `adds r1, r0, r1`. Giving the
 * element address a second use blocks that combination. */
struct Unk4EEFCPair
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
};

void WholeFigure_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;
    struct Unk4EEFCPair *pos;
    struct Unk4EEFCPair *e1;
    struct Unk4EEFCPair *e2;

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

    pos = (struct Unk4EEFCPair *)GetFigurePositionTable(
        gUnknown_085D6A48[gUnknown_03004580[gUnknown_0300453C][1]][0],
        gUnknown_03004580[gUnknown_0300453C][3]);

    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 =
        (e1 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->x;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 =
        (e2 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->y;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;

    SetSlotSpritePosition(gUnknown_03001FBC,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y);
}
asm(".global sub_0804EEFC\n.thumb_set sub_0804EEFC, WholeFigure_Init\n");
