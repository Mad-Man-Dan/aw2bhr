#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B774.
 * sub_0803B774 @ 0x0803B774
 */

#include "proc.h"

void StartMusicDuckRelease(void)
{
    Proc_Start(gUnknown_0849E7A0, PROC_TREE_3);
}
asm(".global sub_0803B774\n.thumb_set sub_0803B774, StartMusicDuckRelease\n");
