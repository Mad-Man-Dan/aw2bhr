#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067070.
 * sub_08067070 @ 0x08067070
 */

/* The gUnknown_08580DD8 slot script's third wrapper, alongside the
 * StartMatchSetupScreen/sub_080670BC installer pair. gUnknown_0202F200 is the same 0/1
 * mode flag those two set, and here it GUARDS the removal: the mode-0 slot is
 * torn down (and MatchSetupUnpackRuleIndices run first), the mode-1 slot is left alone.
 *
 * `pop {r0}` fixes this as void even though sub_0801537C returns an int, so
 * both calls are bare statements. */

void MatchSetupScreen_Finish(void)
{
    sub_080733B8();
    sub_0801537C(gUnknown_08580CC4);

    if (gUnknown_0202F200 == 0)
    {
        MatchSetupUnpackRuleIndices();
        sub_0801537C(gUnknown_08580DD8);
    }
}
asm(".global sub_08067070\n.thumb_set sub_08067070, MatchSetupScreen_Finish\n");
