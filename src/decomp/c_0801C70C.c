#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C70C.
 * sub_0801C70C @ 0x0801C70C, sub_0801C754 @ 0x0801C754
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* SIX parameters -- four in registers and two on the stack ([sp,#0x18] and
 * [sp,#0x1c] past the six pushed registers). Only the last is narrow: it
 * arrives `lsls #0x10; lsrs #0x10` on its way to AP_Create's u16 second
 * parameter.
 *
 * The `(void *)` cast on a1 is the price of the declared `const void *` first
 * parameter meeting AP_Create's `void *`; it is byte-neutral and keeps the
 * ROM blobs its callers pass from having to drop const.
 *
 * It ends `pop {r1}; bx r1`, i.e. it returns a value, and the value is
 * whatever Proc_Start left in r0 -- no instruction moves anything else there.
 * `return (int)proc;` is byte-identical to falling off the end and says so. */
struct Unk0801C70CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};
/* The per-frame callback of the ProcScr_WaitForLaser proc APProc_Create starts:
 * the three fields it reads (+0x50, +0x54, +0x58) are exactly the three that
 * function writes, which is what pins the parameter as that proc and fixes the
 * struct below. */
struct Unk0801C754Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

int APProc_Create(const void *a1, int a2, int a3, int a4, int a5, u16 a6)
{
    struct Unk0801C210 *h;
    struct Unk0801C70CProc *proc;

    h = AP_Create((void *)a1, a6, 1);
    AP_SwitchAnimation(h, a5);
    h->unk22 = a4;

    proc = Proc_Start(ProcScr_WaitForLaser, PROC_TREE_3);
    proc->unk50 = h;
    proc->unk54 = a2;
    proc->unk58 = a3;

    return (int)proc;
}
asm(".global sub_0801C70C\n.thumb_set sub_0801C70C, APProc_Create\n");

void APProc_OnUpdate(struct Unk0801C754Proc *proc)
{
    if (AP_Update(proc->unk50, proc->unk54, proc->unk58) == 0)
    {
        if (proc->unk50 == NULL || proc->unk50->unk00 == NULL)
            Proc_End(proc);
    }
}

asm(".global sub_0801C754\n.thumb_set sub_0801C754, APProc_OnUpdate\n");
