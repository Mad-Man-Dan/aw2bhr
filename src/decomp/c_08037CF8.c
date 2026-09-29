#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037CF8.
 * sub_08037CF8 @ 0x08037CF8
 */

struct Unk37CF8
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u16 unk1e;
};

void LoadAndDrawMapPreview(struct Unk37CF8 *a)
{
    LoadMapData(a->unk1e);
    DrawMapPreviewFromBlob((void *)0x06000020);
    LayoutMapPreviewTilemap();

    gUnknown_03002B34 = ((u8 *)gUnknown_03003F68)[0] * 2 - 0xa8;
    gUnknown_03002F18 = ((u8 *)gUnknown_03003F68)[1] * 2 - 0x60;
    gUnknown_03002B40 = 0x5d;
    gUnknown_03002B4C = 0xf0;
    gUnknown_03002EFC = 0;
    gUnknown_03002B44 = 0xa0;
    sub_0802481C();
}
asm(".global sub_08037CF8\n.thumb_set sub_08037CF8, LoadAndDrawMapPreview\n");
