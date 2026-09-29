#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080353E8.
 * sub_080353E8 @ 0x080353E8
 */

void UpdateRainParticles(void)
{
    s16 i;
    s16 j;
    int n;
    int m;

    for (i = 0; i < 0x20; i++)
    {
        gUnknown_02027DE8[i].unk00 += gUnknown_02027DE8[i].unk04;
        gUnknown_02027DE8[i].unk02 += gUnknown_02027DE8[i].unk06;
    }

    n = gGameClock & 1;
    asm("" : "=r"(m) : "0"(n));

    for (j = n * 16; j < m * 16 + 16; j++)
    {
        PutOamLo(((gUnknown_02027DE8[j].unk00 >> 8) - gMap->scrollX) & 0xff,
                     ((gUnknown_02027DE8[j].unk02 >> 8) - gMap->scrollY) & 0xff,
                     &gUnknown_0849BDA0[gUnknown_02027DE8[j].unk08 * 4],
                     0);
    }
}
asm(".global sub_080353E8\n.thumb_set sub_080353E8, UpdateRainParticles\n");
