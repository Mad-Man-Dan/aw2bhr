#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065960.
 * sub_08065960 @ 0x08065960
 */

void MatchSetupBg3AutoScroll_Loop(void)
{
    if (gGameClock & 1)
    {
        gUnknown_0300200C++;
        gUnknown_03002000--;
    }
}
asm(".global sub_08065960\n.thumb_set sub_08065960, MatchSetupBg3AutoScroll_Loop\n");
