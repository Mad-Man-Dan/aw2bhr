#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067690.
 * sub_08067690 @ 0x08067690
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
struct Unk08067690Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ const s8 *unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
};

void IntroT3_Child_IDLE_08067691(struct Unk08067690Proc *proc)
{
    int f;

    f = 0;

    if (proc->unk30 == 0)
    {
        proc->unk38 += 3;
        proc->unk34 = 2;
        proc->unk30 = proc->unk2c[proc->unk38];

        if (proc->unk30 == 0)
        {
            proc->unk38 = f;
            proc->unk30 = proc->unk2c[0];
        }
    }

    if (proc->unk34 != 0)
    {
        proc->unk34--;
        gDispIo.disp_ct.bg0_enable = 0;
        gDispIo.disp_ct.bg1_enable = 0;
        gDispIo.disp_ct.bg2_enable = 1;
        gDispIo.disp_ct.bg3_enable = 1;
        gDispIo.disp_ct.obj_enable = 1;
        return;
    }

    if (proc->unk38 & 1)
    {
        f = 0;
        gDispIo.disp_ct.bg0_enable = 1;
        gDispIo.disp_ct.bg1_enable = 0;
        gDispIo.disp_ct.bg2_enable = 1;
        gDispIo.disp_ct.bg3_enable = 1;
        gDispIo.disp_ct.obj_enable = 1;
    }
    else
    {
        f = 1;
        gDispIo.disp_ct.bg0_enable = 0;
        gDispIo.disp_ct.bg1_enable = 1;
        gDispIo.disp_ct.bg2_enable = 1;
        gDispIo.disp_ct.bg3_enable = 1;
        gDispIo.disp_ct.obj_enable = 1;
    }

    SetBgScrollShadow(f, proc->unk2c[proc->unk38 + 1], proc->unk2c[proc->unk38 + 2]);

    proc->unk30--;
}

asm(".global sub_08067690\n.thumb_set sub_08067690, IntroT3_Child_IDLE_08067691\n");
