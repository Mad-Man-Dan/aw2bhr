#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080324C4.
 * sub_080324C4 @ 0x080324C4
 */

#include "hardware.h"

void LinkScreenInit(int a1, int a2, u8 a3)
{
    int i;
    int zero;

    LoadBg1WindowFrame(0);
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    LinkC3_EndScreenProcs();
    sub_080733B8();

    SetBgScrollShadow(0, 0xFFD0, 8);
    SetBgScrollShadow(3, 0, 0);

    ResetWindowShadows();

    if ((gGameClock & 1) || a2 == -1)
    {
        ApplyPaletteExt(gUnknown_081D8A14, 0, 0x20);
        Decompress(gUnknown_081D3EE8,
                   (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000));
    }
    else
    {
        ApplyPaletteExt(gUnknown_081D8A34, 0, 0x20);
        Decompress(gUnknown_081D6458,
                   (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000));
    }

    zero = 0;
    CpuFastSet(&zero, (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06003000),
               0x01000008);

    for (i = 0; i < 0x80; i++)
    {
        gBG3TilemapBuffer[i + 0x200] = 0x180;
        gBG3TilemapBuffer[i] = 0x180;
    }

    for (i = 0x80; i < 0x200; i++)
        gBG3TilemapBuffer[i] = i - 0x80;

    StartHeaderBanner(gUnknown_0849B644, gUnknown_02010C50, 0xec, 0xf, 0, a3, a1);

    if (a2 == -1)
    {
        SetBgScrollShadow(0, 0, 0);
        ApplyWindowFramePalette(0, 3);
        Decompress(gUnknown_081D2660, (void *)0x06006280);
        sub_08032484(gBG0TilemapBuffer + 0x221);
    }
    else
    {
        ApplyPaletteExt(gUnknown_081320AC, 0x60, 0x20);
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, a2, 2);
    }
}
asm(".global sub_080324C4\n.thumb_set sub_080324C4, LinkScreenInit\n");
