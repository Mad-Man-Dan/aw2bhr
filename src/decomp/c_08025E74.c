#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025E74.
 * ClearAllUnits @ 0x08025E74
 */

void ClearAllUnits(void)
{
    int i;

    for (i = 0; i < 256; i++)
        gUnits[i].type = 0;
    gUnknown_030032C0 = 0;
}

asm(".global sub_08025E74\n.thumb_set sub_08025E74, ClearAllUnits\n");
