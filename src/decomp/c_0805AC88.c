#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805AC88.
 * sub_0805AC88 @ 0x0805AC88
 */

void AiClearEscortTally(void)
{
    int i;

    for (i = 0; i < 0x40; i++)
        gUnknown_03004730[i] = 0;
}
asm(".global sub_0805AC88\n.thumb_set sub_0805AC88, AiClearEscortTally\n");
