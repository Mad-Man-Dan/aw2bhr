#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028ED0.
 * sub_08028ED0 @ 0x08028ED0
 */

#include "proc.h"

void StartRangeSpread(ProcPtr parent)
{
    Proc_StartBlocking(gUnknown_08499FEC, parent);
}
asm(".global sub_08028ED0\n.thumb_set sub_08028ED0, StartRangeSpread\n");
