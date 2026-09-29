#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A218.
 * sub_0806A218 @ 0x0806A218
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
struct Unk806A218
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_IDLE_0806A219(struct Unk806A218 *proc)
{
    u32 zero;

    switch (0x128 - proc->unk2c)
    {
    case 0:
        StartIntroCoReveal(0, 1, 0x61, proc);
        break;

    case 0x62:
        StartIntroCoReveal(1, 4, 0x61, proc);
        break;

    case 0xc4:
        StartIntroCoReveal(0, 2, 0x61, proc);
        break;

    case 0x11c:
        zero = 0;
        CpuFastSet(&zero, (void *)0x0600D000, 0x01000400);
        ApplyPaletteExt(gUnknown_0817C3E8, 0, 0x20);
        Decompress(gUnknown_081866F8, (void *)0x06000000);
        Decompress(gUnknown_08186D4C, gBG0TilemapBuffer);
        BG_EnableSyncBG0();
        break;

    case 0x11d:
        gDispIo.disp_ct.bg0_enable = 1;
        gDispIo.disp_ct.bg1_enable = 1;
        gDispIo.disp_ct.bg2_enable = 1;
        gDispIo.disp_ct.bg3_enable = 1;
        gDispIo.disp_ct.obj_enable = 1;
        Proc_Start(gUnknown_085814E8, proc);
        break;
    }

    if (proc->unk2c != 0)
    {
        proc->unk2c--;
    }
    else
    {
        Proc_Break(proc);
        EndIntroBgScroll();
    }
}

asm(".global sub_0806A218\n.thumb_set sub_0806A218, IntroT3_IDLE_0806A219\n");
