#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B6A8.
 * sub_0803B6A8 @ 0x0803B6A8
 */

#include "proc.h"
/* The proc this block animates. Only two members are ever touched: the word at
 * +0x54 (an Interpolate endpoint) and the SIGNED halfword at +0x64, which is
 * the step counter -- `movs r1,#0; ldrsh r0,[r5,r1]` is the register-offset
 * form `ldrsh` is forced into and is what fixes the sign. */
struct UnkB6A8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
    /* 58 */ STRUCT_PAD(0x58, 0x64);
    /* 64 */ s16 unk64;
};

/* One frame of a 0x14-step interpolation. The counter is POST-incremented, so
 * the value handed to Interpolate is the one before the store -- the `ldrh`,
 * the `adds #1; strh`, and only then the `lsls #16; asrs #16` that sign-extends
 * the OLD value into r3.
 *
 * Interpolate is a 5-argument function, so 0x14 goes on the stack (`sub sp,#4`
 * in the prologue, `str r0,[sp]` before the `bl`), and its result is consumed
 * by SetMusicVolume rather than dropped -- the two `bl`s are a nesting here
 * because SetMusicVolume takes an int and nothing sets r0 between them. */
void MusicDuck_Loop(struct UnkB6A8Proc *proc)
{
    if (proc->unk64 > 0x13)
        Proc_Break(proc);
    SetMusicVolume(Interpolate(0, 0x100, proc->unk54, proc->unk64++, 0x14));
}
asm(".global sub_0803B6A8\n.thumb_set sub_0803B6A8, MusicDuck_Loop\n");
