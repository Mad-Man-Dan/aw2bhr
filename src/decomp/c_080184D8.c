#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080184D8.
 * sub_080184D8 @ 0x080184D8
 */

struct Unk80184D8
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
};

void sub_080184D8(struct Unk80184D8 *s)
{
    s->unk08 = 0;
}
