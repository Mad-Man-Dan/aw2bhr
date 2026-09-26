#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012C1C.
 * sub_08012C1C @ 0x08012C1C
 */

struct Unk8012C30
{
    u8 unk00_0 : 2;
    u8 unk00_2 : 2;
    u8 unk00_4 : 4;
    u8 unk01_0 : 5;
    u8 unk01_5 : 1;
    u8 unk01_6 : 2;
};

void sub_08012C1C(struct Unk8012C30 *s, u32 value)
{
    s->unk01_0 = value >> 11;
}
