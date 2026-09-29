#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078800.
 * SetupCoSelectHuntsEnd @ 0x08078800
 */

#include "proc.h"

/* SetupCoSelectFactoryBlues's twin, 0x48 bytes along: the same shape with the roles of
 * AddCoSelectGroupBlueMoon and AddCoSelectGroupYellowComet swapped and the tag 0x6a instead of 0x6b. */

void SetupCoSelectHuntsEnd(void)
{
    s32 i;

    ClearArmyCount();
    i = AddCoSelectGroupYellowComet(0);
    i = AddCoSelectGroupOrangeStar(i);

    if (IsCampaignCompletionFlagSet(0x6a))
    {
        AddCoSelectGroupBlueMoon(i);
        SetCoSelectGroupSwitchAllButFirst();
    }
    else
    {
        SetCoSelectGroupSwitchNone();
    }
}

asm(".global sub_08078800\n.thumb_set sub_08078800, SetupCoSelectHuntsEnd\n");
