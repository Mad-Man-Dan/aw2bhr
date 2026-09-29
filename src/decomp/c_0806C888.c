#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C888.
 * sub_0806C888 @ 0x0806C888
 */

#include "proc.h"

int IsCreditsRunning(void)
{
    return Proc_Find(gUnknown_08581AC8) != 0;
}
asm(".global sub_0806C888\n.thumb_set sub_0806C888, IsCreditsRunning\n");
