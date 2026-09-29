#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080897C8.
 * CoDesignEditor_DrawEnterPicker @ 0x080897C8, CoDesignEditor_DrawLeavePicker @ 0x08089A04
 */

/* Matched wave 54 (W54-B), 572 bytes, three attempts. Twin of CoDesignEditor_DrawLeavePicker,
 * which is the same animation run backwards and matched first try off this
 * derivation. Exemplar was sub_080880BC (src/decomp/c_08088044.c).

 * The three attempts were all one bug: `i * 6 - (proc->unk30 - K)` reassociates
 * to `(i * 6 + K) - proc->unk30` and the K lands in the giv's initialiser, so
 * the ROM's `movs r6,#0` + in-loop `subs r2,#K` becomes `movs r6,#K` and no
 * subs. The inline assignment `(y0 = proc->unk30 - K)` is the barrier, and the
 * four locals MUST be four names -- one shared `y` costs a register and a copy
 * at each of the four sites. Full chapter in docs/agbcc-codegen.md. */
#include "global.h"
#include "hardware.h"
struct Unk080897C8
{
    /* 00 */ u8 filler0[0x30];
    /* 30 */ int unk30;
    /* 34 */ u8 filler34[0x08];
    /* 3C */ int unk3c;
    /* 40 */ u8 filler40[0x08];
    /* 48 */ u16 unk48;
    /* 4A */ u8 filler4a[0x02];
    /* 4C */ s16 unk4c;
};
struct Unk08089A04
{
    /* 00 */ u8 filler0[0x30];
    /* 30 */ int unk30;
    /* 34 */ u8 filler34[0x08];
    /* 3C */ int unk3c;
    /* 40 */ u8 filler40[0x0C];
    /* 4C */ s16 unk4c;
};

