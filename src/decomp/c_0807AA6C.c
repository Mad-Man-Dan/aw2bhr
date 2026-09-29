#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807AA6C.
 * sub_0807AA6C @ 0x0807AA6C
 */

/* ResultsScreen_FinishClose's neighbour: same guard on the same predicate, but it plays a
 * sound instead of calling SetSoundMixerChannelCount8. The two differ in the guarded call and
 * nothing else. */
void MatchSummary_PlayMusic(void)
{
    if (IsCampaignMilestoneMission() == 0)
        PlayMusic(0xCD);
}
asm(".global sub_0807AA6C\n.thumb_set sub_0807AA6C, MatchSummary_PlayMusic\n");
