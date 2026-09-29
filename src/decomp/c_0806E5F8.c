#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E5F8.
 * sub_0806E5F8 @ 0x0806E5F8, sub_0806E638 @ 0x0806E638, sub_0806E658 @ 0x0806E658
 */

#include "proc.h"
/* A four-frame slide-in step: interpolates the OTHER proc's X from 0xb4 to
 * 0x140 over unk58, then either advances the frame counter or breaks.
 *
 * The two procs are different objects and the code says so -- the stepped one
 * arrives in r0 and is kept in r5, while Proc_Find's result goes to r4 and is
 * the only thing written.
 *
 * Both arms of the tail JOIN at the epilogue, so the out-of-line arm is the
 * ELSE: `ble` reaches the increment, making Proc_Break the `if` body and the
 * condition `> 3`. */
struct Unk6E5F8Target
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
};
struct Unk6E5F8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};
/* Hands a word field of one proc to the halfword field of another and restarts
 * it. The `ldr` / `strh` pair across the assignment is the whole type evidence:
 * unk5c is a word in the source proc and unk34 is a halfword in the target, and
 * the narrowing is free because a `strh` truncates on its own.
 *
 * Proc_Find's result is used for BOTH the store and Proc_Goto's first argument
 * with no copy in between, which is why nothing is saved beyond r4. */
struct Unk6E638Target
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ u16 unk34;
};
struct Unk6E638Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x5c);
    /* 5c */ s32 unk5c;
};
/* SoundRoomCoSlideOut_Loop's slide-OUT twin: the same four-frame step over the same proc
 * and the same field, with the endpoints exchanged (0x140 down to 0xb4) and a
 * different easing selector.
 *
 * The selector is 4 and not 1, and the ROM hides it: `movs r0, #4; str r0, [sp]`
 * sets up the FIFTH argument, and r0 is never written again before the `bl`, so
 * the same 4 is also the first argument. agbcc CSEs the constant across the two
 * positions, which makes the first argument invisible unless the register is
 * traced. Its twin, which passes 1, has to re-materialise r0 after the stack
 * store and that extra `movs` is the difference between the two bodies. */
struct Unk6E658Target
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
};
struct Unk6E658Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};

void SoundRoomCoSlideOut_Loop(struct Unk6E5F8Proc *proc)
{
    struct Unk6E5F8Target *target = Proc_Find(gUnknown_08582B2C);

    target->unk2c = Interpolate(1, 0xb4, 0x140, proc->unk58, 4);

    if (proc->unk58 > 3)
        Proc_Break(proc);
    else
        proc->unk58++;
}
asm(".global sub_0806E5F8\n.thumb_set sub_0806E5F8, SoundRoomCoSlideOut_Loop\n");

void SoundRoomCoSlideOut_ReloadPortrait(struct Unk6E638Proc *proc)
{
    struct Unk6E638Target *target = Proc_Find(gUnknown_08582B2C);

    target->unk34 = proc->unk5c;
    Proc_Goto(target, 0);
}
asm(".global sub_0806E638\n.thumb_set sub_0806E638, SoundRoomCoSlideOut_ReloadPortrait\n");

void SoundRoomCoSlideIn_Loop(struct Unk6E658Proc *proc)
{
    struct Unk6E658Target *target = Proc_Find(gUnknown_08582B2C);

    target->unk2c = Interpolate(4, 0x140, 0xb4, proc->unk58, 4);

    if (proc->unk58 > 3)
        Proc_Break(proc);
    else
        proc->unk58++;
}
asm(".global sub_0806E658\n.thumb_set sub_0806E658, SoundRoomCoSlideIn_Loop\n");
