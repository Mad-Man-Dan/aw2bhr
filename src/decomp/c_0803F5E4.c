#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F5E4.
 * sub_0803F5E4 @ 0x0803F5E4
 */

void LoadMapObjectGraphics(int a1, int a2)
{
    u8 *src;
    u8 *table;

    if (a1 != 1)
    {
        src = gUnknown_080CFFC4;
        table = gUnknown_080D16C4;
    }
    else
    {
        src = gUnknown_080D0B44;
        table = gUnknown_080D1BC4;
    }
    RegisterDataMove(src, (void *)(0x06010000 + ((a2 & 0x3FF) << 5)), 0xB80);
    CpuFastSet(table + ((((GetPlayerCoCountry(1) - 1) << 3) & 0x3FF) << 5),
               (void *)(0x06010000 + (((a2 + 0x5C) & 0x3FF) << 5)), 0x40);
    CpuFastSet(table + ((((GetPlayerCoCountry(2) - 1) << 3) & 0x3FF) << 5),
               (void *)(0x06010000 + (((a2 + 0x64) & 0x3FF) << 5)), 0x40);
    CpuFastSet(table + ((((GetPlayerCoCountry(3) - 1) << 3) & 0x3FF) << 5),
               (void *)(0x06010000 + (((a2 + 0x6C) & 0x3FF) << 5)), 0x40);
    CpuFastSet(table + ((((GetPlayerCoCountry(4) - 1) << 3) & 0x3FF) << 5),
               (void *)(0x06010000 + (((a2 + 0x74) & 0x3FF) << 5)), 0x40);
    LoadInventionGraphics(a1, a2);
}
asm(".global sub_0803F5E4\n.thumb_set sub_0803F5E4, LoadMapObjectGraphics\n");
