#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C824.
 * sub_0806C824 @ 0x0806C824
 */

#include "proc.h"
#include "hardware.h"
struct Unk6C824Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

/* src/decomp/c_0806C154.c with the two BLDALPHA coefficients swapped and no
 * sprite: same `.bits.effect = 1`, same `gUnknown_03001FFC = gUnknown_03001FFC`
 * self-assignment (an `ldrh` immediately followed by a `strh` to the same
 * address, which is the only thing that produces a load whose value is stored
 * straight back), same `> 0x1f` bound. */
void CreditsResultFade_Loop(struct Unk6C824Proc *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0x10 - (proc->unk58 >> 1);
    gUnknown_03002B28 = proc->unk58 >> 1;
    gUnknown_03001FFC = gUnknown_03001FFC;

    if (proc->unk58 > 0x1F)
        Proc_Break(proc);
    else
        proc->unk58++;
}
asm(".global sub_0806C824\n.thumb_set sub_0806C824, CreditsResultFade_Loop\n");
