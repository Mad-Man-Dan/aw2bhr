#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080895E4.
 * sub_080895E4 @ 0x080895E4
 */

#include "hardware.h"
struct Unk080895E4Proc
{
    /* 0x00 */ u8 filler_00[0x48];
    /* 0x48 */ u16 unk48;
};

void CoDesignEditor_DrawConfirm(struct Unk080895E4Proc *proc)
{
    int t;
    int scale;

    if (DivRem(proc->unk48, 0x18) <= 0xB)
        t = DivRem(proc->unk48, 0x18);
    else
        t = 0x18 - DivRem(proc->unk48, 0x18);

    scale = t * Div(0xC0, 0x18) + 0x100;

    SetObjAffine(1,
                 Div(COS_Q12(0) * 16, scale != 0 ? scale : 2),
                 Div(-SIN_Q12(0) * 16, scale != 0 ? scale : 2),
                 Div(SIN_Q12(0) * 16, scale != 0 ? scale : 2),
                 Div(COS_Q12(0) * 16, scale != 0 ? scale : 2));

    if (gUnknown_03005964 <= 3)
    {
        DrawOamObject(gUnknown_03005964 + 0x83,
                     gUnknown_03005964 * 0x18 + 8,
                     gUnknown_03005964 * 6 + 0x28,
                     0, 4);
        DrawOamObject(0x8B,
                     ((gUnknown_03005964 * 0x18 - 0x10) & 0x1FF) | 0x200,
                     ((gUnknown_03005964 * 6 + 0x10) & 0xFF) | 0x300,
                     0, 5);

        if (gUnknown_03005964 <= 1)
            DrawOamObject(0x4F,
                         (gUnknown_03005964 * 0x18 + 0x20) & 0x1FF,
                         (gUnknown_03005964 * 6 + 0x28) & 0xFF,
                         0, 0);
        else
            DrawOamObject(0x4F,
                         (gUnknown_03005964 * 0x18 - 0x20) & 0x1FF,
                         (gUnknown_03005964 * 6 + 0x28) & 0xFF,
                         0, 0);
    }
    else
    {
        DrawOamObject(gUnknown_03005964 + 0x83,
                     (gUnknown_03005964 - 4) * 0x18 + 0x10,
                     (gUnknown_03005964 - 4) * 6 + 0x4A,
                     0, 4);
        DrawOamObject(0x8B,
                     (((gUnknown_03005964 - 4) * 0x18 - 8) & 0x1FF) | 0x200,
                     (((gUnknown_03005964 - 4) * 6 + 0x32) & 0xFF) | 0x300,
                     0, 5);

        if (gUnknown_03005964 <= 5)
            DrawOamObject(0x4F,
                         ((gUnknown_03005964 - 4) * 0x18 + 0x28) & 0x1FF,
                         ((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF,
                         0, 0);
        else
            DrawOamObject(0x4F,
                         ((gUnknown_03005964 - 4) * 0x18 - 0x18) & 0x1FF,
                         ((gUnknown_03005964 - 4) * 6 + 0x4A) & 0xFF,
                         0, 0);
    }

    proc->unk48++;
}
asm(".global sub_080895E4\n.thumb_set sub_080895E4, CoDesignEditor_DrawConfirm\n");
