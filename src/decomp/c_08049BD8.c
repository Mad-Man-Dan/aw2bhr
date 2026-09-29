#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049BD8.
 * sub_08049BD8 @ 0x08049BD8
 */

#include "proc.h"

void StartShopScreen(void)
{
    Proc_Start(ProcScr_BattleMaps, PROC_TREE_3);
}
asm(".global sub_08049BD8\n.thumb_set sub_08049BD8, StartShopScreen\n");
