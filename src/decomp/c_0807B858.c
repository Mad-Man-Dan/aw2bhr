#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B858.
 * sub_0807B858 @ 0x0807B858
 */

/* The gUnknown_03002B34 / gUnknown_03002F18 twin of sub_08078DD8 -- same guard
 * on bit 0 of gGameClock, same increment/decrement pair.
 * gUnknown_03002F18 is declared volatile for sub_0804BA4C's sake; a plain
 * `--` on it is byte-identical either way. */

void MissionTitle_ScrollBackdrop_Loop(void)
{
    if (gGameClock & 1)
    {
        gUnknown_03002B34++;
        gUnknown_03002F18--;
    }
}
asm(".global sub_0807B858\n.thumb_set sub_0807B858, MissionTitle_ScrollBackdrop_Loop\n");
