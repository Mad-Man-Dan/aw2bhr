#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038474.
 * sub_08038474 @ 0x08038474
 */

/* Nested and not sequential: GetCampaignScoreRank takes an argument and so READS r0,
 * which GetAverageCampaignScore has just written. `pop {r1}` leaves that result live. */
int GetAverageCampaignRank(void)
{
    return GetCampaignScoreRank(GetAverageCampaignScore());
}
asm(".global sub_08038474\n.thumb_set sub_08038474, GetAverageCampaignRank\n");
