#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08048558.
 * sub_08048558 @ 0x08048558
 */

void ClearBg0AndRebuildUnitLayers(void)
{
    RebuildMapUnitLayers2();
    ClearBg0Tilemap();
    BG_EnableSyncBG0();
}
asm(".global sub_08048558\n.thumb_set sub_08048558, ClearBg0AndRebuildUnitLayers\n");
