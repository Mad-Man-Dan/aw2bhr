#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B828.
 * sub_0803B828 @ 0x0803B828
 */

#include "proc.h"

void StartMainMenu(void)
{
    Proc_Start(ProcScr_MainMenu, PROC_TREE_3);
}
asm(".global sub_0803B828\n.thumb_set sub_0803B828, StartMainMenu\n");
