#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C0E4.
 * sub_0806C0E4 @ 0x0806C0E4, sub_0806C114 @ 0x0806C114
 */

#include "proc.h"
struct Unk6C0E4Proc
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u8 filler_34[0x04];
    /* 0x38 */ int unk38;
};
struct Unk6C114Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
};

/* gUnknown_0858265C is indexed with `lsls #2` off the BARE symbol -- no load in
 * front of it -- so the symbol is the array itself; contrast gUnknown_08499590,
 * which is a pointer and always shows an `ldr r3, [r1]` first. The word loaded
 * out is then dereferenced at +0x30, so the elements are pointers.
 *
 * `blo` makes both counters unsigned. */
void CreditsPage_HoldLoop(struct Unk6C0E4Proc *proc)
{
    if (proc->unk30 >= gUnknown_0858265C[proc->unk38]->unk30)
    {
        proc->unk30 = 0;
        Proc_Break(proc);
    }
    else
    {
        proc->unk30++;
    }
}
asm(".global sub_0806C0E4\n.thumb_set sub_0806C0E4, CreditsPage_HoldLoop\n");

/* The cursor advance for the gUnknown_0858265C table: step +0x38, stop at the
 * NULL terminator, otherwise run the entry's sub-script under this proc and
 * jump back to label 0. Proc_StartBlocking's result is dead -- r0 is rewritten
 * by `adds r0, r4, #0` before Proc_Goto. */
void CreditsPage_Advance(struct Unk6C114Proc *proc)
{
    proc->unk38++;

    if (gUnknown_0858265C[proc->unk38] == NULL)
    {
        Proc_Break(proc);
    }
    else
    {
        Proc_StartBlocking(gUnknown_08581C48, proc);
        Proc_Goto(proc, 0);
    }
}
asm(".global sub_0806C114\n.thumb_set sub_0806C114, CreditsPage_Advance\n");
