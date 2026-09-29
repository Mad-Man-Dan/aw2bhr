#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086688.
 * sub_08086688 @ 0x08086688
 */

struct Unk8086688
{
    /* 0x00 */ u8 filler_00[0x48];
    /* 0x48 */ s16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x0e];
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x04];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6a */ s16 unk6a;
};

void MapSelectList_DrawFrame(ProcPtr arg)
{
    struct Unk8086688 *p = arg;
    int i;

    MapSelectList_StepTransition(arg);
    MapSelectList_DrawPropertyIcons();
    MapSelectList_DrawPropertyCounts(p->unk64, p->unk66, p->unk68, p->unk6a);
    MapSelectList_DrawScrollArrows(p->unk5c);
    MapSelectList_DrawScrollbar(p->unk5c);

    if (gPlaySt.gameMode == 2)
        MapSelectList_DrawRowCursor(gUnknown_03005930, 0x28);
    else
        MapSelectList_DrawRowCursor(gUnknown_03005980, 0x48);

    if (p->unk4c <= 0x1D)
    {
        ApplyPaletteExt((u16 *)(DivRem(Div(p->unk48, 4), 0x10) * 2 + (int)gUnknown_08239F84),
                        0x298, 2);

        i = DivRem(Div(p->unk48, 4), 0x10) * 2;

        ApplyPaletteExt((u16 *)((u8 *)gUnknown_0823E550 + i),
                        0x2a0,
                        (0x10 - DivRem(Div(p->unk48, 4), 0x10)) * 2);

        ApplyPaletteExt(gUnknown_0823E550,
                        (u16)((0x10 - DivRem(Div(p->unk48, 4), 0x10)) * 2 + 0x2a0),
                        DivRem(Div(p->unk48, 4), 0x10) * 2);

        if (--p->unk48 == 0)
            p->unk48 = 0x40;
    }
}
asm(".global sub_08086688\n.thumb_set sub_08086688, MapSelectList_DrawFrame\n");
