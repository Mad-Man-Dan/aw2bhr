#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037F80.
 * sub_08037F80 @ 0x08037F80
 */

#include "proc.h"

void StartEndOfGameProc(void)
{
    Proc_Start(gUnknown_0849D56C, PROC_TREE_3);
}
asm(".global sub_08037F80\n.thumb_set sub_08037F80, StartEndOfGameProc\n");
