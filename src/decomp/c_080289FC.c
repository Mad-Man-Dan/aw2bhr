#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080289FC.
 * sub_080289FC @ 0x080289FC, sub_08028A68 @ 0x08028A68, MarkDefeatedArmies @ 0x08028AEC, sub_08028B70 @ 0x08028B70
 */

/* "Is army a1 the last one standing among the armies sharing its +0x2a team
 * byte?" -- gated on gPlaySt.unk04 bit 2, and answering TRUE (the
 * fall-through) whenever the gate is closed.
 *
 * The `&&` chain is what puts the gPlayers pool word INSIDE the loop:
 * the deref is written in place, once per iteration, and nothing hoists it
 * because the `bl IsPlayerAliveAndActive` in the same body clobbers it. r7 and r6 are
 * strength_reduce's two givs for `a1 * 0x3c` and `i * 0x3c`; only r6 is
 * initialised in the preheader (`movs r6,#0x3c`, i.e. i = 1) and neither is
 * source.
 *
 * `i` is `int`: the increment is a bare `adds r4,#1` and the exit test a signed
 * `ble` with no narrowing anywhere, while the two IsPlayerAliveAndActive arguments carry
 * the `lsls #0x18; lsrs #0x18` that function's u8 parameter demands. */
bool8 sub_080289FC(int a1)
{
    int i;

    if (gPlaySt.event20 & 4)
    {
        if (!IsPlayerAliveAndActive(a1))
            return FALSE;

        for (i = 1; i <= 4; i++)
        {
            if (a1 != i
             && gPlayers[a1].team == gPlayers[i].team
             && !IsPlayerAliveAndActive(i))
                return FALSE;
        }
    }

    return TRUE;
}

/* Runs the four per-army end-of-turn predicates over slots 1..4 and reports the
 * first one that fails to DefeatArmy, with a reason code (8, 0x10, 1, 2).
 * MarkDefeatedArmies is the same function against RecordArmyDefeat -- the two instruction
 * streams differ in nothing but that callee, at all four call sites.
 *
 * `i` is s16 and that is what the whole body's shift pattern is: agbcc keeps the
 * induction variable as `i << 16` in r6 and re-derives every argument from it
 * per the callee's own parameter type -- `lsls/lsrs #0x18` for IsPlayerAliveAndActive's
 * u8, `lsrs #0x10` for the three u16 predicates, `asrs #0x10` for CheckArmySurvivesHumanNoUnitsRule's
 * and DefeatArmy's `int`. Nothing here is a mask and nothing is authored: the
 * `movs r1,#0x80; lsls #9` at the bottom is the loop optimiser adding 0x10000 to
 * the shifted biv, i.e. `i++`.
 *
 * r4 is a COUNTER, not a walking bitmask -- it holds 1,2,3,4,5 and the bottom
 * adds one, not doubles. Its reuse for the sign-extended copy inside the body is
 * register allocation reclaiming a dead range, not a second variable. */
void DefeatArmiesFailingRules(void)
{
    s16 i;

    for (i = 1; i <= 4; i++)
    {
        if (IsPlayerAliveAndActive(i))
        {
            if (!CheckArmySurvivesTeamRule(i))
                DefeatArmy(i, 8);
            else if (!CheckArmySurvivesHumanNoUnitsRule(i))
                DefeatArmy(i, 0x10);
            else if (!CheckArmySurvivesNoUnitsRule(i))
                DefeatArmy(i, 1);
            else if (!CheckArmySurvivesHqRule(i))
                DefeatArmy(i, 2);
        }
    }
}
asm(".global sub_08028A68\n.thumb_set sub_08028A68, DefeatArmiesFailingRules\n");

/* Byte-for-byte the same function as DefeatArmiesFailingRules with RecordArmyDefeat in place of
 * DefeatArmy at all four call sites -- 132 bytes and 56 instructions each,
 * differing in nothing else, not a register, an immediate or a branch. See the
 * comment on DefeatArmiesFailingRules for the readout of the s16 induction variable. */
void MarkDefeatedArmies(void)
{
    s16 i;

    for (i = 1; i <= 4; i++)
    {
        if (IsPlayerAliveAndActive(i))
        {
            if (!CheckArmySurvivesTeamRule(i))
                RecordArmyDefeat(i, 8);
            else if (!CheckArmySurvivesHumanNoUnitsRule(i))
                RecordArmyDefeat(i, 0x10);
            else if (!CheckArmySurvivesNoUnitsRule(i))
                RecordArmyDefeat(i, 1);
            else if (!CheckArmySurvivesHqRule(i))
                RecordArmyDefeat(i, 2);
        }
    }
}

asm(".global sub_08028AEC\n.thumb_set sub_08028AEC, MarkDefeatedArmies\n");

/* "Which army slot is first at or past gPlaySt.unk31?" -- a linear
 * scan of slots 1..4 returning the slot number, or 0 both when unk31 is zero
 * and when nothing reaches it.
 *
 * `i` is u8, not int: the increment is `adds #1; lsls #0x18; lsrs #0x18` and the
 * exit test is an UNSIGNED `bls`, and the returned value needs no widening
 * because the counter already lives zero-extended. The compare against unk31 is
 * `bhs`, unsigned on both sides, which is what types the +0x11 member u8.
 *
 * The `ldr r3, [gPlayers]` in the preheader is the loop optimiser's
 * LICM hoist of the pointer deref, not source -- the body is written with the
 * ordinary `gPlayers[i]` subscript. */
int GetCaptureLimitWinner(void)
{
    u8 i;

    if (gPlaySt.captureLimit == 0)
        return 0;

    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].captures >= gPlaySt.captureLimit)
            return i;
    }

    return 0;
}
asm(".global sub_08028B70\n.thumb_set sub_08028B70, GetCaptureLimitWinner\n");
