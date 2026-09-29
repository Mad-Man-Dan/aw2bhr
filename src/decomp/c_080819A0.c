#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080819A0.
 * sub_080819A0 @ 0x080819A0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk80819A0
{
    /* 00 */ STRUCT_PAD(0x00, 0x34);
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
};
void sub_08084700(struct Unk80819A0 *);

void MainMenuCarouselWheel_LabelSlideInLoop(struct Unk80819A0 *proc)
{
    int i;

    for (i = 0; i <= 4; i++)
    {
        if (i == 2)
            PutSprite(2, gUnknown_08616972[i + 1] & 0x1FF, gUnknown_08616980[i + 1] - 8,
                      gUnknown_08615C76, 0x8998);
        else
            PutSprite(2, gUnknown_08616972[i + 1], gUnknown_08616980[i + 1], gUnknown_08615C84,
                      ((gUnknown_0861696C[DivRem(proc->unk52 + i, 6)] + 2) << 12) | 0x800
                          | (gUnknown_0861696C[DivRem(proc->unk52 + i, 6)] * 32 + 0x1D8));
    }

    proc->unk34 = Interpolate(4, 0, 0x30, proc->unk4c, 8);

    if (gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] == 0)
    {
        if (GetCampaignSaveFlag() == 0)
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04, 0xAA98);
        else
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04,
                      ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xA98);

        PutSprite(3, proc->unk34 + 0x60, 0x58, gUnknown_08615C04,
                  ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xAA4);
    }
    else if (gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] == 1)
    {
        if (GetVersusSaveFlag() == 0)
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04, 0xBA98);
        else
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04,
                      ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xA98);

        PutSprite(3, proc->unk34 + 0x60, 0x58, gUnknown_08615C04,
                  ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xAA4);
    }
    else if (gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] == 5)
    {
        if (GetWarRoomSaveFlag() == 0)
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04, 0xCA98);
        else
            PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04,
                      ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xA98);

        PutSprite(3, proc->unk34 + 0x60, 0x58, gUnknown_08615C04,
                  ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xAA4);
    }
    else if (gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] == 3)
    {
        PutSprite(3, proc->unk34 + 0x60, 0x48, gUnknown_08615C04,
                  ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xAB0);
        PutSprite(3, proc->unk34 + 0x60, 0x58, gUnknown_08615C04,
                  ((gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)] + 2) << 12) | 0xABC);
    }

    ApplyPaletteExt(GetMainMenuTilePalette(gUnknown_0861696C[DivRem(gUnknown_03005934 + 2, 6)]), 0x300, 0x20);

    sub_08043C28(Interpolate(4, 0x78, 0, proc->unk4c, 8) + 0xb0, 0xa0, 0x1800, 4, 1);

    if (proc->unk4c <= 3)
        gUnknown_03002020++;

    if (proc->unk4c <= 7)
    {
        proc->unk4c++;
    }
    else
    {
        proc->unk4c = 0x20;
        Proc_Break(proc);
    }

    sub_08084700(proc);
}

asm(".global sub_080819A0\n.thumb_set sub_080819A0, MainMenuCarouselWheel_LabelSlideInLoop\n");
