#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B828.
 * sub_0806B828 @ 0x0806B828
 */

#include "proc.h"
#include "hardware.h"
struct Unk6B828Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
};

/* `(x & 0x3f) | 0x40` on a BYTE of gUnknown_030030E0 is the 2-bit `effect`
 * field at bits 6-7 being assigned 1 -- an AND against the complement of a
 * group mask followed by an OR, which is store_fixed_bit_field, not a scalar
 * read-modify-write. `.bits.effect` is the spelling src/decomp/c_0806C154.c
 * already uses for the same write; the `.raw` alternative in hardware.h is for
 * the `ldrh` + pool-mask form, which this is not.
 *
 * The `>> 3` result is kept live and reused for the 0xe subtraction, so the
 * source really does write `proc->unk58 >> 3` twice and lets CSE do it. */
void CreditsFadeProc_Loop(struct Unk6B828Proc *proc)
{
    proc->unk58 += proc->unk5c;

    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = proc->unk58 >> 3;
    gUnknown_03002B28 = 0xE - (proc->unk58 >> 3);
    gUnknown_03001FFC = 0;

    proc->unk60++;

    if (proc->unk60 > 0x6F)
        Proc_Break(proc);
}
asm(".global sub_0806B828\n.thumb_set sub_0806B828, CreditsFadeProc_Loop\n");
