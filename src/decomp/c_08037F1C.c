#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037F1C.
 * sub_08037F1C @ 0x08037F1C
 */

/* Two independent `if`s and not an if/else: the second test re-loads
 * gGameClock from the same pool word the first one kept live in r2,
 * which is only possible if the first `if` falls through into it. */
void Bg3AutoScroll_Loop(void)
{
    if (gGameClock & 1)
        gUnknown_0300200C++;
    if (DivRem(gGameClock, 3) == 0)
        gUnknown_03002000--;
}
asm(".global sub_08037F1C\n.thumb_set sub_08037F1C, Bg3AutoScroll_Loop\n");