void CoDesignEditor_DrawEnterPicker(struct Unk080897C8 *proc)
{
    int i;
    int y0;
    int y1;
    int y2;
    int y3;

    if (proc->unk4c <= 0xF)
    {
        gUnknown_03001FE8.bits.size = 2;

        proc->unk30 = Interpolate(1, 0x24, 0xC2, proc->unk4c, 0x10);
        gUnknown_03002F18 = proc->unk30 - DivRem(Div(proc->unk3c, 3), 4);
        gUnknown_03002B34 = DivRem(Div(proc->unk3c, 3), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        if ((s16)(0x78 - proc->unk30) >= 0)
            sub_08043FD8(0x40, (0x78 - proc->unk30) & 0xFF, 0x2980, 4);

        if ((s16)(0x60 - proc->unk30) >= 0)
            sub_08043FD8(0x88, ((0x60 - proc->unk30) & 0xFF) | 0x400, 0x39A4, 4);

        if ((s16)(0x48 - proc->unk30) >= 0)
            sub_08043FD8(0xD0, ((0x48 - proc->unk30) & 0xFF) | 0x400, 0x49C8, 4);

        if ((s16)(0x80 - proc->unk30) >= 0)
            PutSprite(1, 0x20, 0x40 - proc->unk30, gUnknown_0848B6A0, 0x800);
    }
    else if (proc->unk4c <= 0x1F)
    {
        proc->unk30 = Interpolate(4, -0x9E, 0, proc->unk4c - 0x10, 0x10);
        gUnknown_03002F18 = proc->unk30 + 0x160;
        gUnknown_03002B34 = 0;

        for (i = 0; i < 4; i++)
            DrawOamObject(i + 0x83, i * 0x18 + 8,
                         (i * 6 - (y0 = proc->unk30 - 0x28)) & 0xFF, 0x800, 4);

        for (i = 0; i < 4; i++)
            DrawOamObject(i + 0x87, i * 0x18 + 0x10,
                         (i * 6 - (y1 = proc->unk30 - 0x4A)) & 0xFF, 0x800, 4);

        if (gUnknown_03005964 <= 3)
            DrawOamObject(0x8B,
                         (gUnknown_03005964 * 0x18) & 0x1FF,
                         (gUnknown_03005964 * 6 - (y2 = proc->unk30 - 0x20)) & 0xFF,
                         0x800, 5);
        else
            DrawOamObject(0x8B,
                         ((gUnknown_03005964 - 4) * 0x18 + 8) & 0x1FF,
                         ((gUnknown_03005964 - 4) * 6 - (y3 = proc->unk30 - 0x42)) & 0xFF,
                         0x800, 5);
    }
    else
    {
        gUnknown_03002F18 = 0x160;
        gUnknown_03002B34 = 0;
        gUnknown_03005908 = 1;
        proc->unk48 = 0;
        CoDesignEditor_DrawHelpText();
    }
}
asm(".global sub_080897C8\n.thumb_set sub_080897C8, CoDesignEditor_DrawEnterPicker\n");

void CoDesignEditor_DrawLeavePicker(struct Unk08089A04 *proc)
{
    int i;
    int y0;
    int y1;
    int y2;
    int y3;

    if (proc->unk4c <= 0xF)
    {
        proc->unk30 = Interpolate(1, 0, -0x9E, proc->unk4c, 0x10);
        gUnknown_03002F18 = proc->unk30 + 0x160;

        for (i = 0; i < 4; i++)
            DrawOamObject(i + 0x83, i * 0x18 + 8,
                         (i * 6 - (y0 = proc->unk30 - 0x28)) & 0xFF, 0x800, 4);

        for (i = 0; i < 4; i++)
            DrawOamObject(i + 0x87, i * 0x18 + 0x10,
                         (i * 6 - (y1 = proc->unk30 - 0x4A)) & 0xFF, 0x800, 4);

        if (gUnknown_03005964 <= 3)
            DrawOamObject(0x8B,
                         (gUnknown_03005964 * 0x18) & 0x1FF,
                         (gUnknown_03005964 * 6 - (y2 = proc->unk30 - 0x20)) & 0xFF,
                         0x800, 5);
        else
            DrawOamObject(0x8B,
                         ((gUnknown_03005964 - 4) * 0x18 + 8) & 0x1FF,
                         ((gUnknown_03005964 - 4) * 6 - (y3 = proc->unk30 - 0x42)) & 0xFF,
                         0x800, 5);
    }
    else if (proc->unk4c <= 0x1F)
    {
        proc->unk30 = Interpolate(4, 0xC2, 0x24, proc->unk4c - 0x10, 0x10);
        gUnknown_03002F18 = proc->unk30 - DivRem(Div(proc->unk3c, 3), 4);
        gUnknown_03002B34 = DivRem(Div(proc->unk3c, 3), 4) * 3;

        if ((s16)gUnknown_03002B34 < 0)
        {
            gUnknown_03002F18 -= 4;
            gUnknown_03002B34 += 0xC;
        }

        if ((s16)(0x78 - proc->unk30) >= 0)
            sub_08043FD8(0x40, (0x78 - proc->unk30) & 0xFF, 0x2980, 4);

        if ((s16)(0x60 - proc->unk30) >= 0)
            sub_08043FD8(0x88, ((0x60 - proc->unk30) & 0xFF) | 0x400, 0x39A4, 4);

        if ((s16)(0x48 - proc->unk30) >= 0)
            sub_08043FD8(0xD0, ((0x48 - proc->unk30) & 0xFF) | 0x400, 0x49C8, 4);

        if ((s16)(0x80 - proc->unk30) >= 0)
            PutSprite(1, 0x20, 0x40 - proc->unk30, gUnknown_0848B6A0, 0x800);
    }
    else
    {
        gUnknown_03005908 = 0;
        CoDesignEditor_DrawHelpText();
        gUnknown_03001FE8.bits.size = 1;
        CoDesignEditor_DrawBrowse(proc);
    }
}
asm(".global sub_08089A04\n.thumb_set sub_08089A04, CoDesignEditor_DrawLeavePicker\n");
