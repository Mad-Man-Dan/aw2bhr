#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F8D0.
 * sub_0807F8D0 @ 0x0807F8D0
 */

#include "proc.h"

void StartBlockWarRoomSelection(ProcPtr parent)
{
    Proc_Start(gUnknown_08616740, parent);
}
asm(".global sub_0807F8D0\n.thumb_set sub_0807F8D0, StartBlockWarRoomSelection\n");
