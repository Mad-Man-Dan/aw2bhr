#include "global.h"
#include "hardware.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803768C.
 * sub_0803768C @ 0x0803768C, sub_080376DC @ 0x080376DC
 */

void DrawMapPreviewTiles(int a, int b, int c, int d)
{
    gUnknown_0300057C = d;
    ApplyPaletteExt(gUnknown_081253F0, (u16)(d * 0x20), 0x20);
    RenderMapPreviewToVram((void *)(a + (c & 0x3ff) * 32));
    FillMapPreviewTilemap((u16 *)b, (d << 12) | c);
}
asm(".global sub_0803768C\n.thumb_set sub_0803768C, DrawMapPreviewTiles\n");

void DrawMapPreviewToBg(void *a, int b, int c, int d, int e, int f)
{
    int tile;
    u8 *p;

    tile = (u16)b;

    gUnknown_0300057C = f;
    ApplyPaletteExt(gUnknown_081253F0, (u16)(f * 0x20), 0x20);

    p = (u8 *)a + (tile & 0x3ff) * 32;
    sub_0801B6EC(p);
    sub_0801B6FC(p);

    FillMapPreviewTilemap(BG_GetMapTilePointer(c, d, e), (f << 12) | tile);
    BG_EnableSync(c);
}
asm(".global sub_080376DC\n.thumb_set sub_080376DC, DrawMapPreviewToBg\n");
