#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D88C.
 * sub_0803D88C @ 0x0803D88C, sub_0803D8C0 @ 0x0803D8C0
 */

#include "proc.h"
/* Copies StartSaveScreen's proc field into the script record StartEventScript
 * installs. The `adds r4,#0x64` before the `ldrh` is not a choice: 0x64 is past
 * the `ldrh` immediate's range. */
struct UnkD8C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

/* Two tests, one `&&`: both `bne`s land on the same PlayMusic arm. The
 * `lsls #0x18; cmp #0` on IsPlayer1TeamAlive's result is the bool8 truth test. */
void SaveScreen_SkipIfPlayerDead(ProcPtr proc)
{
    if (gPlaySt.gameMode == 1 && !IsPlayer1TeamAlive())
        Proc_GotoScript(proc, gUnknown_0849F388);
    else
        PlayMusic(0xcd);
}
asm(".global sub_0803D88C\n.thumb_set sub_0803D88C, SaveScreen_SkipIfPlayerDead\n");

void SaveScreen_StartMessage(struct UnkD8C0Proc *proc)
{
    StartEventScript(gUnknown_0849F3A8)->unk10 = proc->unk64;
}
asm(".global sub_0803D8C0\n.thumb_set sub_0803D8C0, SaveScreen_StartMessage\n");
