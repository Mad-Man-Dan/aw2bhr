#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08022974.
 * sub_08022974 @ 0x08022974
 */

/* Two calls. gUnknown_030033EC arrives as a plain `ldrh` with no shift pair,
 * which is what ApplyArmyWindowFramePalette's u16 parameter costs -- a narrower or signed
 * parameter would have added one. */
void sub_08022974(void)
{
    LoadTilePoolPalette(0, 0xA);
    ApplyArmyWindowFramePalette(gUnknown_030033EC);
}
