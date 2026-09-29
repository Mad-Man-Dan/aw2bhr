#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803BEF8.
 * sub_0803BEF8 @ 0x0803BEF8
 */

void Versus_ResetCoPickIndex(void)
{
    gUnknown_0849ECDC->unk01 = 0;
}
asm(".global sub_0803BEF8\n.thumb_set sub_0803BEF8, Versus_ResetCoPickIndex\n");
