#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08088044.
 * sub_08088044 @ 0x08088044, sub_080880BC @ 0x080880BC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk08088044
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ u8 filler_3c[0x0c];
    /* 0x48 */ s16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ s16 unk68;
};
#include "hardware.h"
struct Unk80880BC
{
    /* 00 */ u8 filler0[0x30];
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3C */ int unk3c;
    /* 40 */ u8 filler40[0xC];
    /* 4C */ s16 unk4c;
    /* 4E */ u8 filler4e[0xA];
    /* 58 */ int unk58;
};
/* The interpolated affine scale. Interpolate is called TWICE per use in the
 * ROM, so the guard against a zero divisor cannot have been a temporary --
 * this is a macro whose argument is evaluated twice. */
#define SCALE(t, d) (Interpolate(0, 0x10, 0x100, (t), (d)) ? Interpolate(0, 0x10, 0x100, (t), (d)) : 2)

void CoDesignEditor_Init(struct Unk08088044 *proc)
{
    int i;

    sub_08043B44(0xa);

    proc->unk4c = 0;
    proc->unk34 = 0x78;
    proc->unk38 = 0;
    proc->unk52 = 0;
    proc->unk4e = 0;
    proc->unk58 = 0;
    proc->unk5c = 0;
    proc->unk60 = 0;
    proc->unk2c = 0;
    proc->unk30 = 0;
    proc->unk48 = 0;
    proc->unk68 = 4;
    proc->unk64 = 0;

    for (i = 0; i < 5; i++)
        gUnknown_03005978[i] = 0;

    StartHeaderBanner(gUnknown_085802C0, gUnknown_0200FC50, 0x27c, 9, 0, 0, (int)proc);
}

void CoDesignEditor_IntroLoop(struct Unk80880BC * proc)
{
    if (proc->unk4c <= 0x18)
    {
        proc->unk30 = Interpolate(5, -0xCC, 0x24, proc->unk4c, 0x18);
        gUnknown_03002F18 = proc->unk30 - DivRem(Div(proc->unk3c, 3), 4);
        gUnknown_03002B34 = DivRem(Div(proc->unk3c, 3), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        proc->unk34 = Interpolate(5, 0x78, 0, proc->unk4c, 0x18);

        if (proc->unk4c >= 0xF && proc->unk4c <= 0x12)
        {
            DrawWindowBackgroundOnBg2(0x15 - proc->unk4c, 0xE, (proc->unk4c - 0xD) * 2, 4);
            CoDesignEditor_SetupBlend();
        }
    }
    else
    {
        Proc_Break(proc);
        gUnknown_03001FE8.bits.size = 1;
        sub_0808A3DC();
    }

    if (proc->unk4c > 0x11)
    {
        SetObjAffine(0,
            Div(COS_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 8)),
            Div(-SIN_Q12(0) * 0x10, 0x100),
            Div(SIN_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 8)),
            Div(COS_Q12(0) * 0x10, 0x100));

        sub_08043B60(0x30, 0x178, 0xA2CC, 3);

        if (proc->unk4c <= 0x15)
        {
            SetObjAffine(1,
                Div(COS_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 4)),
                Div(-SIN_Q12(0) * 0x10, 0x100),
                Div(SIN_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 4)),
                Div(COS_Q12(0) * 0x10, 0x100));
        }

        DrawOamObject(gUnknown_03005958[proc->unk58] + 0x3E, 0x220, 0x178, 0, 0);
    }

    proc->unk4c++;

    if ((s16)(0x78 - proc->unk30) <= 0xD0)
        sub_08043FD8(0x40, (0x78 - proc->unk30) & 0xFF, 0x2980, 4);

    if ((s16)(0x60 - proc->unk30) <= 0xD0)
        sub_08043FD8(0x88, ((0x60 - proc->unk30) & 0xFF) | 0x400, 0x39A4, 4);

    if ((s16)(0x48 - proc->unk30) <= 0xD0)
        sub_08043FD8(0xD0, ((0x48 - proc->unk30) & 0xFF) | 0x400, 0x49C8, 4);

    if ((s16)(0x40 - proc->unk30) <= 0xD0)
        PutSprite(1, 0x20, 0x40 - proc->unk30, gUnknown_0848B6A0, 0);

    sub_08043C28(proc->unk34 + 0xB0, 0xA0, 0x1040, 2, 1);
    proc->unk3c--;
}

asm(".global sub_08088044\n.thumb_set sub_08088044, CoDesignEditor_Init\n"
    ".global sub_080880BC\n.thumb_set sub_080880BC, CoDesignEditor_IntroLoop\n");

extern void CoDesignEditor_Loop(void);

struct ProcCmd CONST_DATA ProcScr_CoDesignC2[] =
{
    PROC_CALL(CoDesignEditor_Init),
    PROC_SLEEP(64),
    PROC_REPEAT(CoDesignEditor_IntroLoop),
    PROC_REPEAT(CoDesignEditor_Loop),
    PROC_END,
};

asm(".global gUnknown_08616EB4\n.set gUnknown_08616EB4, ProcScr_CoDesignC2\n");
