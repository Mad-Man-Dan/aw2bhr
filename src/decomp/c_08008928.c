#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08008928.
 * sub_08008928 @ 0x08008928, RemoveUnitAt @ 0x08008A8C
 */

#include "hardware.h"

int DesignRoomPlaceUnitAtCursor(void)
{
    int x;
    int y;
    int v;
    int q;
    int f;
    int result;

    result = 0;

    gUnknown_030033EC = f = gActiveMap->unitArmy;
    gUnknown_03003F2C = (f - 1) << 6;

    x = gActiveMap->cursorX;
    y = gActiveMap->cursorY;

    if (gActiveMap->cursorUnit == 0x19)
    {
        if (RemoveUnitAt(1, x, y))
        {
            RebuildMapUnitLayers2();
            result = 2;
        }
    }
    else
    {
        v = GetUnitTypeAt(x, y);

        if (v != gActiveMap->cursorUnit)
        {
            s8 *costs;
            int idx;
            int c;

            if (v == 0x19)
            {
                v = -1;
            }
            else if (CountArmyUnits(gActiveMap->unitArmy) > 0x31
                     && (v >> 6) + 1 != gActiveMap->unitArmy)
            {
                return -1;
            }

            /* The movement chart: the cost of entering each terrain, 32 terrain entries
             * per movement type, so the index is terrain + movementType * 32. A
             * cost of -1 means the unit cannot go there. movementChart holds three
             * such tables and this is the first. */
            costs = gUnknown_085D3DD0[1].power[0].movementChart[0];

            idx = gMap->rowOffset[y] + x;
            c = (gMap->terrain[idx] & 0x1f)
                + gUnknown_085D5ABC[gActiveMap->cursorUnit & 0x3f].movementType * 32;

            q = costs[c];

            if (q != -1)
            {
                if (v > 0)
                    RemoveUnitAt(0, x, y);

                if (CountArmyUnits(gActiveMap->unitArmy) <= 0x31
                    && CreateUnitAt(x, y, gActiveMap->cursorUnit & 0x3f))
                    result = 1;
            }
            else
            {
                result = q;
            }
        }
    }

    if (result > 0)
        DesignRoomCountArmyUnits();

    return result;
}
asm(".global sub_08008928\n.thumb_set sub_08008928, DesignRoomPlaceUnitAtCursor\n");

int RemoveUnitAt(int mode, int x, int y)
{
    struct Unit *e;
    int idx;
    int result;

    result = 0;

    idx = gMap->rowOffset[y] + x;

    if (gMap->unit[idx] != 0)
    {
        e = &gUnits[gMap->unit[idx]];

        if (mode != 0)
        {
            SetWinEnable(1, 0, 0);

            gUnknown_03002B40 = 0;
            gUnknown_03002EFC = 0;
            gUnknown_03002B4C = 0;
            gUnknown_03002B44 = 0;

            gUnknown_030030A4.bits.win0_enable_blend = 0;
            gUnknown_030030DC.bits.win1_enable_blend = 1;

            gUnknown_030030A4.bits.win0_enable_bg0 = 1;
            gUnknown_030030A4.bits.win0_enable_bg1 = 1;
            gUnknown_030030A4.bits.win0_enable_bg2 = 1;
            gUnknown_030030A4.bits.win0_enable_bg3 = 1;
            gUnknown_030030A4.bits.win0_enable_obj = 1;

            gUnknown_030030DC.bits.win0_enable_bg0 = 1;
            gUnknown_030030DC.bits.win0_enable_bg1 = 1;
            gUnknown_030030DC.bits.win0_enable_bg2 = 1;
            gUnknown_030030DC.bits.win0_enable_bg3 = 1;
            gUnknown_030030DC.bits.win0_enable_obj = 1;

            StartUnitDestroyed(e);
        }
        else
        {
            e->type = 0;
        }

        DesignRoomCountArmyUnits();

        result = 1;
    }

    return result;
}

asm(".global sub_08008A8C\n.thumb_set sub_08008A8C, RemoveUnitAt\n");
