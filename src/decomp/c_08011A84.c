#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011A84.
 * sub_08011A84 @ 0x08011A84
 */

/* The reset for the 16-entry pointer list gUnknown_03002FA0 and its count.
 * Same loop as sub_08011B18, which does list B (gUnknown_03000000).
 */
void ClearVBlankCallbackQueue(void)
{
    u8 i;

    gUnknown_030030E8 = 0;

    for (i = 0; i < 16; i++)
        gUnknown_03002FA0[i] = 0;
}
asm(".global sub_08011A84\n.thumb_set sub_08011A84, ClearVBlankCallbackQueue\n");
