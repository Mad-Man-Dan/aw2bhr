#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804DCA8.
 * BattleshipFigure_Init @ 0x0804DCA8
 */

/* MATCHED -- byte-for-byte identical to the original.
 *
 * Twin of CruiserFigure_Init at normalised shape ratio 0.960, and the only source
 * difference is the priority lookup: this one reads the same eight halfwords as
 * `gUnknown_0855239C[gUnknown_0300453C * 2 + gUnknown_0300450C]` where the twin
 * uses the XOR index on gUnknown_085523A4 (== 0855239C + 8). Transcribed from
 * the twin; it matched in one compile both before and after the twin's last
 * fix, which is what confirms the two functions fail and close together.
 *
 * See CruiserFigure_Init for why the `ox`/`oy` locals, the zero-trip loop and the
 * `(meta = gUnknown_03004580, ...)` anchor are all load-bearing.
 */

struct UnkPosPair
{
    u16 x;
    u16 y;
};

void BattleshipFigure_Init(void)
{
    struct OamData oam;
    u16 pal;
    u16 prio;
    struct UnkPosPair *pos;
    struct UnkPosPair *e1;
    struct UnkPosPair *e2;
    u16 (*meta)[8];
    u16 *ox;
    u16 *oy;

    CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *)&oam);

    gUnknown_03001470[gUnknown_03001FBC].unk2c = 0;
    gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
    gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_0300451C;

    oam.hFlip = gUnknown_0300453C ^ 1;
    pal = gUnknown_08551D0C[gUnknown_0300453C][0];
    oam.paletteNum = pal;
    oam.tileNum = gUnknown_0300453C * 0x100;
    prio = gUnknown_0855239C[gUnknown_0300453C * 2 + gUnknown_0300450C];
    oam.priority = prio;

    do
    {
        pos = (struct UnkPosPair *)GetFigurePositionTable(
            gUnknown_085D6A48[(meta = gUnknown_03004580,
                gUnknown_03004582[gUnknown_0300453C][0])][0],
            gUnknown_03004580[gUnknown_0300453C][3]);
    } while (0);

    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04 =
        (e1 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->x;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk04;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06 =
        (e2 = &pos[gUnknown_0300453C * 5 + gUnknown_0300451C])->y;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y =
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk06;
    gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].unk1e = 0;

    StartFigureEntrySlide(gUnknown_0300453C, 0, 0, 0);

    SetSlotSpriteAttrs(gUnknown_03001FBC, *(struct UnkVec *)&oam);

    ox = gUnknown_084C3F70[gUnknown_0300453C];
    oy = gUnknown_084C3F78[gUnknown_0300453C];
    SetSlotSpritePosition(gUnknown_03001FBC,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x - *ox,
        gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y - *oy);

    SetSlotSpriteHook(gUnknown_03001FBC,
        gUnknown_08552FB8[gUnknown_03004582[gUnknown_0300453C][0]]
                         [gUnknown_03004580[gUnknown_0300453C][2]][2]);
}

asm(".global sub_0804DCA8\n.thumb_set sub_0804DCA8, BattleshipFigure_Init\n");
