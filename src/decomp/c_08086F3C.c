#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086F3C.
 * sub_08086F3C @ 0x08086F3C
 */

#include "hardware.h"

void BuildMapSelectPreviewNow(int a1)
{
    u8 *p;
    int i, j, k;

    ClearBg1Tilemap();
    p = (u8 *)&gUnknown_02027F74;
    p += 4;

    if (p[a1] <= 0xb3 && (u8)IsCampaignMapUnlocked(p[a1]))
    {
        LoadMapData(p[a1]);
        sub_0801B6EC((void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06004000));
        sub_0801B6FC((void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06004000));
        FillMapPreviewTilemap(gBG1TilemapBuffer, 0x6200);
        gUnknown_03005918 = ((u8 *)gUnknown_03003F68)[0];
        gUnknown_030058F4 = ((u8 *)gUnknown_03003F68)[1];
        sub_0802481C();
    }
    else if (p[a1] > 0xb3)
    {
        Decompress(gUnknown_0823FD7C,
                   (void *)(gUnknown_03001FE8.bits.chr_block * 0x4000 + 0x06004000));
        k = 0;
        for (i = 0; i <= 7; i++)
            for (j = 0; j <= 9; j++)
                gBG1TilemapBuffer[i * 32 + j] = (k++ + 0x200) | 0x6000;
        gUnknown_03005918 = 0x14;
        gUnknown_030058F4 = 0x10;
    }
    BG_EnableSyncBG1();
}
asm(".global sub_08086F3C\n.thumb_set sub_08086F3C, BuildMapSelectPreviewNow\n");
