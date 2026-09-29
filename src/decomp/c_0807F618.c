#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F618.
 * sub_0807F618 @ 0x0807F618
 */

/* Existence predicate for gUnknown_08616710, the script sub_0807F82C hands to
 * Proc_StartBlocking. Identical to IsCircleWipeActive but for the script. */

#include "proc.h"

int IsEndCoSelectRunning(void)
{
    return Proc_Find(gUnknown_08616710) != 0;
}
asm(".global sub_0807F618\n.thumb_set sub_0807F618, IsEndCoSelectRunning\n");
