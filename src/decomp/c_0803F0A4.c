#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F0A4.
 * sub_0803F0A4 @ 0x0803F0A4
 */

#include "proc.h"
struct UnkF0A4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ STRUCT_PAD(0x60, 0x64);
    /* 64 */ u16 unk64;
    /* 66 */ u16 unk66;
    /* 68 */ u16 unk68;
    /* 6a */ u16 unk6a;
};

/* Seven arguments, three of them on the stack. All `int`: the two halfword
 * members fed from the stack pair are narrowed AT THE STORE, so a5 arrives as
 * a whole word and is read back with a single `ldrh [sp,#0x1c]` at its use.
 * Declaring either of them `u16` adds a PROMOTE_MODE narrowing in the prologue
 * and swaps which of the two gets preloaded into r7. */
void StartCannonFire(int a1, int a2, int a3, int a4, int a5, int a6, ProcPtr parent)
{
    struct UnkF0A4Proc *proc;

    RunMapEventsForAction(0x2c, 0);
    proc = Proc_StartBlocking(gUnknown_0849F830, parent);
    proc->unk54 = a1;
    proc->unk58 = a2;
    proc->unk64 = a3;
    proc->unk66 = a4;
    proc->unk68 = a6;
    proc->unk6a = a5;
    proc->unk5c = 0;
    ScrollCameraToCenterCell(a1 + 1, a2 + 1);
}
asm(".global sub_0803F0A4\n.thumb_set sub_0803F0A4, StartCannonFire\n");
