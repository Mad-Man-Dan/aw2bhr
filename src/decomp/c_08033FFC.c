#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033FFC.
 * sub_08033FFC @ 0x08033FFC, sub_0803405C @ 0x0803405C, sub_08034130 @ 0x08034130
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk33FFCProc
{
    /* 0x00 */ u8 filler_00[0x36];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 filler_37[0x15];
    /* 0x4c */ u32 unk4c;
};
struct Unk3405CProc
{
    /* 0x00 */ u8 filler_00[0x36];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 filler_37[0x15];
    /* 0x4c */ u32 unk4c;
};
struct Unk34130Child
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u8 unk30[0x03];
    /* 0x33 */ u8 filler_33[0x03];
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
};
struct Unk34130Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ struct Unk34130Child *unk2c;
};

void LinkC3_IDLE_08033FFD(struct Unk33FFCProc *proc)
{
    PutSpriteExt(0, 0x60, gUnknown_0849BC38[2],
        gUnknown_0849BC18[proc->unk36], (proc->unk36 + 1) << 12);

    if (proc->unk4c > 0x78 || (gpKeySt->pressed & 1))
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }
    else
    {
        proc->unk4c++;
    }
}

void LinkC3_IDLE_0803405D(struct Unk3405CProc *proc)
{
    int t = Interpolate(0, 0x100, 0x10, proc->unk4c, 0x10);

    SetObjAffine(0xc,
        Div(COS_Q12(0) * 16, 0x100),
        Div(-SIN_Q12(0) * 16, t),
        Div(SIN_Q12(0) * 16, 0x100),
        Div(COS_Q12(0) * 16, t));

    PutSpriteExt(0, 0x1860, gUnknown_0849BC38[2] | 0x100,
        gUnknown_0849BC18[proc->unk36], (proc->unk36 + 1) << 12);

    if (proc->unk4c > 0xf)
        Proc_Break(proc);
    else
        proc->unk4c++;
}

void LinkC2_IDLE_08034131(struct Unk34130Proc *proc)
{
    struct Unk34130Child *child = proc->unk2c;
    u8 n = child->unk36;

    if (gpKeySt->pressed & 2)
    {
        UnlockMainMenu();
        PlayMusicOrSfx2(0x66);
        Proc_Goto(proc, 2);
    }
    else if (child->unk37 == 0)
    {
        if (gpKeySt->pressed & 9)
        {
            gUnknown_0849B060->unk0d = n;
            Proc_EndEach(gUnknown_0849BB50);
            PlayMusicOrSfx2(0x71);
            Proc_Break(child);
            Proc_Break(proc);
        }
        else
        {
            if (gpKeySt->repeated & DPAD_UP)
            {
                if (n != 0)
                    n = n - 1;
                else
                    n = 2;
            }
            else if (gpKeySt->repeated & DPAD_DOWN)
            {
                if (n <= 1)
                    n = n + 1;
                else
                    n = 0;
            }

            if (n != child->unk36)
            {
                child->unk30[child->unk36] = 0;
                child->unk36 = n;
                child->unk30[child->unk36] = 1;
                child->unk37 = 1;

                LinkC4_SetCursorRow(n);
                PlayMusicOrSfx2(0x64);
                Proc_Goto(child, 0);
            }
        }
    }
}

asm(".global sub_08033FFC\n.thumb_set sub_08033FFC, LinkC3_IDLE_08033FFD\n"
    ".global sub_0803405C\n.thumb_set sub_0803405C, LinkC3_IDLE_0803405D\n"
    ".global sub_08034130\n.thumb_set sub_08034130, LinkC2_IDLE_08034131\n");
