#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F3E4.
 * sub_0803F3E4 @ 0x0803F3E4
 */

#include "proc.h"
struct UnkF3E4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
};

/* `adds r1, r2, #0` before the bl -- the parent is the third parameter. */
void StartVolcanoRock(int a, int b, ProcPtr parent)
{
    struct UnkF3E4Proc *proc = Proc_Start(gUnknown_0849F918, parent);

    proc->unk2c = a;
    proc->unk30 = b;
}
asm(".global sub_0803F3E4\n.thumb_set sub_0803F3E4, StartVolcanoRock\n");
