#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080780D0.
 * sub_080780D0 @ 0x080780D0
 */

#include "proc.h"

void StartWorldMapAfterMission(void)
{
    Proc_Start(gUnknown_08614894, PROC_TREE_3);
}
asm(".global sub_080780D0\n.thumb_set sub_080780D0, StartWorldMapAfterMission\n");
