#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BF08.
 * sub_0803BF08 @ 0x0803BF08
 */

struct Unk0803BF08 /* >= 0x0c */
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
};

void sub_0803BF08(struct Unk0803BF08 *p)
{
    p->unk08 = 0;
}
