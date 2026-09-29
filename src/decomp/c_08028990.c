#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028990.
 * sub_08028990 @ 0x08028990, sub_080289BC @ 0x080289BC
 */

/* The GUARD is the early return, not the call: the ROM falls through to
 * `bl DoesArmyHaveUnits` and parks `movs r0, #1` past the literal pool. With a
 * single condition, `if (flag) return f(a); return 1;` comes out the other way
 * round -- gcc makes the trailing `return 1` the fall-through. Its sibling
 * CheckArmySurvivesHumanNoUnitsRule needs no inversion because two `&&`-ed conditions already leave
 * the call as the fall-through. */
u8 CheckArmySurvivesNoUnitsRule(u16 a1)
{
    if ((gPlaySt.event20 & 1) == 0)
        return 1;

    return DoesArmyHaveUnits(a1);
}
asm(".global sub_08028990\n.thumb_set sub_08028990, CheckArmySurvivesNoUnitsRule\n");

/* The parameter is `int`, NOT u16, and this function is the only place that
 * shows: it hands the raw value to DoesArmyHaveUnits with no narrowing at all, where
 * a u16 parameter there would have emitted `lsls #0x10; lsrs #0x10` in front of
 * the `bl`. Its sibling CheckArmySurvivesNoUnitsRule passes an already-zero-extended u16 and so
 * cannot distinguish the two. */
u8 CheckArmySurvivesHumanNoUnitsRule(int a1)
{
    if (gPlayers[a1].aiControlled == 1 && (gPlaySt.event20 & 0x10))
        return DoesArmyHaveUnits(a1);

    return 1;
}
asm(".global sub_080289BC\n.thumb_set sub_080289BC, CheckArmySurvivesHumanNoUnitsRule\n");
