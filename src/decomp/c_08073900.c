#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073900.
 * sub_08073900 @ 0x08073900
 */

#include "proc.h"
struct Unk8073900
{
    /* 0x00 */ u8 filler_00[0x5c];
    /* 0x5c */ s32 unk_5c;
};

void StartCircleWipe(s32 arg)
{
    struct Unk8073900 * proc = Proc_Start(gUnknown_086141B4, PROC_TREE_VSYNC);

    proc->unk_5c = arg;
}
asm(".global sub_08073900\n.thumb_set sub_08073900, StartCircleWipe\n");
