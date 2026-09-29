#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047094.
 * TerrainInfoWindow_OnEnd @ 0x08047094
 */

/* MATCHED. Byte-for-byte the same function as UnitInfoPanel_Init -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void TerrainInfoWindow_OnEnd(void)
{
    s16 i;

    for (i = 0; i <= 0x3FF; i++)
        gBG0TilemapBuffer[i] = 0;

    BG_EnableSyncBG0();
    DisableWindow0AndResetMapLayers(0, 0, 0, 0);
    RedrawUnitLayer();
    RedrawUnitIconLayer();
}

asm(".global sub_08047094\n.thumb_set sub_08047094, TerrainInfoWindow_OnEnd\n");
