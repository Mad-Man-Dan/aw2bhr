#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E574.
 * sub_0806E574 @ 0x0806E574, sub_0806E590 @ 0x0806E590, sub_0806E5A0 @ 0x0806E5A0
 */

#include "proc.h"
/* Reads the same u16 handle twice rather than caching it, which is what the two
 * separate `ldrh r0, [r4, #0x34]` say -- a cached local would have kept it in a
 * register across the first call. The proc layout is StartSoundRoomCoPortrait's, the
 * starter that writes unk34 in the first place. `movs #0x90; lsls #1` is 0x120,
 * the screen width in eighths of a pixel. */
struct Unk6E574Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ u16 unk34;
};
/* SoundRoomCoPortrait_LoadPart0's single-call sibling over the same field and the same 0x120.
 * The bare `push {lr}` and the `ldrh r0, [r0, #0x34]` straight off the incoming
 * register are what says nothing is live across the call. */
struct Unk6E590Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ u16 unk34;
};
/* The two coordinate fields are masked to their hardware widths on the way out
 * -- 9 bits for X and 8 for Y, which is exactly BG offset range -- so the proc
 * stores them unclamped as signed words and the clamp lives here. The 9-bit
 * mask needs a pool word; the 8-bit one is an imm8. */
struct Unk6E5A0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
};

void SoundRoomCoPortrait_LoadPart0(struct Unk6E574Proc *proc)
{
    LoadCoFullBodyPart0(proc->unk34, 0x120);
    LoadCoPalette(proc->unk34, 0x10);
}
asm(".global sub_0806E574\n.thumb_set sub_0806E574, SoundRoomCoPortrait_LoadPart0\n");

void SoundRoomCoPortrait_LoadPart1(struct Unk6E590Proc *proc)
{
    LoadCoFullBodyPart1(proc->unk34, 0x120);
}
asm(".global sub_0806E590\n.thumb_set sub_0806E590, SoundRoomCoPortrait_LoadPart1\n");

void SoundRoomCoPortrait_Draw(struct Unk6E5A0Proc *proc)
{
    sub_08043C28(proc->unk2c & 0x1FF, proc->unk30 & 0xFF, 0x920, 0, 1);
}
asm(".global sub_0806E5A0\n.thumb_set sub_0806E5A0, SoundRoomCoPortrait_Draw\n");
