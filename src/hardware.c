#include "global.h"
#include "hardware.h"

/* BG tilemap dirty tracking, flushing, clearing, and cell access. */

void BG_ClearSync(void)
{
    sModifiedBGs = 0;
}
asm(".global sub_08013AC8\n.thumb_set sub_08013AC8, BG_ClearSync\n");

void BG_EnableSync(u8 a1)
{
    sModifiedBGs |= 1 << a1;
}

void BG_EnableSyncBG0(void)
{
    BG_EnableSyncByMask(1);
}
asm(".global sub_08013AEC\n.thumb_set sub_08013AEC, BG_EnableSyncBG0\n");

void BG_EnableSyncBG1(void)
{
    BG_EnableSyncByMask(2);
}
asm(".global sub_08013AFC\n.thumb_set sub_08013AFC, BG_EnableSyncBG1\n");

void BG_EnableSyncBG2(void)
{
    BG_EnableSyncByMask(4);
}
asm(".global sub_08013B0C\n.thumb_set sub_08013B0C, BG_EnableSyncBG2\n");

void BG_EnableSyncBG3(void)
{
    BG_EnableSyncByMask(8);
}
asm(".global sub_08013B1C\n.thumb_set sub_08013B1C, BG_EnableSyncBG3\n");

void FlushBgTilemaps(void)
{
    if (sModifiedBGs & 1)
        CpuCopyAuto(gBG0TilemapBuffer,
            (void *)(gUnknown_03002B6C.bits.tm_block * 0x800 + 0x06000000), 0x800);

    if (sModifiedBGs & 2)
        CpuCopyAuto(gBG1TilemapBuffer,
            (void *)(gUnknown_03001FE8.bits.tm_block * 0x800 + 0x06000000), 0x800);

    if (sModifiedBGs & 4)
        CpuCopyAuto(gBG2TilemapBuffer,
            (void *)(gUnknown_030030B4.bits.tm_block * 0x800 + 0x06000000), 0x800);

    if (sModifiedBGs & 8)
        CpuCopyAuto(gBG3TilemapBuffer,
            (void *)(gUnknown_0300251C.bits.tm_block * 0x800 + 0x06000000), 0x800);

    sModifiedBGs = 0;
    gUnknown_03000048 = 0;
}

void ClearBg0Tilemap(void)
{
    u16 i;

    for (i = 0; i < 0x400; i++)
        gBG0TilemapBuffer[i] = 0;

    for (i = 0; i < 0x10; i++)
        *(u16 *)(0x06000000 + gUnknown_03002B6C.bits.chr_block * 0x4000 + i * 2) = 0;
}
asm(".global sub_08013C00\n.thumb_set sub_08013C00, ClearBg0Tilemap\n");

void ClearBg1Tilemap(void)
{
    u16 i;

    for (i = 0; i < 0x400; i++)
        gBG1TilemapBuffer[i] = 0;

    for (i = 0; i < 0x10; i++)
        *(u16 *)(0x06000000 + gUnknown_03001FE8.bits.chr_block * 0x4000 + i * 2) = 0;
}
asm(".global sub_08013C54\n.thumb_set sub_08013C54, ClearBg1Tilemap\n");

void ClearBg2Tilemap(void)
{
    u16 i;

    for (i = 0; i < 0x400; i++)
        gBG2TilemapBuffer[i] = 0x360;

    for (i = 0; i < 0x10; i++)
        *(u16 *)(0x0600D800 + gUnknown_030030B4.bits.chr_block * 0x4000 + i * 2) = 0;
}
asm(".global sub_08013CA8\n.thumb_set sub_08013CA8, ClearBg2Tilemap\n");

u16 *BG_GetMapTilePointer(int which, int x, int y)
{
    u16 **pp;

    switch (which)
    {
    default:
    case 0:
        pp = &gBG0TilemapBuffer;
        break;
    case 1:
        pp = &gBG1TilemapBuffer;
        break;
    case 2:
        pp = &gBG2TilemapBuffer;
        break;
    case 3:
        pp = &gBG3TilemapBuffer;
        break;
    }

    return *pp + y * 32 + x;
}
