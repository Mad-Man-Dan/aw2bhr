#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074600.
 * sub_08074600 @ 0x08074600
 */

#include "hardware.h"
#include "proc.h"
/* The proc's +0x4c counter is `u16` -- the same field c_08074628.c seeds to 0
 * -- and the sign test is an explicit `(s16)` cast at the use.  A bare
 * `lsls #0x10; cmp #0; bge` with no `asrs` is the sign-BIT test: agbcc knows
 * only bit 15 matters, so it never materialises the sign-extended value.  Its
 * twin DarkenRampUp_Loop compares against 0xf and therefore does get the `asrs`.
 */
struct Unk8074628
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk_4c;
};

void DarkenRampDown_Loop(struct Unk8074628 *proc)
{
    gUnknown_03001FFC = proc->unk_4c;
    proc->unk_4c--;

    if ((s16)proc->unk_4c < 0)
        Proc_Break(proc);
}
asm(".global sub_08074600\n.thumb_set sub_08074600, DarkenRampDown_Loop\n");
