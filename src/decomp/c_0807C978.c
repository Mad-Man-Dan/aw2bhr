#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C978.
 * sub_0807C978 @ 0x0807C978
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Family F075, third member -- see StartTargetPickList. Both `varies` entries move
 * together here: a different reset routine (CoSelect_SetupBlend, which likewise reads
 * no argument register) and Proc_Start instead of Proc_StartBlocking. */

void WarRoomMapSelected_0807C979(ProcPtr parent)
{
    CoSelect_SetupBlend();
    Proc_Start(ProcScr_CoSelect, parent);
}

asm(".global sub_0807C978\n.thumb_set sub_0807C978, WarRoomMapSelected_0807C979\n");
