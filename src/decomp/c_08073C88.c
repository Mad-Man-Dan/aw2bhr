#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073C88.
 * sub_08073C88 @ 0x08073C88
 */

#include "proc.h"
/* Same shape as StartCircleWipe on the neighbouring script gUnknown_086141B4. */
struct Unk73C88Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x5c);
    /* 5c */ s32 unk5c;
};

void StartPolygonWipe(s32 a)
{
    struct Unk73C88Proc * proc = Proc_Start(gUnknown_086141DC, PROC_TREE_VSYNC);

    proc->unk5c = a;
}
asm(".global sub_08073C88\n.thumb_set sub_08073C88, StartPolygonWipe\n");
