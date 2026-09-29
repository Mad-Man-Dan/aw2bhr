#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047078.
 * sub_08047078 @ 0x08047078
 */

void TerrainInfoWindow_DrawSprites(void)
{
    sub_08046A84(gUnknown_02028DD5, gUnknown_02028DD6);
}
asm(".global sub_08047078\n.thumb_set sub_08047078, TerrainInfoWindow_DrawSprites\n");
