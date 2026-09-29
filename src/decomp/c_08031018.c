#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031018.
 * sub_08031018 @ 0x08031018
 */

void LinkLobbyLoadGraphics(void)
{
    CopyToPaletteBufferNoSync(gUnknown_081D3E48, 0x300, 0x20);
    CopyToPaletteBufferNoSync(gUnknown_081D3E48, 0x320, 0x20);
    CopyToPaletteBufferNoSync(gUnknown_081D3E48, 0x340, 0x20);
    CopyToPaletteBufferNoSync(gUnknown_081D3E48, 0x360, 0x20);

    Decompress(gUnknown_081D3810, (void *)0x060114A0);
    InitTilePool(2, (void *)0x06010000, 0, 0x16);

    LoadTilePoolGraphic(0x50);
    LoadTilePoolGraphic(0x4f);
    LoadTilePoolGraphic(0x4a);
    LoadTilePoolGraphic(0x4b);
    LoadTilePoolGraphic(0x4c);
    LoadTilePoolGraphic(0x4d);

    ApplyPaletteExt(gUnknown_081320AC, 0x60, 0x20);
    ApplyPaletteExt(gUnknown_0849B0A0, 0xe0, 0x20);

    LoadBg1WindowFrame(0);

    gUnknown_0849B018->unk1e = 0x10;
    gUnknown_0849B060->unk00 = 0x13;
}
asm(".global sub_08031018\n.thumb_set sub_08031018, LinkLobbyLoadGraphics\n");
