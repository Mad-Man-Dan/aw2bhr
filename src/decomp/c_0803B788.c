#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B788.
 * sub_0803B788 @ 0x0803B788
 */

#include "proc.h"

void StartMusicPauseFade(void)
{
    Proc_Start(gUnknown_0849E7B8, PROC_TREE_3);
}
asm(".global sub_0803B788\n.thumb_set sub_0803B788, StartMusicPauseFade\n");
