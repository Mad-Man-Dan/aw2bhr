#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039634.
 * sub_08039634 @ 0x08039634
 */

#include "proc.h"
struct Unk39634Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

void StartCoPowerSequence(int a, int b)
{
    struct Unk39634Proc *proc = Proc_Start(gUnknown_0849D77C, PROC_TREE_3);

    proc->unk54 = a;
    proc->unk58 = b;
}
asm(".global sub_08039634\n.thumb_set sub_08039634, StartCoPowerSequence\n");
