#include "global.h"
#include "hardware.h"
#include "map.h"

void DesignRoomUpdateArmyPanel(int a1, int a2, int a3)
{
    u8 buf[8];

    sub_0808B6E8(buf, gUnknown_0808D754, 8);

    switch (gActiveMap->armyPanelState[a1])
    {
    case 0:
        gActiveMap->armyPanelState[a1] = gActiveMap->panelSide != 0 ? 0xA : 0x32;
        gActiveMap->armyPanelX[a1] = 0x1180;
        gActiveMap->armyPanelTimer[a1] =
            buf[gActiveMap->panelSide == 0 ? a1 + 4 : a1];
        break;
    case 0xA:
        gActiveMap->armyPanelTimer[a1]--;
        if (gActiveMap->armyPanelTimer[a1] <= 0)
        {
            gActiveMap->armyPanelX[a1] +=
                (0xBE0 - gActiveMap->armyPanelX[a1]) >> 3;
            if (gActiveMap->armyPanelX[a1] <= 0xCC0)
            {
                gActiveMap->armyPanelX[a1] = 0xCC0;
                gActiveMap->armyPanelState[a1] = 0x14;
            }
        }
        break;
    case 0x14:
        if (gActiveMap->panelSide == 0 || gActiveMap->mode != 1)
            gActiveMap->armyPanelState[a1] = 0x1E;
        break;
    case 0x1E:
        gActiveMap->armyPanelX[a1] +=
            (0x1180 - gActiveMap->armyPanelX[a1]) >> 3;
        if (gActiveMap->armyPanelX[a1] > 0x10DF)
        {
            gActiveMap->armyPanelX[a1] = 0x10E0;
            if (gActiveMap->mode == 1)
                gActiveMap->armyPanelState[a1] =
                    gActiveMap->panelSide == 0 ? 0x32 : 0;
        }
        break;
    }

    switch (gActiveMap->armyPanelState[a1])
    {
    case 0x32:
        gActiveMap->armyPanelState[a1] = gActiveMap->panelSide == 0 ? 0x3C : 0;
        gActiveMap->armyPanelX[a1] = 0xFD80;
        gActiveMap->armyPanelTimer[a1] =
            buf[gActiveMap->panelSide == 0 ? a1 + 4 : a1];
        break;
    case 0x3C:
        gActiveMap->armyPanelTimer[a1]--;
        if (gActiveMap->armyPanelTimer[a1] <= 0)
        {
            gActiveMap->armyPanelX[a1] +=
                (0x140 - gActiveMap->armyPanelX[a1]) >> 3;
            if (gActiveMap->armyPanelX[a1] > 0x1F)
            {
                gActiveMap->armyPanelX[a1] = 0x20;
                gActiveMap->armyPanelState[a1] = 0x46;
            }
        }
        break;
    case 0x46:
        if (gActiveMap->panelSide != 0 || gActiveMap->mode != 1)
            gActiveMap->armyPanelState[a1] = 0x50;
        break;
    case 0x50:
        gActiveMap->armyPanelX[a1] +=
            (-928 - gActiveMap->armyPanelX[a1]) >> 3;
        if (gActiveMap->armyPanelX[a1] <= -640)
        {
            gActiveMap->armyPanelX[a1] = -640;
            if (gActiveMap->mode == 1)
                gActiveMap->armyPanelState[a1] =
                    gActiveMap->panelSide == 0 ? 0x32 : 0;
        }
        break;
    }

    sub_080032EC(a1, gActiveMap->armyPanelX[a1] >> 4, 0x6A);
}
asm(".global sub_080030BC\n.thumb_set sub_080030BC, DesignRoomUpdateArmyPanel\n");
