#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B698.
 * sub_0803B698 @ 0x0803B698
 */

struct UnkP3B698
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ u32 unk54;
    /* 0x58 */ u8 filler_58[0x0c];
    /* 0x64 */ u16 unk64;
};

void sub_0803B698(struct UnkP3B698 *proc)
{
    proc->unk64 = 0;
    proc->unk54 = 0x80;
}
