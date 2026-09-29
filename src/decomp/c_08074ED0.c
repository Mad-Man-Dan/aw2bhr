#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074ED0.
 * sub_08074ED0 @ 0x08074ED0
 */

#include "proc.h"
struct Unk8074ED0
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ void * unk_54;
    /* 0x58 */ s32 unk_58;
};

void StartWorldMapScope(void * arg, ProcPtr parent)
{
    struct Unk8074ED0 * proc = Proc_Start(gUnknown_08614344, parent);

    proc->unk_54 = arg;
    proc->unk_58 = 0;
}
asm(".global sub_08074ED0\n.thumb_set sub_08074ED0, StartWorldMapScope\n");
