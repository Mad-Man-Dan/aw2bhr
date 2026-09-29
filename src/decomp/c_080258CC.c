#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080258CC.
 * sub_080258CC @ 0x080258CC
 */

void RebuildMapUnitLayers(void)
{
    int i;
    int j;
    int id;
    u8 c;

    gMap->camX = gMap->scrollX / 16;
    gMap->camY = gMap->scrollY / 16;
    gMap->unk08 = gMap->scrollX;
    gMap->unk0a = gMap->scrollY;
    UpdateMapBgScroll();
    RebuildVisionPlanes();

    for (i = 0; i < gMap->height; i++)
    {
        for (j = 0; j < gMap->width; j++)
        {
            gMap->unit[gMap->rowOffset[i] + j] = 0;
            gMap->unitUnk[gMap->rowOffset[i] + j] = 0;
        }
    }

    for (id = 1; id <= 0xFF; id++)
    {
        if (gUnits[id].type == 0)
            continue;
        if ((gUnits[id].flags & 6) == 2)
            continue;
        c = gMap->unitUnk[gMap->rowOffset[gUnits[id].y]
                          + gUnits[id].x];
        if (c != 0)
        {
            if ((gUnits[c].flags & 4) == 0)
                continue;
        }
        gMap->unitUnk[gMap->rowOffset[gUnits[id].y]
                      + gUnits[id].x] = id;
        if (gUnits[id].flags & 2)
            continue;
        if (gPlayers[gUnknown_030033EC].aiControlled != 2)
        {
            if (gMap->visible[gMap->rowOffset[gUnits[id].y]
                              + gUnits[id].x] == 0)
                continue;
        }
        gMap->unit[gMap->rowOffset[gUnits[id].y]
                   + gUnits[id].x] = id;
    }

    for (id = 1; id <= 0xFF; id++)
    {
        if (gUnits[id].type == 0)
            continue;
        if ((u8)(gUnits[id].flags & 2) != 0)
            continue;
        if (gPlayers[gUnknown_030033EC].aiControlled == 2)
            continue;
        if (IsUnitIdVisibleToViewer(id))
        {
            if (gMap->visible[gMap->rowOffset[gUnits[id].y]
                              + gUnits[id].x] != 0)
                continue;
        }
        gMap->unit[gMap->rowOffset[gUnits[id].y]
                   + gUnits[id].x] = 0;
    }

    RenderMap();
    RedrawUnitLayer();
    RedrawUnitIconLayer();
}
asm(".global sub_080258CC\n.thumb_set sub_080258CC, RebuildMapUnitLayers\n");
