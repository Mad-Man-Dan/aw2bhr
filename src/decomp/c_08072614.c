#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072614.
 * sub_08072614 @ 0x08072614
 */

#include "proc.h"
struct Unk72614Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u16 *unk2c;
    /* 30 */ u16 unk30;
    /* 32 */ u16 unk32;
    /* 34 */ u16 unk34;
    /* 36 */ u16 unk36;
    /* 38 */ u16 unk38;
    /* 3a */ u16 unk3a;
};

/* The per-frame body of the StartPaletteAnimatorExt palette-rotate proc: every unk34
 * frames, roll the unk32-entry palette by one and re-upload it as two runs
 * (tail then head). unk3a selects the direction. */
void PaletteAnimator_Loop(struct Unk72614Proc *proc)
{
    int i;

    if (++proc->unk36 >= proc->unk34)
    {
        proc->unk36 = 0;

        i = DivRem(proc->unk38, proc->unk32);

        if (proc->unk3a != 0)
            i = proc->unk32 - i - 1;

        ApplyPaletteExt(proc->unk2c + i, proc->unk30,
            (u16)((proc->unk32 - i) * 2));

        if (i > 0)
            ApplyPaletteExt(proc->unk2c,
                (u16)(proc->unk32 * 2 + proc->unk30 - i * 2), (u16)(i * 2));

        proc->unk38++;
    }
}
asm(".global sub_08072614\n.thumb_set sub_08072614, PaletteAnimator_Loop\n");
