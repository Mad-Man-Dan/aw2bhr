#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803ED54.
 * sub_0803ED54 @ 0x0803ED54
 */

struct Unk803ED54
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ struct Unk02028360 *unk2c;
};

void sub_0803ED54(struct Unk803ED54 *p)
{
    p->unk2c = gUnknown_02028360;
}
