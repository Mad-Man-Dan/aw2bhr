#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045EAC.
 * sub_08045EAC @ 0x08045EAC
 */

/* Three sequential calls, all results discarded. */
void MapEventFx_ShakeAndFlashWithSfx1D5(void)
{
    StartScreenShake(2, 0x8C, 0);
    StartWhiteFlash(2, 0, 0xB4, 0);
    PlayMusicOrSfx2(0x1D5);
}
asm(".global sub_08045EAC\n.thumb_set sub_08045EAC, MapEventFx_ShakeAndFlashWithSfx1D5\n");
