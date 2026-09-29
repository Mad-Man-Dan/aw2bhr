#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031CD4.
 * sub_08031CD4 @ 0x08031CD4
 */

void LinkClearTransferProgress(void)
{
    gUnknown_0849B060->unk0a = 0;
}
asm(".global sub_08031CD4\n.thumb_set sub_08031CD4, LinkClearTransferProgress\n");
