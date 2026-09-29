#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F634.
 * sub_0806F634 @ 0x0806F634
 */

#include "proc.h"
#include "hardware.h"
/* A 16-frame fade-in tick: proc->unk58 counts UP and is mirrored into the BLDY
 * shadow every frame, and the proc breaks once the counter passes 15.
 *
 * unk58 is a plain int -- ldr/str, and the strh into gUnknown_03001FFC is the
 * u16 global's own narrowing store, NOT evidence about the field.
 *
 * The assignment is chained rather than split into two statements: the ROM
 * materialises the global's address BEFORE loading the field, which is the
 * order a single assignment expression gives. Writing
 * t = proc->unk58; gUnknown_03001FFC = t; swaps those two ldrs.
 * gUnknown_03001FFC is volatile, so the compare cannot read it back -- the
 * binding local is what keeps the ROM's single ldr of the field. */
struct Unk6F634Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ s32 unk58;
};

void SoundRoomFadeToBlack_Loop(struct Unk6F634Proc *proc)
{
    int t;

    gUnknown_03001FFC = t = proc->unk58;

    if (t > 0xF)
        Proc_Break(proc);

    proc->unk58++;
}
asm(".global sub_0806F634\n.thumb_set sub_0806F634, SoundRoomFadeToBlack_Loop\n");
