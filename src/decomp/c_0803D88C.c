#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D88C.
 * sub_0803D88C @ 0x0803D88C, sub_0803D8C0 @ 0x0803D8C0
 */

#include "proc.h"
/* Copies sub_0803D73C's proc field into the script record StartEventScript
 * installs. The `adds r4,#0x64` before the `ldrh` is not a choice: 0x64 is past
 * the `ldrh` immediate's range. */
struct UnkD8C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

/* Two tests, one `&&`: both `bne`s land on the same sub_0803B524 arm. The
 * `lsls #0x18; cmp #0` on IsPlayer1TeamAlive's result is the bool8 truth test. */
void sub_0803D88C(ProcPtr proc)
{
    if (gPlaySt.gameMode == 1 && !IsPlayer1TeamAlive())
        Proc_GotoScript(proc, gUnknown_0849F388);
    else
        sub_0803B524(0xcd);
}

void sub_0803D8C0(struct UnkD8C0Proc *proc)
{
    StartEventScript(gUnknown_0849F3A8)->unk10 = proc->unk64;
}
