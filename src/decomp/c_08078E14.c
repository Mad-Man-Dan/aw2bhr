#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078E14.
 * GetCampaignMissionId @ 0x08078E14
 */

int GetCampaignMissionId(void)
{
    return gPlaySt.mapID - 0x89;
}

asm(".global sub_08078E14\n.thumb_set sub_08078E14, GetCampaignMissionId\n");
