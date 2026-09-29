#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078588.
 * sub_08078588 @ 0x08078588
 */

#include "proc.h"
/* sub_08078504 wrapped in a null check on the script pointer. The ROM re-loads
 * +0x54 after the StartDialogueBlock call rather than keeping the value it just
 * tested, which is what an ordinary non-const field does across a call. */

struct UnkProc8615AAC
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ const u8 *unk_54;
};

void sub_08078588(struct UnkProc8615AAC *proc)
{
    if (proc->unk_54 != NULL)
    {
        gUnknown_03002F08.unk00 = 0;
        StartDialogueBlock(proc);
        StartEventScript(proc->unk_54);
    }
}
