#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060554.
 * sub_08060554 @ 0x08060554
 */

void AiExecutorFinishAfterBuy(void)
{
    gUnknown_03004780 = 2;
    gUnknown_030045D4 = 0;
}
asm(".global sub_08060554\n.thumb_set sub_08060554, AiExecutorFinishAfterBuy\n");
