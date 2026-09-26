#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080143EC.
 * sub_080143EC @ 0x080143EC
 */

struct Unk080143EC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 filler_2e[0x06];
    /* 0x34 */ u16 unk34;
};

void sub_080143EC(struct Unk080143EC *s, u16 *dst)
{
    dst[0] = s->unk34 | s->unk2c;
    dst[0x20] = (s->unk34 + 1) | s->unk2c;
}
