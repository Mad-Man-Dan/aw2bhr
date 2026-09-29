#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013028.
 * sub_08013028 @ 0x08013028
 */

void SetSlotScriptFlag(void)
{
    gUnknown_03002F1C = 1;
}
asm(".global sub_08013028\n.thumb_set sub_08013028, SetSlotScriptFlag\n");
