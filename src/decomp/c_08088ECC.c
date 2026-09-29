#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08088ECC.
 * CoDesignEditor_DrawBrowse @ 0x08088ECC, CoDesignEditor_DrawPicker @ 0x08089464
 */

/* Matched wave 54 (W54-B), 1432 bytes, first attempt, from sub_080880BC's
 * promoted body in src/decomp/c_08088044.c -- same subsystem, same globals,
 * same sub_08043FD8/PutSprite/Interpolate vocabulary.

 * The one thing worth knowing before reading it: the four sprite blocks are
 * near-copies that differ in exactly two ways, and both differences are real
 * source, not codegen. (1) Which of the two sprites gets its Interpolate
 * WRAPPED in a second Interpolate keyed on proc->unk4c -- 0x48 when unk4e < 0,
 * 0x49 otherwise, and NEITHER in the unk4e == 0 && unk60 == 0 block. (2) The
 * "4"-variant reads of proc->unk48 are `(s16)` casts (`ldrsh`) in three of the
 * four blocks and plain u16 reads (`ldrh`) in the fourth. That looks like a
 * transcription error and is not: the fourth block really does have four
 * `ldrh` in a row where the others alternate ldrh/ldrsh. Left as written.

 * `gUnknown_03005944 > 1` is cast to int here for the same reason as in
 * sub_0808844C -- see the header note on that global. */
#include "global.h"
struct Unk08088ECC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 filler_40[0x08];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x08];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 filler_5c[0x04];
    /* 0x60 */ int unk60;
};
#include "hardware.h"
struct Unk08089464Proc
{
    /* 0x00 */ u8 filler_00[0x48];
    /* 0x48 */ u16 unk48;
};

