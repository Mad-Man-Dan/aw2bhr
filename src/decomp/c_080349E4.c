#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080349E4.
 * sub_080349E4 @ 0x080349E4
 */

void sub_080349E4(void)
{
    u8 v;

    if (ShouldPromptCountryName())
        sub_0802BFA8();

    SetMapLayersDefault();

    v = gPlayers[GetNextActiveArmy(gUnknown_030033EC)].teamColor;

    sub_0801F150(1, (void *)0x06010000, 0x1ca, 0x13);
    LoadTilePoolGraphic(v + 0x3d);

    gUnknown_030032D8 = 3;
}
