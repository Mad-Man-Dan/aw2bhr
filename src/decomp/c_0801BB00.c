#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BB00.
 * sub_0801BB00 @ 0x0801BB00
 */

void SetIRQHandler(int index, void *handler)
{
    gUnknown_03002FE0[index] = handler;
}
asm(".global sub_0801BB00\n.thumb_set sub_0801BB00, SetIRQHandler\n");
