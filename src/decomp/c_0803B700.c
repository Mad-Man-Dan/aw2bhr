#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B700.
 * sub_0803B700 @ 0x0803B700, sub_0803B734 @ 0x0803B734
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

/* Adopt the endpoint of whatever gUnknown_0849E778 proc is already running and
 * end that family; if there is none, end ourselves instead. Either way the step
 * counter is reset, which is why the `strh` sits after the join and not in
 * both arms.
 *
 * gUnknown_0849E778's address is loaded once into r5 and reused for both
 * Proc_Find and Proc_EndEach -- one pool word, so the script is named once in
 * the source and CSE'd, not spelled twice. */
void MusicDuckRelease_Init(struct UnkB6A8Proc *proc)
{
    struct UnkB6A8Proc *other = Proc_Find(gUnknown_0849E778);

    if (other != NULL) {
        proc->unk54 = other->unk54;
        Proc_EndEach(gUnknown_0849E778);
    } else {
        Proc_End(proc);
    }
    proc->unk64 = 0;
}
asm(".global sub_0803B700\n.thumb_set sub_0803B700, MusicDuckRelease_Init\n");

/* MusicDuck_Loop with the two middle Interpolate endpoints SWAPPED: this one
 * runs from proc->unk54 to 0x100, the other from 0x100 to proc->unk54. The
 * whole difference is which argument register receives the `ldr [r4,#0x54]`
 * and which receives the `movs #0x80; lsls #1`. */
void MusicDuckRelease_Loop(struct UnkB6A8Proc *proc)
{
    if (proc->unk64 > 0x13)
        Proc_Break(proc);
    SetMusicVolume(Interpolate(0, proc->unk54, 0x100, proc->unk64++, 0x14));
}
asm(".global sub_0803B734\n.thumb_set sub_0803B734, MusicDuckRelease_Loop\n");
