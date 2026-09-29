#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013098.
 * sub_08013098 @ 0x08013098, sub_080130B0 @ 0x080130B0, sub_080130C8 @ 0x080130C8
 */

#include "proc.h"
struct Unk13098Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};
struct Unk130B0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
};

void StartLockingWipeToBlack(int a, ProcPtr parent)
{
    struct Unk13098Proc *proc = Proc_StartBlocking(ProcScr_SomeFade, parent);

    proc->unk64 = a;
}
asm(".global sub_08013098\n.thumb_set sub_08013098, StartLockingWipeToBlack\n");

void StartLockingWipeFromBlack(int a, ProcPtr parent)
{
    struct Unk130B0Proc *proc = Proc_StartBlocking(ProcScr_FadeLoadMap, parent);

    proc->unk64 = a;
}
asm(".global sub_080130B0\n.thumb_set sub_080130B0, StartLockingWipeFromBlack\n");

void sub_080130C8(u16 *dst, int delta, int size)
{
    while ((size -= 2) != 0)
    {
        *dst += delta;
        dst++;
    }
}
