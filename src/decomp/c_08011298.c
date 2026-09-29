#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011298.
 * sub_08011298 @ 0x08011298
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk8011298Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void WipeToBlack_Init(struct Unk8011298Proc *proc)
{
    if (gUnknown_03002B5C == 1)
    {
        sub_080153B8((struct Unk03001470 *)proc);
        return;
    }

    gUnknown_03002B5C = 1;
    proc->unk64 = 1;
    SetupWipeWindow();
    SetWin0FullScreen();
    gUnknown_03001408 = 0;
    gUnknown_03002F3C = 0;
    StartFadeScreenLines(gUnknown_0200B274, (void *)0x04000040, 1, 0xA240, FillWipeScanlineTable);
}

asm(".global sub_08011298\n.thumb_set sub_08011298, WipeToBlack_Init\n");
