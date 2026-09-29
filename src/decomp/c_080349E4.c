#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080349E4.
 * sub_080349E4 @ 0x080349E4
 */

void MapState_PrepareTurnHandover(void)
{
    u8 v;

    if (ShouldPromptCountryName())
        StartScreenCoverWipeLocked();

    SetMapLayersDefault();

    v = gPlayers[GetNextActiveArmy(gUnknown_030033EC)].teamColor;

    InitTilePool(1, (void *)0x06010000, 0x1ca, 0x13);
    LoadTilePoolGraphic(v + 0x3d);

    gUnknown_030032D8 = 3;
}
asm(".global sub_080349E4\n.thumb_set sub_080349E4, MapState_PrepareTurnHandover\n");
