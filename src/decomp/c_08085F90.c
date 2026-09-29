#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085F90.
 * sub_08085F90 @ 0x08085F90
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void MapSelect_Idle(ProcPtr proc)
{
}

asm(".global sub_08085F90\n.thumb_set sub_08085F90, MapSelect_Idle\n");
