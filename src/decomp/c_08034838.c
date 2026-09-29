#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034838.
 * sub_08034838 @ 0x08034838
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void ClearSavingEnabled(void)
{
    gPlaySt.savingEnabled = 0;
}

asm(".global sub_08034838\n.thumb_set sub_08034838, ClearSavingEnabled\n");
