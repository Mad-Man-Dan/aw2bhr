#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078790.
 * SetupCoSelectLiberation @ 0x08078790, SetupCoSelectNeotanksBm @ 0x080787A4
 */

/* Family F035 (data/families.json): `push {lr}; bl A; movs r0,#0; bl B; bl C;
 * pop {r0}; bx r0` -- 20 bytes, five members, and `varies` lists only the three
 * `bl` targets, so the `movs r0, #0` is byte-identical in every member.
 *
 * THREE STATEMENTS, not a nest. Sequential `bl`s carry no nesting signal at all
 * (docs/agbcc-codegen.md: r0 already holds the result, so the nest is the
 * spelling that needs no instruction), and the evidence is entirely callee-side:
 *   - callee A writes r0 before reading it, so it is nullary;
 *   - callee B does take an argument, but it is given an explicit `movs r0,#0`,
 *     which is what makes A's result dead;
 *   - callee C writes r0 before reading it, so a nest is not expressible.
 * `pop {r0}` is the void epilogue, so nothing is returned either.
 */

/* Family F035 (data/families.json): `push {lr}; bl A; movs r0,#0; bl B; bl C;
 * pop {r0}; bx r0` -- 20 bytes, five members, and `varies` lists only the three
 * `bl` targets, so the `movs r0, #0` is byte-identical in every member.
 *
 * THREE STATEMENTS, not a nest. Sequential `bl`s carry no nesting signal at all
 * (docs/agbcc-codegen.md: r0 already holds the result, so the nest is the
 * spelling that needs no instruction), and the evidence is entirely callee-side:
 *   - callee A writes r0 before reading it, so it is nullary;
 *   - callee B does take an argument, but it is given an explicit `movs r0,#0`,
 *     which is what makes A's result dead;
 *   - callee C writes r0 before reading it, so a nest is not expressible.
 * `pop {r0}` is the void epilogue, so nothing is returned either.
 */


/* The gUnknown_030058E0 display-list variant: ClearArmyCount resets the cursor
 * and SetCoSelectGroupSwitchNone clears the five words of gUnknown_030059C0, and both are
 * nullary. AddCoSelectGroupOrangeStar returns the advanced byte index (`pop {r1}; bx r1`) and
 * that result is DISCARDED here -- unlike SetupCoSelectHotPursuit, which chains the four
 * builders and does consume it.
 */

void SetupCoSelectLiberation(void)
{
    ClearArmyCount();
    AddCoSelectGroupOrangeStar(0);
    SetCoSelectGroupSwitchNone();
}

asm(".global sub_08078790\n.thumb_set sub_08078790, SetupCoSelectLiberation\n");

/* The gUnknown_030058E0 display-list variant: ClearArmyCount resets the cursor
 * and SetCoSelectGroupSwitchNone clears the five words of gUnknown_030059C0, and both are
 * nullary. AddCoSelectGroupBlueMoon returns the advanced byte index (`pop {r1}; bx r1`) and
 * that result is DISCARDED here -- unlike SetupCoSelectHotPursuit, which chains the four
 * builders and does consume it.
 */

void SetupCoSelectNeotanksBm(void)
{
    ClearArmyCount();
    AddCoSelectGroupBlueMoon(0);
    SetCoSelectGroupSwitchNone();
}

asm(".global sub_080787A4\n.thumb_set sub_080787A4, SetupCoSelectNeotanksBm\n");
