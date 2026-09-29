#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011B18.
 * sub_08011B18 @ 0x08011B18
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void ClearVBlankHooks(void)
{
    u8 i;

    for (i = 0; i < 16; i++)
        gUnknown_03000000[i] = 0;
}

asm(".global sub_08011B18\n.thumb_set sub_08011B18, ClearVBlankHooks\n");
