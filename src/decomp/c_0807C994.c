#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C994.
 * sub_0807C994 @ 0x0807C994, sub_0807C9EC @ 0x0807C9EC, sub_0807CAFC @ 0x0807CAFC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk807C994
{
    /* 0x00 */ u8 filler_00[0x48];
    /* 0x48 */ u16 unk48;
};
struct Unk0807C9EC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ u8 filler_3c[0x40 - 0x3c];
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[0x48 - 0x44];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[0x4c - 0x4a];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u16 unk4e;
    /* 0x50 */ u8 filler_50[0x52 - 0x50];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x58 - 0x54];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u16 unk66;
    /* 0x68 */ u16 unk68;
    /* 0x6a */ u16 unk6a;
};
struct Unk807CAFC
{
    /* 00 */ u8 filler0[0x30];
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3C */ int unk3c;
    /* 40 */ u8 filler40[0xC];
    /* 4C */ s16 unk4c;
    /* 4E */ u8 filler4e[0x16];
    /* 64 */ s16 unk64;
};
/* The interpolated affine scale. Interpolate is called TWICE per use in the
 * ROM, so the guard against a zero divisor cannot have been a temporary --
 * this is a macro whose argument is evaluated twice. */
#define SCALE(t, d) (Interpolate(0, 0x10, 0x100, (t), (d)) ? Interpolate(0, 0x10, 0x100, (t), (d)) : 2)

void WarRoomMapSelected_IDLE_0807C995(struct Unk807C994 *proc)
{
    int i;

    i = DivRem(Div(proc->unk48, 4), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_08234AF0 + i), 0x218, 2);

    i = DivRem(Div(proc->unk48, 4), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_08239F84 + i), 0x3B8, 2);

    proc->unk48++;
}

void CoSelect_0807C9ED(struct Unk0807C9EC *proc)
{
    sub_08043B44(9);

    proc->unk4c = 0;
    proc->unk34 = 0x78;
    proc->unk38 = 0;
    proc->unk2c = 0;
    proc->unk30 = 0;

    if ((u8)IsBlockWarRoomSelectionActive() != 0)
    {
        struct Unk0807C9EC *src = Proc_Find(gUnknown_086165C0);

        proc->unk52 = src->unk52;
        proc->unk4e = src->unk4e;
        proc->unk58 = src->unk58;
        proc->unk5c = src->unk5c;
        proc->unk60 = src->unk60;
        proc->unk64 = src->unk64;
        proc->unk66 = src->unk66;
        proc->unk40 = src->unk40;
        proc->unk48 = src->unk48;
        proc->unk68 = src->unk68;
        proc->unk6a = src->unk6a;
    }
    else
    {
        int i;

        proc->unk52 = 0;
        proc->unk4e = 0;
        proc->unk58 = 0;
        proc->unk5c = 0;
        proc->unk60 = 0;
        proc->unk64 = 0;
        proc->unk66 = 0;
        proc->unk40 = 0;
        proc->unk48 = 0;
        proc->unk68 = 4;
        proc->unk6a = 1;

        for (i = 0; i <= 4; i++)
            gUnknown_03005950[i] = 0;
    }

    StartHeaderBanner(gUnknown_085802D8 + 0xC, gUnknown_0200FC50, 0x2DC, 0xB, 0, 1, (int)proc);
}

void CoSelect_IDLE_0807CAFD(struct Unk807CAFC * proc)
{
    int i;

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

        if (proc->unk4c >= 0xC && proc->unk4c <= 0x12)
        {
            DrawWindowBackgroundOnBg2(0x13 - proc->unk4c, 0xC, proc->unk4c * 2 - 0x15, sub_0803BD14() * 2 + 2);
            CoSelect_SetupBlend();
        }
    }
    else
    {
        Proc_Break(proc);
        gUnknown_03001FE8.bits.size = 1;
    }

    if (proc->unk4c > 0x11)
    {
        SetObjAffine(0,
            Div(COS_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 8)),
            Div(-SIN_Q12(0) * 0x10, 0x100),
            Div(SIN_Q12(0) * 0x10, SCALE(proc->unk4c - 0x11, 8)),
            Div(COS_Q12(0) * 0x10, 0x100));

        for (i = 0; i < proc->unk64; i++)
        {
            sub_08043B60(0x2A, 0x100 | (0x68 + i * 0x10), 0x9000 | (0x3C0 + i * 0xC), 0);

            if (gUnknown_030059C0[i] != 0)
                DrawOamObject(gUnknown_03005958[gUnknown_0300599C[i]] + 0x3E, 0x14, 0x68 + i * 0x10, 0, 2);
            else
                DrawOamObject(gUnknown_03005958[i] + 0x3E, 0x14, 0x68 + i * 0x10, 0, 2);
        }

        DrawOamObject(0x54, 0x5C, 0x68, 0, 0);
        DrawOamObject(sub_0803BD14() + 0x55, 0x65, 0x70, 0, 0);
        DrawOamObject(0x5F, 0x6D, 0x70, 0, 0);
    }

    proc->unk4c++;

    if ((s16)(0x78 - proc->unk30) <= 0xD0)
        sub_08043FD8(0x40, (0x78 - proc->unk30) & 0xFF, 0x2980, 5);

    if ((s16)(0x60 - proc->unk30) <= 0xD0)
        sub_08043FD8(0x88, ((0x60 - proc->unk30) & 0xFF) | 0x400, 0x39A4, 5);

    if ((s16)(0x48 - proc->unk30) <= 0xD0)
        sub_08043FD8(0xD0, ((0x48 - proc->unk30) & 0xFF) | 0x400, 0x49C8, 5);

    if ((s16)(0x40 - proc->unk30) <= 0xD0)
        PutSprite(0, 0x20, 0x40 - proc->unk30, gUnknown_0848B6A0, 0);

    sub_08043C28(proc->unk34 + 0xB0, 0xA0, 0x1040, 0, 1);
    proc->unk3c--;
}

asm(".global sub_0807C994\n.thumb_set sub_0807C994, WarRoomMapSelected_IDLE_0807C995\n"
    ".global sub_0807C9EC\n.thumb_set sub_0807C9EC, CoSelect_0807C9ED\n"
    ".global sub_0807CAFC\n.thumb_set sub_0807CAFC, CoSelect_IDLE_0807CAFD\n");

extern void CoSelect_IDLE_0807CE5D(void);
extern void sub_0807E980(void);
extern void CoSelect_IDLE_0807ED91(void);
extern void CoSelect_IDLE_0807EEED(void);

struct ProcCmd CONST_DATA ProcScr_CoSelect[] =
{
    PROC_CALL(CoSelect_0807C9ED),
    PROC_REPEAT(CoSelect_IDLE_0807CAFD),
    PROC_REPEAT(CoSelect_IDLE_0807CE5D),
    PROC_REPEAT(sub_0807E980),
    PROC_REPEAT(CoSelect_IDLE_0807ED91),
    PROC_REPEAT(CoSelect_IDLE_0807EEED),
    PROC_1D(30),
    PROC_SLEEP(1),
    PROC_END_EACH((void *)0x086165C0),
    PROC_END_EACH((void *)0x086166C8),
    PROC_END,
};

asm(".global gUnknown_08616638\n.set gUnknown_08616638, ProcScr_CoSelect\n");
