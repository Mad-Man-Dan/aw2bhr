#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F6A0.
 * sub_0806F6A0 @ 0x0806F6A0, sub_0806F6C4 @ 0x0806F6C4, sub_0806F6D8 @ 0x0806F6D8
 */

#include "proc.h"
#include "hardware.h"
/* The fade-OUT twin of SoundRoomFadeToBlack_Loop: same shape, counting down from 16 and
 * breaking at 0. Read that function's header for why the assignment is
 * chained. Diffed against it rather than assumed: the compare is <= 0 (ROM
 * bgt) and the step is --, so the two differ in more than a constant. */
struct Unk6F6A0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};
/* FadeOutMusicDefault is declared void (void) and the ROM passes nothing: r0 still
 * holds proc at the bl only because it arrived there. The adds r4, r0, #0 is
 * the save of proc across the call, not argument setup.
 *
 * adds r4, #0x64 rather than a strh displacement because 0x64 is past strh's
 * imm5*2 = 62 limit. */
struct Unk6F6C4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ s16 unk64;
};
/* A 48-frame dwell: unk64 counts up each frame and, once past 0x2f, the proc
 * plays unk58 as a sound id (unless it is the -1 "none" sentinel) and breaks.
 *
 * unk64 is s16 -- the movs r3,#0; ldrsh r0,[r1,r3] reg+reg form is what a
 * SIGNED halfword compare costs; the ldrh beside it is the increment's own
 * read, and agbcc hoists it above the branch.
 *
 * unk58 is int: it is compared against -1 with a full-word cmp after
 * movs r0,#1; rsbs r0,r0,#0. The lsls #0x10; asrs #0x10 before the call is
 * therefore an explicit (s16) cast at the use, not the field's width --
 * PlayMusic takes int. */
struct Unk6F6D8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
    /* 5C */ STRUCT_PAD(0x5c, 0x64);
    /* 64 */ s16 unk64;
};

void SoundRoomFadeFromBlack_Loop(struct Unk6F6A0Proc *proc)
{
    int t;

    gUnknown_03001FFC = t = proc->unk58;

    if (t <= 0)
        Proc_Break(proc);

    proc->unk58--;
}
asm(".global sub_0806F6A0\n.thumb_set sub_0806F6A0, SoundRoomFadeFromBlack_Loop\n");

void SoundRoomMusicSwitch_Init(struct Unk6F6C4Proc *proc)
{
    FadeOutMusicDefault();
    proc->unk64 = 0;
}
asm(".global sub_0806F6C4\n.thumb_set sub_0806F6C4, SoundRoomMusicSwitch_Init\n");

void SoundRoomMusicSwitch_Loop(struct Unk6F6D8Proc *proc)
{
    if (proc->unk64 > 0x2F)
    {
        if (proc->unk58 != -1)
            PlayMusic((s16)proc->unk58);

        Proc_Break(proc);
    }
    else
    {
        proc->unk64++;
    }
}
asm(".global sub_0806F6D8\n.thumb_set sub_0806F6D8, SoundRoomMusicSwitch_Loop\n");
