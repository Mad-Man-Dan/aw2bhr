#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072314.
 * sub_08072314 @ 0x08072314, sub_08072320 @ 0x08072320
 */

#include "proc.h"
struct Unk72314Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ void (*unk4c)(void);
    /* 50 */ STRUCT_PAD(0x50, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
};
struct Unk72320Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ void (*unk4c)(void);
};

/* The init of the gUnknown_08613EE4 fade proc: clear the two accumulators
 * FadeCore_Tick steps and the completion callback FadeCore_Loop invokes.
 *
 * +0x4c is a `void (*)(void)`, not the `s32` src/decomp/c_080723C0.c models it
 * as -- FadeCore_Loop loads it and calls it through `_call_via_r0`. Clearing it
 * is byte-identical either way, so the callback is the only evidence and it
 * lives in a different function; the promoted file's weaker model is not
 * wrong, just uninformed. */
void FadeCore_Init(struct Unk72314Proc *proc)
{
    proc->unk58 = 0;
    proc->unk5c = 0;
    proc->unk4c = NULL;
}
asm(".global sub_08072314\n.thumb_set sub_08072314, FadeCore_Init\n");

/* The gUnknown_08613EE4 fade's per-frame body: step the fade, and on the frame
 * it reports done, run the completion callback (if any) and break.
 *
 * `lsls r0, r0, #0x18` after the `bl` is the narrowing of FadeCore_Tick's 8-bit
 * return -- the caller-side evidence that types it. `bl _call_via_r0` is the
 * ordinary ARMv4T indirect call, and r0 specifically says the callback takes no
 * arguments, which is a harder readout than most arity guesses. */
void FadeCore_Loop(struct Unk72320Proc *proc)
{
    if (FadeCore_Tick(proc) == 0)
    {
        if (proc->unk4c != NULL)
            proc->unk4c();

        Proc_Break(proc);
    }
}
asm(".global sub_08072320\n.thumb_set sub_08072320, FadeCore_Loop\n");
