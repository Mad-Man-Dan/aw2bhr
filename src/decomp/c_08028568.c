#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028568.
 * FinalizeMatchResult @ 0x08028568
 */

void FinalizeMatchResult(void)
{
    ClearTeammateDefeats();
    RunMapEventsAtMatchEnd();
    gUnknown_030032D8 = 0x12;
}

asm(".global sub_08028568\n.thumb_set sub_08028568, FinalizeMatchResult\n");
