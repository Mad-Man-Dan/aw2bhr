#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019560.
 * sub_08019560 @ 0x08019560
 */

void EndTextBoxesIfSkipping(void)
{
    if (gUnknown_03002514 == 1)
        sub_08014878();
}
asm(".global sub_08019560\n.thumb_set sub_08019560, EndTextBoxesIfSkipping\n");
