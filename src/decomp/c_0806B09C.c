#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806B09C.
 * sub_0806B09C @ 0x0806B09C, sub_0806B0B4 @ 0x0806B0B4, sub_0806B0E0 @ 0x0806B0E0, sub_0806B0F4 @ 0x0806B0F4
 */

/* Same proc as src/decomp/c_0806B050.c -- +0x58 is the frame counter its
 * neighbours count down. */
struct Unk6B050Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};
#include "proc.h"

void sub_0806B09C(struct Unk6B050Proc *proc)
{
    ColFadeToBlack(-1);
    proc->unk58 = 0x80;
}

void sub_0806B0B4(struct Unk6B050Proc *proc)
{
    if (!(proc->unk58 & 3))
    {
        sub_080718F0();
        EnablePaletteSync();
    }

    proc->unk58--;

    if (proc->unk58 == 0)
        Proc_Break(proc);
}

/* sub_0806B09C's twin: the other end of the same fade, half the frames. */
void sub_0806B0E0(struct Unk6B050Proc *proc)
{
    ColFadeFromBlack(1);
    proc->unk58 = 0x40;
}

/* sub_0806B0B4's twin, ticking every other frame instead of every fourth. */
void sub_0806B0F4(struct Unk6B050Proc *proc)
{
    if (!(proc->unk58 & 1))
    {
        sub_080718F0();
        EnablePaletteSync();
    }

    proc->unk58--;

    if (proc->unk58 == 0)
        Proc_Break(proc);
}
