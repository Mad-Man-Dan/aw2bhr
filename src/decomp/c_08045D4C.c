#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045D4C.
 * sub_08045D4C @ 0x08045D4C
 */

int MapEventCond_SelectedUnitBesideTeamColor1Vs3(void)
{
    int x;
    int y;
    int side;
    int c;
    int oleft;
    int oright;

    x = gUnknown_030040D8->unk02;
    y = gUnknown_030040D8->unk03;

    switch (gPlayers[(((struct Unit *)gUnknown_030040D8
                                - gUnits) >> 6) + 1].teamColor)
    {
    case 3:
        side = 1;
        break;
    case 1:
        side = 3;
        break;
    default:
        return 0;
    }

    if (y > 0)
    {
        c = gMap->unitUnk[
                gMap->rowOffset[y - 1] + x];
        if (c != 0 && gPlayers[(c >> 6) + 1].teamColor == side)
            return 1;
    }

    if (y < gMap->height - 1)
    {
        c = gMap->unitUnk[
                gMap->rowOffset[y + 1] + x];
        if (c != 0 && gPlayers[(c >> 6) + 1].teamColor == side)
            return 1;
    }

    if (x > 0)
    {
        oleft = gMap->rowOffset[y] - 1;
        c = gMap->unitUnk[oleft + x];
        if (c != 0 && gPlayers[(c >> 6) + 1].teamColor == side)
            return 1;
    }

    if (x < gMap->width - 1)
    {
        oright = gMap->rowOffset[y] + 1;
        c = gMap->unitUnk[oright + x];
        if (c != 0 && gPlayers[(c >> 6) + 1].teamColor == side)
            return 1;
    }

    return 0;
}
asm(".global sub_08045D4C\n.thumb_set sub_08045D4C, MapEventCond_SelectedUnitBesideTeamColor1Vs3\n");
