#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045FA4.
 * sub_08045FA4 @ 0x08045FA4
 */

void MapEventFx_ShakeScreenWithSfx23(void)
{
    StartScreenShake(2, 0x8c, 0);
    PlayMusicOrSfx2(0x23);
}
asm(".global sub_08045FA4\n.thumb_set sub_08045FA4, MapEventFx_ShakeScreenWithSfx23\n");
