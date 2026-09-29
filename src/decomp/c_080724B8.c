#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080724B8.
 * sub_080724B8 @ 0x080724B8
 */

#include "proc.h"
struct Unk724B8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ u32 unk58;
};

/* The tick of the gUnknown_08613F0C proc src/decomp/c_0807249C.c starts, and
 * +0x58 is the counter that starter seeds; u32 to agree with that promoted
 * definition.
 *
 * The `b` over the else arm is a real if/else and not a returned comparison --
 * the function is void (`pop {r0}`) and the two arms do different work rather
 * than producing 0 and 1. */
void TemporaryLock_OnLoop(struct Unk724B8Proc *proc)
{
    if (proc->unk58 == 0)
        Proc_Break(proc);
    else
        proc->unk58--;
}
asm(".global sub_080724B8\n.thumb_set sub_080724B8, TemporaryLock_OnLoop\n");
