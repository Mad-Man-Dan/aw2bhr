#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035224.
 * sub_08035224 @ 0x08035224, sub_080352B4 @ 0x080352B4, sub_08035354 @ 0x08035354
 */

void InitSnowParticles(void)
{
    s16 i;
    s16 j;

    CpuCopyAuto(gUnknown_0809169C, (void *)0x06012E00, 0x60);

    for (i = 0; i <= 0x1f; i++)
    {
        gUnknown_02027C68[i].unk00 = (GetNextRandomNumber() % 0xff) << 8;
        gUnknown_02027C68[i].unk02 = (GetNextRandomNumber() % 0xff) << 8;
        j = i % 0x10;
        gUnknown_02027C68[i].unk04 = gUnknown_08090E40[j * 3];
        gUnknown_02027C68[i].unk06 = gUnknown_08090E40[j * 3 + 1];
        gUnknown_02027C68[i].unk08 = gUnknown_08090E40[j * 3 + 2];
    }
}
asm(".global sub_08035224\n.thumb_set sub_08035224, InitSnowParticles\n");

/* The second loop's bound must be reached through the SAME local the loop is
 * seeded from (`i = k * 0x10;` then `j < i + 0x10`), not written out again as
 * `k * 0x10 + 0x10`. Spelling the bound from k a second time makes gcc emit
 * the bound's `lsls #4` AHEAD of the loop variable's `lsls #0x14`, which is
 * the whole of an otherwise size-exact 93.1% near-miss -- same instructions,
 * same count, wrong order. Found by decomp-permuter. */
void UpdateSnowParticles(void)
{
    s16 i;
    s16 j;
    int k;

    for (i = 0; i <= 0x1f; i++)
    {
        gUnknown_02027C68[i].unk00 = gUnknown_02027C68[i].unk00 + gUnknown_02027C68[i].unk04;
        gUnknown_02027C68[i].unk02 = gUnknown_02027C68[i].unk02 + gUnknown_02027C68[i].unk06;
    }

    k = gGameClock & 1;
    i = k * 0x10;

    for (j = i; j < i + 0x10; j++)
    {
        PutOamLo(((gUnknown_02027C68[j].unk00 >> 8) - gMap->scrollX) & 0xff,
                     ((gUnknown_02027C68[j].unk02 >> 8) - gMap->scrollY) & 0xff,
                     gUnknown_0849BD98,
                     gUnknown_02027C68[j].unk08);
    }
}
asm(".global sub_080352B4\n.thumb_set sub_080352B4, UpdateSnowParticles\n");

void InitRainParticles(void)
{
    s16 i;
    s16 j;

    CpuCopyAuto(gUnknown_080916FC, (void *)0x06012E60, 0x60);

    for (i = 0; i <= 0x1f; i++)
    {
        gUnknown_02027DE8[i].unk00 = (GetNextRandomNumber() % 0xff) << 8;
        gUnknown_02027DE8[i].unk02 = (GetNextRandomNumber() % 0xff) << 8;
        j = i % 0x10;
        gUnknown_02027DE8[i].unk04 = gUnknown_08090E40[j * 3] * 3;
        gUnknown_02027DE8[i].unk06 = gUnknown_08090E40[j * 3 + 1] * 8;
        gUnknown_02027DE8[i].unk08 = gUnknown_08090E40[j * 3 + 2];
    }
}
asm(".global sub_08035354\n.thumb_set sub_08035354, InitRainParticles\n");