void CoDesignEditor_DrawBrowse(void *arg)
{
    struct Unk08088ECC *proc = arg;

    if (proc->unk4e != 0)
    {
        sub_08043FD8((0x88 - proc->unk38 * 3) & 0x1FF,
                     ((proc->unk38 + 0x3C) & 0xFF) | 0x400, 0x7A34, 4);
        sub_08043FD8((0xD0 - proc->unk38 * 3) & 0x1FF,
                     ((proc->unk38 + 0x24) & 0xFF) | 0x400, 0x8A58, 4);

        if (proc->unk4e > 0)
        {
            sub_08043FD8((0x40 - proc->unk2c * 3) & 0x1FF,
                         ((proc->unk2c + 0x54) & 0xFF) | 0x400, 0x6A10, 4);
            sub_08043FD8((0x118 - proc->unk38 * 3) & 0x1FF,
                         ((proc->unk38 + 0xC) & 0xFF) | 0x400, 0x59EC, 4);
        }
        else if (proc->unk4e < 0)
        {
            sub_08043FD8((0x40 - proc->unk38 * 3) & 0x1FF,
                         ((proc->unk38 + 0x54) & 0xFF) | 0x400, 0x6A10, 4);
            sub_08043FD8((-8 - (proc->unk2c + 5) * 3) & 0x1FF,
                         ((proc->unk2c + 0x71) & 0xFF) | 0x400, 0x59EC, 4);
        }

        gUnknown_03002F18 = 0x24 - DivRem(proc->unk3c, 4);
        gUnknown_03002B34 = DivRem(proc->unk3c, 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        if (proc->unk4e < 0)
        {
            DrawOamObject(0x48,
                0x18 - Interpolate(4, Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20), 0x10, proc->unk4c, 0x10),
                Interpolate(4, Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20), 8, proc->unk4c, 0x10) + 0x3C,
                0, 0);
            DrawOamObject(0x49,
                Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20) + 0x58,
                0x2C - Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20),
                0, 0);
        }
        else
        {
            DrawOamObject(0x48,
                0x18 - Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20),
                Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20) + 0x3C,
                0, 0);
            DrawOamObject(0x49,
                Interpolate(4, Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20), 0x10, proc->unk4c, 0x10) + 0x58,
                0x2C - Interpolate(4, Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20), 8, proc->unk4c, 0x10),
                0, 0);
        }
    }
    else if (proc->unk60 != 0)
    {
        sub_08043FD8(0x40, ((0x54 - proc->unk30) & 0xFF) | 0x400, 0x2980, 4);
        sub_08043FD8(0x88, ((0x3C - proc->unk30) & 0xFF) | 0x400, 0x39A4, 4);
        sub_08043FD8(0xD0, ((0x24 - proc->unk30) & 0xFF) | 0x400, 0x49C8, 4);

        gUnknown_03002F18 = proc->unk30 + 0x24 - DivRem(Div(proc->unk3c, 4), 4);
        gUnknown_03002B34 = DivRem(Div(proc->unk3c, 4), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        if (proc->unk60 < 0)
        {
            DrawOamObject(0x48,
                0x18 - Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20),
                Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20) + 0x3C,
                0, 0);
            DrawOamObject(0x49,
                Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20) + 0x58,
                0x2C - Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20),
                0, 0);
        }
        else
        {
            DrawOamObject(0x48,
                0x18 - Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20),
                Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20) + 0x3C,
                0, 0);
            DrawOamObject(0x49,
                Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20) + 0x58,
                0x2C - Interpolate(4, 0, 4, DivRem((s16)proc->unk48, 0x20), 0x20),
                0, 0);
        }
    }
    else
    {
        sub_08043FD8(0x40, 0x54, 0x2980, 4);
        sub_08043FD8(0x88, 0x43C, 0x39A4, 4);
        sub_08043FD8(0xD0, 0x424, 0x49C8, 4);

        gUnknown_03002F18 = 0x24 - DivRem(Div(proc->unk3c, 3), 4);
        gUnknown_03002B34 = DivRem(Div(proc->unk3c, 3), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        DrawOamObject(0x48,
            0x18 - Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20),
            Interpolate(4, 0, 4, DivRem(proc->unk48, 0x20), 0x20) + 0x3C,
            0, 0);
        DrawOamObject(0x49,
            Interpolate(4, 0, 8, DivRem(proc->unk48, 0x20), 0x20) + 0x58,
            0x2C - Interpolate(4, 0, 4, DivRem(proc->unk48, 0x20), 0x20),
            0, 0);
    }

    if ((int)gUnknown_03005944 > 1)
    {
        DrawOamObject(0x43, 0x38, 0x1C, 0, 0);
        DrawOamObject(0x44, 0x38, 0x54, 0, 0);
    }

    sub_08043C28(proc->unk34 + 0xB0, 0xA0, 0x1040, 2, 1);
    PutSprite(1, 0x20, 0x1C, gUnknown_0848B6A0, 0);
    DrawOamObject(gUnknown_03005958[proc->unk58] + 0x3E, 0x20, 0x78, 0, 0);
    sub_08043B60(0x30, 0x78, 0xA2CC, 3);
}
asm(".global sub_08088ECC\n.thumb_set sub_08088ECC, CoDesignEditor_DrawBrowse\n");

void CoDesignEditor_DrawPicker(void *arg)
{
    struct Unk08089464Proc *proc = arg;
    int t;
    int scale;
    int i;

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

    for (i = 0; i < 4; i++)
        DrawOamObject(i + 0x83, i * 0x18 + 8, i * 6 + 0x28, 0, 4);

    for (i = 0; i < 4; i++)
        DrawOamObject(i + 0x87, i * 0x18 + 0x10, i * 6 + 0x4A, 0, 4);

    if (gUnknown_03005964 <= 3)
        DrawOamObject(0x8B,
                     ((gUnknown_03005964 * 0x18 - 0x10) & 0x1FF) | 0x200,
                     ((gUnknown_03005964 * 6 + 0x10) & 0xFF) | 0x300,
                     0, 5);
    else
        DrawOamObject(0x8B,
                     (((gUnknown_03005964 - 4) * 0x18 - 8) & 0x1FF) | 0x200,
                     (((gUnknown_03005964 - 4) * 6 + 0x32) & 0xFF) | 0x300,
                     0, 5);

    proc->unk48++;
}
asm(".global sub_08089464\n.thumb_set sub_08089464, CoDesignEditor_DrawPicker\n");
