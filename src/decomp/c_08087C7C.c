#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087C7C.
 * sub_08087C7C @ 0x08087C7C
 */

#include "proc.h"
struct Unk08087C7C
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void CoDesignExit_SaveIfFlagSet(struct Unk08087C7C *proc)
{
    if (proc->unk64 != 0)
        sub_0803D960(proc);
}
asm(".global sub_08087C7C\n.thumb_set sub_08087C7C, CoDesignExit_SaveIfFlagSet\n");
