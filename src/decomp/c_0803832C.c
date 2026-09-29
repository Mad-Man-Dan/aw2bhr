#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803832C.
 * sub_0803832C @ 0x0803832C
 */

void EndOfGame_Finish(void)
{
    SetMapPlayed(gPlaySt.mapID, 1);

    switch (gPlaySt.gameMode)
    {
    case 1:
        EndOfGame_FinishCampaignMap();
        break;
    case 2:
        sub_08038548();
        break;
    case 3:
        EndOfGame_FinishVersusMap();
        break;
    }
}
asm(".global sub_0803832C\n.thumb_set sub_0803832C, EndOfGame_Finish\n");
