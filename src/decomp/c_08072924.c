#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072924.
 * sub_08072924 @ 0x08072924, sub_08072948 @ 0x08072948
 */

#include "proc.h"
struct Unk72924Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ void (*unk2c)(void);
    /* 30 */ STRUCT_PAD(0x30, 0x34);
    /* 34 */ int unk34;
};
struct Unk72948Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ void (*unk2c)(int);
    /* 30 */ int unk30;
    /* 34 */ int unk34;
};

/* The loop body of the gUnknown_08613F34 proc src/decomp/c_08072970.c starts --
 * a delayed nullary call. -1 needs a register (`movs #1; rsbs`) because THUMB's
 * `cmp` immediate is unsigned 8-bit, so the comparison is against the
 * POST-decrement value and the timer legitimately reaches -1.
 *
 * `_call_via_r0` is a nullary indirect call, so +0x2c is `void (*)(void)`;
 * c_08072970.c models the same slot as `u32` because a plain word store cannot
 * tell the difference. */
void CallDelayed_OnLoop(struct Unk72924Proc *proc)
{
    proc->unk34--;

    if (proc->unk34 == -1)
    {
        proc->unk2c();
        Proc_Break(proc);
    }
}
asm(".global sub_08072924\n.thumb_set sub_08072924, CallDelayed_OnLoop\n");

/* The one-argument twin of CallDelayed_OnLoop, on the gUnknown_08613F44 proc: the
 * callback at +0x2c takes the word at +0x30. `_call_via_r1` rather than
 * `_call_via_r0` is the arity readout.
 *
 * The LOCAL is load-bearing and was worth finding. Written the obvious way,
 * `proc->unk2c(proc->unk30)` emits the argument load before the pointer load;
 * the ROM has them the other way round. Binding the pointer to its own local
 * first forces agbcc to expand it ahead of the argument list, which is the only
 * spelling that reproduces `ldr r1,[r4,#0x2c]` then `ldr r0,[r4,#0x30]`. Both
 * forms are 40 bytes, so size alone does not catch this. */
void CallDelayedArg_OnLoop(struct Unk72948Proc *proc)
{
    void (*func)(int);

    proc->unk34--;

    if (proc->unk34 == -1)
    {
        func = proc->unk2c;
        func(proc->unk30);
        Proc_Break(proc);
    }
}
asm(".global sub_08072948\n.thumb_set sub_08072948, CallDelayedArg_OnLoop\n");
