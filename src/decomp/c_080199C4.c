#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080199C4.
 * sub_080199C4 @ 0x080199C4
 */

void ClearChoiceResult(void)
{
    gUnknown_03002EE4 = 0;
}
asm(".global sub_080199C4\n.thumb_set sub_080199C4, ClearChoiceResult\n");
