#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034D18.
 * sub_08034D18 @ 0x08034D18
 */

void MapState_DispatchTurnByController(void)
{
    SetInfoBoxMode(0);

    switch (gPlayers[gUnknown_030033EC].aiControlled)
    {
    case 1:
        gUnknown_030032D8 = 0xd;
        break;

    case 2:
        if (gPlaySt.savingEnabled == 0 || !SioGetSelfId())
        {
            gUnknown_03004780 = 0;
            gUnknown_030032D8 = 0xe;
            break;
        }

        gUnknown_030032D8 = 0x13;
        break;

    case 3:
        gUnknown_030032D8 = 0x13;
        break;

    case 4:
        gUnknown_030032D8 = 0x13;
        break;

    case 5:
        gUnknown_030032D8 = 0x13;
        break;

    case 6:
        gUnknown_030032D8 = 0x13;
        break;
    }
}
asm(".global sub_08034D18\n.thumb_set sub_08034D18, MapState_DispatchTurnByController\n");
