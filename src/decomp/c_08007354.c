#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08007354.
 * sub_08007354 @ 0x08007354
 */

void DesignRoomSaveSelection(void)
{
    struct ActiveMap *p = gActiveMap;
    int i;

    if (p->editMode == 0)
    {
        if (p->flags & 1)
            return;

        p->flags |= 1;

        i = p->ringIndex + 4;
        if (i > 9)
            i = p->ringIndex - 6;

        p->savedTerrain = gDesignRing[i].itemId;
        p->savedTerrainListIndex = p->terrainListIndex;
        gActiveMap->savedPropertyArmy = gActiveMap->propertyArmy;
    }
    else
    {
        if (p->flags & 2)
            return;

        p->flags |= 2;

        i = p->ringIndex + 3;
        if (i > 7)
            i = p->ringIndex - 5;

        p->savedUnit = gDesignRing[i].itemId;
        p->savedUnitListIndex = p->unitListIndex;
        gActiveMap->savedUnitArmy = gActiveMap->unitArmy;
    }
}
asm(".global sub_08007354\n.thumb_set sub_08007354, DesignRoomSaveSelection\n");
