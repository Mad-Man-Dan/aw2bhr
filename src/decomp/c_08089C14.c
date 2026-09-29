#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08089C14.
 * sub_08089C14 @ 0x08089C14, sub_08089F90 @ 0x08089F90
 */

#include "hardware.h"
struct Unk08089C14
{
    u8 filler0[0x48];
    u16 unk48;
    u8 filler4A[2];
    s16 unk4C;
};
struct Unk08089F90
{
    u8 filler0[0x48];
    u16 unk48;
    u8 filler4A[2];
    s16 unk4C;
};

void CoDesignEditor_DrawPickerToConfirm(struct Unk08089C14 *p)
{
    s32 i;
    s32 v;
    s32 u;

    if (DivRem(p->unk48, 24) <= 11)
        v = DivRem(p->unk48, 24);
    else
        v = 24 - DivRem(p->unk48, 24);

    v = v * Div(0xC0, 24) + 0x100;

    SetObjAffine(1,
                 Div(gSinLut[0x40] << 4, v != 0 ? v : 2),
                 Div(-gSinLut[0] << 4, v != 0 ? v : 2),
                 Div(gSinLut[0] << 4, v != 0 ? v : 2),
                 Div(gSinLut[0x40] << 4, v != 0 ? v : 2));

    if (p->unk4C <= 15)
    {
        u = Interpolate(1, 0x100, 0x10, p->unk4C, 0x10);

        SetObjAffine(2,
                     Div(gSinLut[0x40] << 4, u != 0 ? u : 2),
                     Div(-gSinLut[0] << 4, u != 0 ? u : 2),
                     Div(gSinLut[0] << 4, u != 0 ? u : 2),
                     Div(gSinLut[0x40] << 4, u != 0 ? u : 2));

        SetObjAffine(3,
                     Div(gSinLut[0x40] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(-gSinLut[0] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(gSinLut[0] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(gSinLut[0x40] << 4, u != 0x110 ? 0x110 - u : 2));

        for (i = 0; i < 4; i++)
        {
            if (i == gUnknown_03005964)
                DrawOamObject(0x83 + i, 8 + i * 24, 0x28 + i * 6, 0, 4);
            else
                DrawOamObject(0x83 + i, 0x400 | (8 + i * 24), 0x100 | (0x28 + i * 6), 0, 4);
        }

        for (i = 0; i < 4; i++)
        {
            if (i == gUnknown_03005964 - 4)
                DrawOamObject(0x87 + i, 0x10 + i * 24, 0x4A + i * 6, 0, 4);
            else
                DrawOamObject(0x87 + i, 0x400 | (0x10 + i * 24), 0x100 | (0x4A + i * 6), 0, 4);
        }

        if (gUnknown_03005964 <= 3)
        {
            DrawOamObject(0x8B, ((gUnknown_03005964 * 24 - 0x10) & 0x1FF) | 0x200,
                         ((gUnknown_03005964 * 6 + 0x10) & 0xFF) | 0x300, 0, 5);
            if (gUnknown_03005964 <= 1)
                DrawOamObject(0x4F, ((gUnknown_03005964 * 24 + 0x20) & 0x1FF) | 0x600,
                             ((gUnknown_03005964 * 6 + 0x28) & 0xFF) | 0x100, 0, 0);
            else
                DrawOamObject(0x4F, ((gUnknown_03005964 * 24 - 0x20) & 0x1FF) | 0x600,
                             ((gUnknown_03005964 * 6 + 0x28) & 0xFF) | 0x100, 0, 0);
        }
        else
        {
            DrawOamObject(0x8B, (((gUnknown_03005964 - 4) * 24 - 8) & 0x1FF) | 0x200,
                         (((gUnknown_03005964 - 4) * 6 + 0x32) & 0xFF) | 0x300, 0, 5);
            if (gUnknown_03005964 <= 5)
                DrawOamObject(0x4F, (((gUnknown_03005964 - 4) * 24 + 0x28) & 0x1FF) | 0x600,
                             (((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF) | 0x100, 0, 0);
            else
                DrawOamObject(0x4F, (((gUnknown_03005964 - 4) * 24 - 0x18) & 0x1FF) | 0x600,
                             (((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF) | 0x100, 0, 0);
        }

        p->unk48++;
    }
    else
    {
        gUnknown_03005908 = 2;
        CoDesignEditor_DrawConfirm((struct Unk080895E4Proc *)p);
    }
}
asm(".global sub_08089C14\n.thumb_set sub_08089C14, CoDesignEditor_DrawPickerToConfirm\n");

void CoDesignEditor_DrawConfirmToPicker(struct Unk08089F90 *p)
{
    s32 i;
    s32 v;
    s32 u;

    if (DivRem(p->unk48, 24) <= 11)
        v = DivRem(p->unk48, 24);
    else
        v = 24 - DivRem(p->unk48, 24);

    v = v * Div(0xC0, 24) + 0x100;

    SetObjAffine(1,
                 Div(gSinLut[0x40] << 4, v != 0 ? v : 2),
                 Div(-gSinLut[0] << 4, v != 0 ? v : 2),
                 Div(gSinLut[0] << 4, v != 0 ? v : 2),
                 Div(gSinLut[0x40] << 4, v != 0 ? v : 2));

    if (p->unk4C <= 15)
    {
        u = Interpolate(1, 0x100, 0x10, p->unk4C, 0x10);

        SetObjAffine(2,
                     Div(gSinLut[0x40] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(-gSinLut[0] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(gSinLut[0] << 4, u != 0x110 ? 0x110 - u : 2),
                     Div(gSinLut[0x40] << 4, u != 0x110 ? 0x110 - u : 2));

        SetObjAffine(3,
                     Div(gSinLut[0x40] << 4, u != 0 ? u : 2),
                     Div(-gSinLut[0] << 4, u != 0 ? u : 2),
                     Div(gSinLut[0] << 4, u != 0 ? u : 2),
                     Div(gSinLut[0x40] << 4, u != 0 ? u : 2));

        for (i = 0; i < 4; i++)
        {
            if (i == gUnknown_03005964)
                DrawOamObject(0x83 + i, 8 + i * 24, 0x28 + i * 6, 0, 4);
            else
                DrawOamObject(0x83 + i, 0x400 | (8 + i * 24), 0x100 | (0x28 + i * 6), 0, 4);
        }

        for (i = 0; i < 4; i++)
        {
            if (i == gUnknown_03005964 - 4)
                DrawOamObject(0x87 + i, 0x10 + i * 24, 0x4A + i * 6, 0, 4);
            else
                DrawOamObject(0x87 + i, 0x400 | (0x10 + i * 24), 0x100 | (0x4A + i * 6), 0, 4);
        }

        if (gUnknown_03005964 <= 3)
        {
            DrawOamObject(0x8B, ((gUnknown_03005964 * 24 - 0x10) & 0x1FF) | 0x200,
                         ((gUnknown_03005964 * 6 + 0x10) & 0xFF) | 0x300, 0, 5);
            if (gUnknown_03005964 <= 1)
                DrawOamObject(0x4F, ((gUnknown_03005964 * 24 + 0x20) & 0x1FF) | 0x600,
                             ((gUnknown_03005964 * 6 + 0x28) & 0xFF) | 0x100, 0, 0);
            else
                DrawOamObject(0x4F, ((gUnknown_03005964 * 24 - 0x20) & 0x1FF) | 0x600,
                             ((gUnknown_03005964 * 6 + 0x28) & 0xFF) | 0x100, 0, 0);
        }
        else
        {
            DrawOamObject(0x8B, (((gUnknown_03005964 - 4) * 24 - 8) & 0x1FF) | 0x200,
                         (((gUnknown_03005964 - 4) * 6 + 0x32) & 0xFF) | 0x300, 0, 5);
            if (gUnknown_03005964 <= 5)
                DrawOamObject(0x4F, (((gUnknown_03005964 - 4) * 24 + 0x28) & 0x1FF) | 0x600,
                             (((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF) | 0x100, 0, 0);
            else
                DrawOamObject(0x4F, (((gUnknown_03005964 - 4) * 24 - 0x18) & 0x1FF) | 0x600,
                             (((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF) | 0x100, 0, 0);
        }

        p->unk48++;
    }
    else
    {
        gUnknown_03005908 = 1;
        CoDesignEditor_DrawPicker(p);
    }
}
asm(".global sub_08089F90\n.thumb_set sub_08089F90, CoDesignEditor_DrawConfirmToPicker\n");
