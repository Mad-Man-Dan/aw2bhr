#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08060DAC.
 * AiBuildPropertyList @ 0x08060DAC
 */

#include "map.h"

int AiBuildPropertyList(void)
{
    int n;
    int count;
    int i;
    int x;
    int y;

    n = 0;
    count = 0;
    gUnknown_030045C8 = 0;

    for (i = 0; gUnknown_084995A0[i].unk00 != 0xFF; i++)
    {
        if (IsOwnFactoryCell(gUnknown_084995A0[i].unk01, gUnknown_084995A0[i].unk02) == 1
            && gMap->unit[gMap->rowOffset[gUnknown_084995A0[i].unk02] + gUnknown_084995A0[i].unk01] == 0
            && gUnknown_085767F2[gMap->terrain[gMap->rowOffset[gUnknown_084995A0[i].unk02] + gUnknown_084995A0[i].unk01] & 0x1f] != 0)
        {
            gUnknown_085766E4[n].unk00 = gUnknown_084995A0[i].unk01;
            gUnknown_085766E4[n].unk01 = gUnknown_084995A0[i].unk02;
            gUnknown_085766E4[n].unk02 = gUnknown_085767F2[gMap->terrain[gMap->rowOffset[gUnknown_084995A0[i].unk02] + gUnknown_084995A0[i].unk01] & 0x1f];
            gUnknown_085766E4[n].unk03 = 0x7F;
            n++;
            count++;
        }
    }

    gUnknown_085766E4[n].unk00 = 0xFF;
    return count;
}

asm(".global sub_08060DAC\n.thumb_set sub_08060DAC, AiBuildPropertyList\n");
