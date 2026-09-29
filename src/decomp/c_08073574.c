#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073574.
 * sub_08073574 @ 0x08073574
 */

#include "proc.h"
struct Unk73574Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ int unk48;
};

/* Six parameters -- four in registers, the fifth and sixth at [sp,#0x18] and
 * [sp,#0x1c], which is right above this function's six pushed words. All six
 * are saved with a bare `adds rN, rM, #0` (r8 for the fourth) and no
 * PROMOTE_MODE narrowing, so all six are int. */

void StartBgWave(int a, int b, int c, int d, int e, int f)
{
    struct Unk73574Proc *proc = Proc_Start(gUnknown_08614134, PROC_TREE_VSYNC);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk34 = c;
    proc->unk38 = d;
    proc->unk3c = e;
    proc->unk40 = f;

    proc->unk48 = 0;
    proc->unk44 = 0;
}
asm(".global sub_08073574\n.thumb_set sub_08073574, StartBgWave\n");
