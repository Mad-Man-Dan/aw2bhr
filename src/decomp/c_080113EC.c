#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080113EC.
 * sub_080113EC @ 0x080113EC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk80113EC
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void WipeToBlack_Loop(struct Unk80113EC *proc)
{
    gUnknown_03002F3C += proc->unk64;

    if (gUnknown_03002F3C > 0x13f)
        gUnknown_03002F3C = 0x140;

    if (proc->unk64 <= 8)
        gUnknown_03001408 = Interpolate(0, 0, 0x140, gUnknown_03002F3C, 0x140);
    else
        gUnknown_03001408 = Interpolate(4, 0, 0x140, gUnknown_03002F3C, 0x140);

    if (gUnknown_03001408 > 0x13f)
    {
        gUnknown_03001408 = 0x140;
        SetupWipeWindow();
        SetWin0FullScreen();
        Proc_Break(proc);
        return;
    }

    if (gUnknown_03001408 > 0x13f)
        gUnknown_03001408 = 0x140;
}

asm(".global sub_080113EC\n.thumb_set sub_080113EC, WipeToBlack_Loop\n");
