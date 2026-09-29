#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BCF4.
 * sub_0801BCF4 @ 0x0801BCF4
 */

u16 GetPrimaryOAMSize(void)
{
    return gOamTransferHead.objectCount;
}
asm(".global sub_0801BCF4\n.thumb_set sub_0801BCF4, GetPrimaryOAMSize\n");
