#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032D4C.
 * sub_08032D4C @ 0x08032D4C
 */

#include "proc.h"

void StartLinkMapPick(ProcPtr parent)
{
    Proc_Start(gUnknown_0849B6B0, parent);
}
asm(".global sub_08032D4C\n.thumb_set sub_08032D4C, StartLinkMapPick\n");
