#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049170.
 * sub_08049170 @ 0x08049170
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

struct Unk8049170
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
};

void ShopList_ResetRise(struct Unk8049170 *s)
{
    s->unk64 = 0;
}

asm(".global sub_08049170\n.thumb_set sub_08049170, ShopList_ResetRise\n");
