#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080849BC.
 * sub_080849BC @ 0x080849BC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Family F017, exemplar src/decomp/c_080198AC.c: a word `str` of a small
 * constant into a scalar global, no frame. gUnknown_03005940 is already
 * declared `int`. */

void CoInfoScreen_ResetPage(void)
{
    gUnknown_03005940 = 0;
}

asm(".global sub_080849BC\n.thumb_set sub_080849BC, CoInfoScreen_ResetPage\n");
