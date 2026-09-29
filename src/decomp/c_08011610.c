#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011610.
 * sub_08011610 @ 0x08011610, sub_0801163C @ 0x0801163C, sub_08011668 @ 0x08011668, sub_08011684 @ 0x08011684
 */

#include "hardware.h"
#include "proc.h"
struct Unk11610Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk1163CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk11668Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk11684Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

/* r1 is never written before the `bl`, so the parent is this wrapper's own
 * second parameter passed straight through -- confirmed at the call site,
 * StartSlowLockingFadeToWhite does `adds r1, r0, #0; movs r0, #4; bl StartLockingFadeToWhite`. */
void StartLockingFadeToWhite(int a, ProcPtr parent)
{
    struct Unk11610Proc *proc = Proc_StartBlocking(gUnknown_0848923C, parent);

    proc->unk64 = a;
    gUnknown_030030E0.bits.effect = 2;
}
asm(".global sub_08011610\n.thumb_set sub_08011610, StartLockingFadeToWhite\n");

void StartLockingFadeFromWhite(int a, ProcPtr parent)
{
    struct Unk1163CProc *proc = Proc_StartBlocking(ProcScr_DesignRoomFadeIn, parent);

    proc->unk64 = a;
    gUnknown_030030E0.bits.effect = 2;
}
asm(".global sub_0801163C\n.thumb_set sub_0801163C, StartLockingFadeFromWhite\n");

void StartWipeToBlack(int a)
{
    struct Unk11668Proc *proc = Proc_Start(ProcScr_SomeFade, PROC_TREE_3);

    proc->unk64 = a;
}
asm(".global sub_08011668\n.thumb_set sub_08011668, StartWipeToBlack\n");

void StartWipeFromBlack(int a)
{
    struct Unk11684Proc *proc = Proc_Start(ProcScr_FadeLoadMap, PROC_TREE_3);

    proc->unk64 = a;
}
asm(".global sub_08011684\n.thumb_set sub_08011684, StartWipeFromBlack\n");
