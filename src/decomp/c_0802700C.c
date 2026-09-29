#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802700C.
 * sub_0802700C @ 0x0802700C, sub_0802706C @ 0x0802706C
 */

bool8 IsCellOwnedByArmyTeam(int a1, int a2, int a3)
{
    int off;
    int army;

    off = gMap->rowOffset[a3] + a2;
    army = gMap->terrain[off];
    army = army >> 5;

    if (army == 0)
        return FALSE;

    if (gPlayers[a1].team == gPlayers[army].team)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802700C\n.thumb_set sub_0802700C, IsCellOwnedByArmyTeam\n");

bool8 ShouldDrawTransportMarker(u8 a1, u16 a2, u16 a3)
{
    if (gPlaySt.fog != 0
        && (gPlayers[a3].turnState & 2) == 0
        && gUnknown_085D5ABC[a1].transportTable != NULL)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802706C\n.thumb_set sub_0802706C, ShouldDrawTransportMarker\n");
