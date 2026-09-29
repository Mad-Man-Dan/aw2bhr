#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804C0FC.
 * sub_0804C0FC @ 0x0804C0FC
 */

void SpawnSideFigures(u16 side)
{
    u16 i;
    u16 *row0;
    u16 *row;

    LoadFigurePalettes(gUnknown_03004500[side], gUnknown_03004580[side][1],
        gUnknown_08551CFC[side][0], gUnknown_08551CFC[side][1]);
    sub_0804FF44(side);

    row0 = gUnknown_085D6A48[gUnknown_03004580[side][1]];
    if (row0[2] == 0)
    {
        for (i = 0; i < 5; i++)
        {
            if (gUnknown_02029A10[side].entries[i].unk00 != 0)
                StartFigureSlot(side, i);
        }
        CopyFigureSheetToVram(side);
    }

    row = gUnknown_085D6A48[gUnknown_03004580[side][1]];
    if (row[2] == 1)
    {
        if (row[0] == 6)
            SpawnWholeFigure2(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x13)
            SpawnWholeFigure(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x10)
            SpawnWholeFigure3(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x14)
            sub_0804DB14(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x15)
            SpawnCruiserFigure(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x16)
            SpawnLanderFigure(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x17)
            SpawnSubmarineFigure(side);
        if (gUnknown_085D6A48[gUnknown_03004580[side][1]][0] == 0x11)
            sub_0804C578(side);
        CopyFigureSheetToVram(side);
    }
}
asm(".global sub_0804C0FC\n.thumb_set sub_0804C0FC, SpawnSideFigures\n");
