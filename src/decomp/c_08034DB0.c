#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034DB0.
 * sub_08034DB0 @ 0x08034DB0, sub_08034DCC @ 0x08034DCC
 */

void MapState_WaitForCampaignIntro(void)
{
    if (sub_0803B628() == 0)
        gUnknown_030032D8 = 5;
}
asm(".global sub_08034DB0\n.thumb_set sub_08034DB0, MapState_WaitForCampaignIntro\n");

/* gUnknown_030033EC is a u16 read here with a bare `ldrb` -- the truncation
 * folded into the load that PlayArmyCoMusic's u8 parameter forces. */
void MapState_PlayTurnMusic(void)
{
    if (gUnknown_03004080 != 1)
        PlayArmyCoMusic(gUnknown_030033EC);

    RunMapEventsAtTurnStart();
    gUnknown_030032D8 = 6;
}
asm(".global sub_08034DCC\n.thumb_set sub_08034DCC, MapState_PlayTurnMusic\n");
