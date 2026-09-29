#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B608.
 * sub_0803B608 @ 0x0803B608, sub_0803B628 @ 0x0803B628
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct Unk3B608Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x64);
    /* 0x64 */ u16 unk64;
};

void FadeSound_IDLE_0803B609(struct Unk3B608Proc *proc)
{
    proc->unk64--;

    if (proc->unk64 == 0)
        Proc_Break(proc);
}

bool8 IsMusicFadeActive(void)
{
    return Proc_Find(ProcScr_FadeSound) != 0;
}

asm(".global sub_0803B628\n.thumb_set sub_0803B628, IsMusicFadeActive\n"
    ".global sub_0803B608\n.thumb_set sub_0803B608, FadeSound_IDLE_0803B609\n");
