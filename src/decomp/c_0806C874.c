#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C874.
 * sub_0806C874 @ 0x0806C874
 */

#include "proc.h"

void StartCredits(void)
{
    Proc_Start(gUnknown_08581AC8, PROC_TREE_3);
}
asm(".global sub_0806C874\n.thumb_set sub_0806C874, StartCredits\n");
