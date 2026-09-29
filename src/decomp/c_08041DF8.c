#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041DF8.
 * sub_08041DF8 @ 0x08041DF8
 */

#include "proc.h"
/* The per-proc sibling of the matched sub_08041E48 next door: the same three
 * bracketing calls and the same gPlaySt.unk32 gate on the same
 * SendActionCommand command, but issuing command 4 with the byte behind
 * GetAttackTargetRecord's result instead of the proc's own coordinates.
 *
 * The `bl GetAttackTargetRecord` sits BETWEEN the two tests, which is what fixes the
 * shape as a nested `if` with the pointer bound in the outer block rather than
 * one `&&`: a call cannot be hoisted above a conditional branch, so an `&&`
 * spelling could not put it there. */
struct Unk41DF8Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x64);
    /* 0x64 */ s16 unk64; /* `ldrsh` through a scratch register, so signed --
                           * the same member sub_08041E48 reads. */
};

void sub_08041DF8(struct Unk41DF8Proc *proc)
{
    u8 *p;

    sub_0802C57C();
    CommitUnitMove();
    sub_0802C594();

    if (proc->unk64 != 0)
    {
        p = (u8 *)GetAttackTargetRecord(gUnknown_03003F40);

        if (gPlaySt.savingEnabled != 0)
            SendActionCommand(4, gUnknown_03003F38, *p, 0);
    }
}
