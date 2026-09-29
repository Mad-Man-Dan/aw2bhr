#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CC90.
 * UnitMenu_DiveUsability @ 0x0802CC90, UnitMenu_RiseUsability @ 0x0802CCCC
 */

/* Four guards, all of which must pass before this reports FALSE.
 *
 * Written as an `if (...) return TRUE;` chain and NOT as one negated
 * disjunction: here the merged THEN block (`movs r0, #1`) sits AFTER the pool
 * and the fallthrough `movs r0, #0` before it, which is the chain's layout.
 * Its neighbour UnitMenu_RiseUsability is the same predicate with the tests reordered and
 * comes out the other way round, so the two spellings are distinguishable and
 * this block contains one of each.
 *
 * gUnknown_030040D8->unk00 is the byte at offset 0, newly named: CanDropFirstCargoAt
 * and CanDropSecondCargoAt read the same byte off the same pointer to index
 * gUnknown_085D5ABC by 0x5c, so it is a record selector rather than a flag.
 *
 * Named per Xenesis's AW2 Subroutine List: "Menu Item Visibility Check -
 * Checks if a unit is a Sub (0x18)". TRUE means the menu item stays visible;
 * it's hidden only when the unit's class IS 0x18 and the other three guards
 * all fail too. The old UnitMenu_DiveUsability symbol is kept as a linker alias below
 * so every other unit keeps resolving it unchanged.
 */

bool8 UnitMenu_DiveUsability(void)
{
    if (gUnknown_030040D8->unk00 != 0x18)
        return TRUE;

    if (gUnknown_030040D8->unk01 & 0x20)
        return TRUE;

    if (!UnitMenu_JoinUsability())
        return TRUE;

    if (!UnitMenu_LoadUsability())
        return TRUE;

    return FALSE;
}

asm(".global sub_0802CC90\n.thumb_set sub_0802CC90, UnitMenu_DiveUsability\n");

/* UnitMenu_DiveUsability's three-test twin, and the spelling is the OTHER one.
 *
 * `if (A && B && C) return FALSE; return TRUE;` is semantically identical and
 * does NOT match: agbcc lays the THEN arm out inline and sends the
 * short-circuit exits to the end, putting `movs #0` before the pool. The ROM
 * has `movs #1` before the pool and `movs #0` after, i.e. the short-circuit
 * exits land on the fallthrough -- which is the De Morgan form below. The last
 * operand is the one that inverts, exactly as in sub_0802C550's `||`.
 */

bool8 UnitMenu_RiseUsability(void)
{
    if (!UnitMenu_JoinUsability() || !UnitMenu_LoadUsability() || !(gUnknown_030040D8->unk01 & 0x20))
        return TRUE;

    return FALSE;
}
asm(".global sub_0802CCCC\n.thumb_set sub_0802CCCC, UnitMenu_RiseUsability\n");
