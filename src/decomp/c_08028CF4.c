#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028CF4.
 * sub_08028CF4 @ 0x08028CF4
 */

/* GetCaptureLimitWinner returns `int`: the result is tested with a BARE `cmp r0, #0` and
 * only then cast to u8 (`lsls #0x18; lsrs #0x18`) for DefeatOtherTeamsAndEndMatch's u8 first
 * parameter. A narrow return would have been re-narrowed before the compare
 * instead. IsOnlyOneTeamLeft does return a byte -- `lsls r0, #0x18; cmp r0, #0`. */
void RunWinLossCheck(void)
{
    int r;

    if (IsOnlyOneTeamLeft())
    {
        MarkDefeatedArmies();
        FinalizeMatchResult();
    }
    else
    {
        r = GetCaptureLimitWinner();

        if (r != 0)
            DefeatOtherTeamsAndEndMatch(r, 0x20);
        else
            DefeatArmiesFailingRules();
    }
}
asm(".global sub_08028CF4\n.thumb_set sub_08028CF4, RunWinLossCheck\n");
