#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047F44.
 * sub_08047F44 @ 0x08047F44
 */

struct Unk08047F44
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ u16 unk24;
    /* 0x26 */ u16 unk26;
};

void sub_08047F44(struct Unk08047F44 *p)
{
    p->unk26 = 0x31;
    p->unk24 = 0x10;
}
