#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080723C0.
 * sub_080723C0 @ 0x080723C0
 */

#include "proc.h"
struct Unk723C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ s32 unk4c;
};

void RemoveFadeCoreCallBack(void)
{
    struct Unk723C0Proc * proc = Proc_Find(gUnknown_08613EE4);

    if (proc != NULL)
        proc->unk4c = 0;
}
asm(".global sub_080723C0\n.thumb_set sub_080723C0, RemoveFadeCoreCallBack\n");
