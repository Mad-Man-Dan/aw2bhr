#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C78C.
 * sub_0801C78C @ 0x0801C78C
 */

struct Unk_0801C78C_Obj
{
    /* 0x00 */ u8 filler_00[0x22];
    /* 0x22 */ u16 unk22;
};
struct Unk_0801C78C_Proc
{
    /* 0x00 */ u8 filler_00[0x50];
    /* 0x50 */ struct Unk_0801C78C_Obj *unk50;
    /* 0x54 */ u32 unk54;
    /* 0x58 */ u32 unk58;
};

void sub_0801C78C(struct Unk_0801C78C_Proc *proc, u32 a, u32 b, u16 c)
{
    proc->unk54 = a;
    proc->unk58 = b;
    proc->unk50->unk22 = c;
}
