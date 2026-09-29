#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F710.
 * sub_0806F710 @ 0x0806F710
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

/* Sits immediately above the m4a/MP2K span, but takes its parent in r0 like
 * every other member of this family -- ordinary compiler output, not
 * m4a_asm.s. */
void StartSoundRoomBlocking(ProcPtr parent)
{
    Proc_StartBlocking(gUnknown_08582D74, parent);
}

asm(".global sub_0806F710\n.thumb_set sub_0806F710, StartSoundRoomBlocking\n");
