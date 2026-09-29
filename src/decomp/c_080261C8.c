#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080261C8.
 * sub_080261C8 @ 0x080261C8
 */

u8 *GetArmyTeamColorPalette(int index)
{
    u8 *base = gUnknown_0810E6E0;

    return base + (gPlayers[index].teamColor - 1) * 0x20;
}
asm(".global sub_080261C8\n.thumb_set sub_080261C8, GetArmyTeamColorPalette\n");
