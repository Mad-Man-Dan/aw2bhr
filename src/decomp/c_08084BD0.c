#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084BD0.
 * sub_08084BD0 @ 0x08084BD0
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void CoInfoScreen_Idle(ProcPtr proc)
{
}

asm(".global sub_08084BD0\n.thumb_set sub_08084BD0, CoInfoScreen_Idle\n");
