#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08029234.
 * sub_08029234 @ 0x08029234
 */

#include "proc.h"
/* Clear one halfword, seed the next from GetAttackTargetCount, then block on a child
 * proc and play a sound.
 *
 * Both fields are reached with `adds r1, r4, #0; adds r1, #0x64` rather than a
 * `strh` displacement -- 0x64 and 0x66 are past strh's imm5*2 limit of 62.
 *
 * No re-narrowing after `bl GetAttackTargetCount`: it returns a word, and the `strh` is
 * the field's own truncation. */
struct Unk29234Proc
{
    /* 00 */ u8 filler_00[0x64];
    /* 64 */ u16 unk64;
    /* 66 */ u16 unk66;
};

void TargetPickList_Init(struct Unk29234Proc *proc)
{
    proc->unk64 = 0;
    proc->unk66 = GetAttackTargetCount();
    StartRangeSpread(proc);
    PlayMusicOrSfx2(0x77);
}
asm(".global sub_08029234\n.thumb_set sub_08029234, TargetPickList_Init\n");
