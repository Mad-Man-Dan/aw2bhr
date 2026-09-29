#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013338.
 * sub_08013338 @ 0x08013338
 */

#include "proc.h"
/* The unknown-functions.h note for this one already had it: arg 0 indexes
 * gUnknown_0848950C by words into +0x4c and arg 1 is a halfword at +0x44. The
 * table's elements are proc scripts -- its first three words are 0x084893DC,
 * 0x084893F0 and 0x08489404, all inside the 0x08489 script run -- so +0x4c
 * holds a `const struct ProcCmd *`.
 *
 * Tree 1 here where StartWhiteFlash uses tree 3; the `movs r1, #1` is the whole
 * difference. */
struct Unk13338Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x44);
    /* 44 */ u16 unk44;
    /* 46 */ STRUCT_PAD(0x46, 0x4c);
    /* 4c */ const struct ProcCmd *unk4c;
};

void StartScreenShake(int a, int b, ProcPtr parent)
{
    struct Unk13338Proc *proc;

    if (parent != 0)
        proc = Proc_StartBlocking(gUnknown_084893AC, parent);
    else
        proc = Proc_Start(gUnknown_084893AC, PROC_TREE_1);

    proc->unk4c = gUnknown_0848950C[a];
    proc->unk44 = b;
}
asm(".global sub_08013338\n.thumb_set sub_08013338, StartScreenShake\n");
