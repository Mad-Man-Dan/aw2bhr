#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FD80.
 * sub_0803FD80 @ 0x0803FD80
 */

void LoadInventionGraphics(int a1, int a2)
{
    u8 *src;

    src = gUnknown_085C77A0[gPlaySt.mapID].tileGraphic4x4;
    Decompress(gUnknown_080D24E0, (void *)(0x06010000 + (((a2 + 0x7C) & 0x3FF) << 5)));
    if (FindInventionOfKind(5) != NULL)
    {
        if (IsHardCampaignMode())
            Decompress(gUnknown_080D2AE8, (void *)(0x06010000 + (((a2 + 0xA0) & 0x3FF) << 5)));
        else
            Decompress(gUnknown_080D2AE8, (void *)(0x06010000 + (((a2 + 0xC4) & 0x3FF) << 5)));
    }
    if (FindInventionOfKind(7) != NULL)
        src = gUnknown_080D22C4;
    if (FindInventionOfKind(2) != NULL)
    {
        src = gUnknown_080D3268;
        ApplyPaletteExt(gUnknown_080D3FC4, 0xE0 << 2, 0x20);
    }
    if (src != NULL)
        Decompress(src, (void *)(0x06010000 + (((a2 + 0xE8) & 0x3FF) << 5)));
}
asm(".global sub_0803FD80\n.thumb_set sub_0803FD80, LoadInventionGraphics\n");
