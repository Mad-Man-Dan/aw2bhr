#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A8F4.
 * sub_0807A8F4 @ 0x0807A8F4
 */

/* A guard on a u8-returning predicate -- the `lsls #0x18` before the test is
 * the re-narrowing agbcc emits for a sub-word return type, not a mask this
 * function applies. MatchSummary_PlayMusic next door is the same shape with a different
 * body, and was diffed against this one rather than derived from it. */
void ResultsScreen_FinishClose(void)
{
    if (IsCampaignMilestoneMission() == 0)
        SetSoundMixerChannelCount8();
}
asm(".global sub_0807A8F4\n.thumb_set sub_0807A8F4, ResultsScreen_FinishClose\n");
