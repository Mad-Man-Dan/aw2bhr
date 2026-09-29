#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080324C4.
 * sub_080324C4 @ 0x080324C4
 */

#include "hardware.h"

void sub_080324C4(int a1, int a2, u8 a3)
{
    int i;
    int zero;

    sub_0801A5B0(0);
    sub_08013C00();
    sub_08013C54();
    sub_08013CA8();
    sub_08013AEC();
    sub_08013AFC();
    sub_08013B0C();
    sub_08013B1C();
    sub_08034290();
    sub_080733B8();

    sub_08072C40(0, 0xFFD0, 8);
    sub_08072C40(3, 0, 0);

    sub_0801237C();

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

    sub_08073304(gUnknown_0849B644, gUnknown_02010C50, 0xec, 0xf, 0, a3, a1);

    if (a2 == -1)
    {
        sub_08072C40(0, 0, 0);
        sub_0802D5CC(0, 3);
        Decompress(gUnknown_081D2660, (void *)0x06006280);
        sub_08032484(gBG0TilemapBuffer + 0x221);
    }
    else
    {
        ApplyPaletteExt(gUnknown_081320AC, 0x60, 0x20);
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, a2, 2);
    }
}
