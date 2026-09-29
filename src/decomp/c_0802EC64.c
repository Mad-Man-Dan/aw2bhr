#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802EC64.
 * sub_0802EC64 @ 0x0802EC64
 */

/* gUnknown_03000578 is volatile: the incremented word is stored and then the
 * very same address is re-`ldr`ed to index the table. A plain u32 CSEs that
 * reload away and comes out two instructions shorter.
 */

void SioStepSendDelay(void)
{
    gUnknown_03000578++;

    if (gUnknown_08090C44[gUnknown_03000578] == 0)
        gUnknown_03000578 = 0;
}
asm(".global sub_0802EC64\n.thumb_set sub_0802EC64, SioStepSendDelay\n");
