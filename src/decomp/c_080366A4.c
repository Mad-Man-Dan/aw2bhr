#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080366A4.
 * sub_080366A4 @ 0x080366A4
 */

/* Restores the ordinary frame callbacks: the VBlank-side hook first, then the
 * main-loop hook AgbMain calls forever from 0x08036D1C. */
void InitMainFrameCallbacks(void)
{
    sub_08011B18();
    SetVBlankCallback(DefaultVBlankCallback);
    SetMainLoopCallback(DefaultMainLoopCallback);
}

asm(".global sub_080366A4\n.thumb_set sub_080366A4, InitMainFrameCallbacks\n");
