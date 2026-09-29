#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080459E4.
 * sub_080459E4 @ 0x080459E4, sub_08045A78 @ 0x08045A78
 */

/* Counts the current army's units that are either at zero HP or whose displayed
 * HP `Div(hp - 1, 10) + 1` is 9 or less, and reports whether there is at least
 * one.  Same scan as MapEventCond_CurrentArmyHasNoUnitWithFuel over `(u16)gUnknown_084995FE[army] + 1 ..
 * + 0x32`.
 *
 * gUnknown_0812A0F4 is NOT a global: the ROM word at 0x0812A0F4 contains
 * 0x03003F2C, so it is agbcc's own -fforce-addr .rodata copy of
 * &gUnknown_03003F2C, created because the army index is read on both sides of
 * the loop's merge (once in the preheader, once in the recomputed bound).  The
 * honest spelling names gUnknown_03003F2C and lets the build place the word --
 * MapEventCond_CurrentArmyHasNoUnitWithFuel, which reads it once, gets a plain `ldr =sym` instead.
 *
 * The element MUST be bound to a local.  Spelling `gUnits[i]` inline
 * three times rematerialises &gUnits from the inline pool inside the
 * loop and emits `adds r1, r5, r0` (index + pointer); the binding gives the
 * address its own callee-saved register (r8) and the ROM's `adds r1, r0, r5`.
 * That is the "pointer-sum vs ADDR_EXPR" anchor rule, and it also buys the
 * second high register the ROM's prologue saves.
 *
 * Nothing in the body is hoisted because `Div` clobbers memory, which is why the
 * bound is recomputed every iteration while the two address constants sit in
 * callee-saved registers. */
bool8 MapEventCond_CurrentArmyHasInjuredUnit(void)
{
    struct Unit *unit;
    int i;
    int count;

    count = 0;

    for (i = (u16)gUnknown_084995FE[gUnknown_03003F2C] + 1;
         i < (u16)gUnknown_084995FE[gUnknown_03003F2C] + 0x33; i++)
    {
        unit = &gUnits[i];

        if (unit->type == 0)
            continue;

        if (unit->hp == 0 || Div(unit->hp - 1, 10) + 1 <= 9)
            count++;
    }

    if (count > 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_080459E4\n.thumb_set sub_080459E4, MapEventCond_CurrentArmyHasInjuredUnit\n");

/* The two-counter sibling of MapEventCond_CurrentArmyHasInjuredUnit: over the current army's 0x32 slots it
 * counts the units of type 0x13 (n) and, among those, the ones at zero HP or
 * with a displayed HP `Div(hp - 1, 10) + 1` of 9 or less (m), returning TRUE
 * only when there are more than one of the former and at least one of the
 * latter.
 *
 * gUnknown_0812A0F8 is NOT a global: the ROM word at 0x0812A0F8 contains
 * 0x03003F2C, so it is agbcc's own -fforce-addr .rodata copy of
 * &gUnknown_03003F2C -- the same word MapEventCond_CurrentArmyHasInjuredUnit gets at 0x0812A0F4.  Two
 * private copies of one address, exactly as the literal-pool chapter predicts.
 * The honest spelling names gUnknown_03003F2C and lets the build place it.
 *
 * The element must be bound to a local for the same reason as MapEventCond_CurrentArmyHasInjuredUnit:
 * inline, &gUnits is rematerialised from the inline pool inside the
 * loop and the address adds come out `index + pointer`; bound, the constant wins
 * the third callee-saved register (sb) and the adds come out `pointer + index`.
 *
 * The unk00 tests are two separate `if`s -- `!= 0 && == 0x13` would be folded to
 * one compare.  `Div` clobbers memory, so nothing in the body is hoisted and the
 * loop bound is recomputed every iteration. */
bool8 MapEventCond_TwoPlusBattleCoptersAnyInjured(void)
{
    struct Unit *unit;
    int i;
    int n;
    int m;

    n = 0;
    m = 0;

    for (i = (u16)gUnknown_084995FE[gUnknown_03003F2C] + 1;
         i < (u16)gUnknown_084995FE[gUnknown_03003F2C] + 0x33; i++)
    {
        unit = &gUnits[i];

        if (unit->type == 0)
            continue;

        if (unit->type != 0x13)
            continue;

        n++;

        if (unit->hp == 0 || Div(unit->hp - 1, 10) + 1 <= 9)
            m++;
    }

    if (n > 1 && m != 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_08045A78\n.thumb_set sub_08045A78, MapEventCond_TwoPlusBattleCoptersAnyInjured\n");
