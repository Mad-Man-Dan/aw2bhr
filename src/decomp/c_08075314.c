#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075314.
 * sub_08075314 @ 0x08075314, sub_08075340 @ 0x08075340
 */

#include "hardware.h"

void WorldMapSelectionFrame_LoadGraphics(void)
{
    Decompress(gUnknown_081D1398, (void *)0x06000800);
    Decompress(gUnknown_081D13E0, gBG1TilemapBuffer);
    BG_EnableSyncBG1();
}
asm(".global sub_08075314\n.thumb_set sub_08075314, WorldMapSelectionFrame_LoadGraphics\n");

void SetWorldMapSelectionFrameColor(int a)
{
    const u16 *p = gUnknown_081D1624;
    int i;

    i = (a & 0x1F) >> 1;
    gPal[0x59] = p[i];
    EnablePaletteSync();
}
asm(".global sub_08075340\n.thumb_set sub_08075340, SetWorldMapSelectionFrameColor\n");
