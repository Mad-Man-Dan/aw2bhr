#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080133B8.
 * sub_080133B8 @ 0x080133B8
 */

#include "proc.h"
/* Steps one entry of a {u16,u16} command list held at +0x4c, publishing the
 * pair into the gUnknown_030030D0 / gUnknown_03002B20 scroll-origin globals.
 * 0x8000 is the list terminator: it rewinds the cursor and breaks the proc if
 * the loop count at +0x44 is -1 (play once), otherwise falls through and
 * replays from the top.
 *
 * `cmds` IS A REAL LOCAL, not punctuation: the ROM loads +0x4c ONCE, before
 * the cursor, and keeps it in a callee-saved register across the whole
 * function. Reading proc->unk4c at each use reloads it in both blocks and
 * costs the r5 push.
 */
struct Unk80133B8Cmd
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
};
struct Unk80133B8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x44);
    /* 44 */ s16 unk44;
    /* 46 */ STRUCT_PAD(0x46, 0x4c);
    /* 4c */ const struct Unk80133B8Cmd *unk4c;
    /* 50 */ STRUCT_PAD(0x50, 0x54);
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

void ScreenShake_Loop(struct Unk80133B8Proc *proc)
{
    const struct Unk80133B8Cmd *cmds = proc->unk4c;

    if (cmds[proc->unk54].unk00 == 0x8000)
    {
        proc->unk54 = 0;

        if (proc->unk44 == -1)
        {
            Proc_Break(proc);
            return;
        }
    }

    gUnknown_030030D0 = cmds[proc->unk54].unk00;
    gUnknown_03002B20 = cmds[proc->unk54].unk02;

    proc->unk54++;
    proc->unk58++;

    if (proc->unk58 == proc->unk44)
        Proc_Break(proc);
}
asm(".global sub_080133B8\n.thumb_set sub_080133B8, ScreenShake_Loop\n");
