#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037E64.
 * sub_08037E64 @ 0x08037E64
 */

void sub_08037E64(void)
{
    BuildMapListForMode(gPlaySt.gameMode);
    sub_08037DC8();
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
}
