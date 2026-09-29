#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080673D0.
 * sub_080673D0 @ 0x080673D0
 */

#include "proc.h"
struct Unk673D0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x38);
    /* 38 */ u32 unk38;
    /* 3c */ u32 unk3c;
};

void StartBlendRampWhite0To16(u32 a, u32 b, ProcPtr parent)
{
    struct Unk673D0Proc *proc = Proc_Start(gUnknown_08580EAC, parent);

    proc->unk2c = a;
    proc->unk38 = 0;
    proc->unk3c = b;
}
asm(".global sub_080673D0\n.thumb_set sub_080673D0, StartBlendRampWhite0To16\n");
