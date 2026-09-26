#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066338.
 * sub_08066338 @ 0x08066338
 */

struct Unk08066338 /* >= 0x28 */
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ u16 unk26;
};

void sub_08066338(struct Unk08066338 *p)
{
    p->unk26 = 3;
}
