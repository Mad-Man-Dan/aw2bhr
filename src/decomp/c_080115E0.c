#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080115E0.
 * sub_080115E0 @ 0x080115E0, sub_080115F8 @ 0x080115F8
 */

#include "proc.h"

struct Unk115E0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
#include "proc.h"

struct Unk115F8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

void StartLockingFadeToBlack(int a, ProcPtr parent)
{
    struct Unk115E0Proc *proc = Proc_StartBlocking(gUnknown_0848923C, parent);

    proc->unk64 = a;
}
asm(".global sub_080115E0\n.thumb_set sub_080115E0, StartLockingFadeToBlack\n");

void StartLockingFadeFromBlack(int a, ProcPtr parent)
{
    struct Unk115F8Proc *proc = Proc_StartBlocking(ProcScr_DesignRoomFadeIn, parent);

    proc->unk64 = a;
}
asm(".global sub_080115F8\n.thumb_set sub_080115F8, StartLockingFadeFromBlack\n");
