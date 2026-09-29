#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080787B8.
 * SetupCoSelectFactoryBlues @ 0x080787B8
 */

#include "proc.h"

/* A conditional variant of the family-F035 display-list builders that
 * src/decomp/c_08078790.c documents. Two of the four builders are CHAINED --
 * `bl AddCoSelectGroupBlueMoon` then `bl AddCoSelectGroupOrangeStar` with no argument setup between them
 * is a nest, exactly as in src/decomp/c_08078864.c -- and the third is applied
 * only when IsCampaignCompletionFlagSet(0x6b) holds.
 *
 * The two `adds r4, r0, #0` copies, the first of which is dead, are the tell
 * for ONE binding local assigned twice rather than two locals or a nested
 * expression: the pseudo lives in r4 across the IsCampaignCompletionFlagSet call, so each
 * assignment emits its copy even though only the second is read. */

void SetupCoSelectFactoryBlues(void)
{
    s32 i;

    ClearArmyCount();
    i = AddCoSelectGroupBlueMoon(0);
    i = AddCoSelectGroupOrangeStar(i);

    if (IsCampaignCompletionFlagSet(0x6b))
    {
        AddCoSelectGroupYellowComet(i);
        SetCoSelectGroupSwitchAllButFirst();
    }
    else
    {
        SetCoSelectGroupSwitchNone();
    }
}

asm(".global sub_080787B8\n.thumb_set sub_080787B8, SetupCoSelectFactoryBlues\n");
