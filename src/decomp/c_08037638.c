#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037638.
 * sub_08037638 @ 0x08037638
 */

void ShowMapPreview(int a, int b, int c, int d)
{
    AddVBlankHook((void *)AnimateMapPreviewPalette);
    StartMapPreviewPictureScript(a + ((c & 0x3ff) << 5));
    DrawMapPreviewTiles(a, b, c, d);
}
asm(".global sub_08037638\n.thumb_set sub_08037638, ShowMapPreview\n");
