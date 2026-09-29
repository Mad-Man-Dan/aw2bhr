#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08081358.
 * sub_08081358 @ 0x08081358
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void MainMenuCarousel_Idle(void)
{
}

asm(".global sub_08081358\n.thumb_set sub_08081358, MainMenuCarousel_Idle\n");
