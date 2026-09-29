#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801137C.
 * sub_0801137C @ 0x0801137C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk801137CProc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

/* sub_08011298's twin: Proc_End instead of sub_080153B8 on the already-in-state
 * path, and the cursor seeded at 0x140 instead of 0. */
void WipeFromBlack_Init(struct Unk801137CProc *proc)
{
    if (gUnknown_03002B5C == 0)
    {
        Proc_End(proc);
        return;
    }

    gUnknown_03002B5C = 0;
    proc->unk64 = 1;
    SetupWipeWindow();
    SetWin0FullScreen();
    gUnknown_03001408 = 0x140;
    gUnknown_03002F3C = 0;
    StartFadeScreenLines(gUnknown_0200B274, (void *)0x04000040, 1, 0xA240, FillWipeScanlineTable);
}

asm(".global sub_0801137C\n.thumb_set sub_0801137C, WipeFromBlack_Init\n");
