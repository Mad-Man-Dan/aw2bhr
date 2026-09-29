#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045F5C.
 * sub_08045F5C @ 0x08045F5C
 */

/* Two separate `bl`s with the same second argument, so an if/else and not a
 * ternary over the first. IsCampaignCompletionFlagSet returns `int` and its result is tested
 * whole (`cmp r0,#0` with no narrowing shift). */
void MapEventFx_SetFlag21Or22ByHardMode(void)
{
    if (IsCampaignCompletionFlagSet(0x60))
        SetCampaignCompletionFlag(0x22, 1);
    else
        SetCampaignCompletionFlag(0x21, 1);
}
asm(".global sub_08045F5C\n.thumb_set sub_08045F5C, MapEventFx_SetFlag21Or22ByHardMode\n");
