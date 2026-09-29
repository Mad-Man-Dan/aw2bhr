#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807C55C.
 * sub_0807C55C @ 0x0807C55C, sub_0807C570 @ 0x0807C570
 */

#include "proc.h"

void StartMissionTitle(void)
{
    Proc_Start(gUnknown_086164A0, PROC_TREE_3);
}
asm(".global sub_0807C55C\n.thumb_set sub_0807C55C, StartMissionTitle\n");

int IsMissionTitleRunning(void)
{
    return Proc_Find(gUnknown_086164A0) != 0;
}
asm(".global sub_0807C570\n.thumb_set sub_0807C570, IsMissionTitleRunning\n");
