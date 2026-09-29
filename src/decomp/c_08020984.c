#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08020984.
 * sub_08020984 @ 0x08020984, sub_08020B88 @ 0x08020B88
 */

/* Recomputes every army's gPlayers unk1c "stance" byte for the current
 * army gUnknown_030033EC: clear all five, seed the current one from the
 * gUnknown_08090944 table (upgraded to 3 when HasHumanTeammate agrees), propagate it
 * to every army GetArmyTeamRelation rates 2, then -- unless HasTwoOpposingHumanTeamsOnCpuTurn vetoes -- paint
 * the complement (v ^ 3) onto the rated-1 live armies and their own rated-2
 * neighbours (`v ^ 3` written at both uses -- the ROM's copy of it sits AFTER
 * the loop's `i = 1` init in the preheader, which is where LICM puts a hoist and
 * not where a source statement would land). Finally flag the current army with bit 1 if no live army is
 * already occupied or flagged.
 *
 * The `cur + k <= 4 ? cur + k : cur + k - 4` wrap is bound to a local in the
 * outer loops (the ROM computes it once into r5) and written out TWICE in the
 * innermost one, which is c_08020864.c's documented case. */
void RecomputeArmyVisionMasks(void)
{
    u16 i;
    u16 j;
    u16 t;
    u16 v;

    gPlayers[0].turnState = 0;
    for (i = 1; i <= 4; i++)
        gPlayers[i].turnState = 0;

    v = gUnknown_08090944[gPlayers[gUnknown_030033EC].aiControlled];
    if (v == 1 && (u8)HasHumanTeammate(gUnknown_030033EC))
        v = 3;
    gPlayers[gUnknown_030033EC].turnState = v;

    for (i = 1; i <= 3; i++)
    {
        t = gUnknown_030033EC + i <= 4
          ? gUnknown_030033EC + i
          : gUnknown_030033EC + i - 4;
        if ((u16)GetArmyTeamRelation(gUnknown_030033EC, t) == 2)
            gPlayers[t].turnState = v;
    }

    if (!(u8)HasTwoOpposingHumanTeamsOnCpuTurn(gUnknown_030033EC))
    {
        for (i = 1; i <= 3; i++)
        {
            t = gUnknown_030033EC + i <= 4
              ? gUnknown_030033EC + i
              : gUnknown_030033EC + i - 4;
            if ((u16)GetArmyTeamRelation(gUnknown_030033EC, t) == 1
             && gPlayers[t].aiControlled == 1)
            {
                gPlayers[t].turnState = v ^ 3;
                for (j = 1; j <= 3; j++)
                {
                    if ((u16)GetArmyTeamRelation(t, t + j <= 4 ? t + j : t + j - 4) == 2)
                        gPlayers[t + j <= 4 ? t + j : t + j - 4].turnState = v ^ 3;
                }
            }
        }
    }

    for (i = 1; i <= 5; i++)
    {
        if (i > 4)
        {
            gPlayers[gUnknown_030033EC].turnState |= 2;
            break;
        }
        if (IsPlayerAliveAndActive(i) && gPlayers[i].aiControlled == 1)
            break;
        if (IsPlayerAliveAndActive(i) && (gPlayers[i].turnState & 2))
            break;
    }

    gUnknown_03004070 = 0;
    gUnknown_03004088 = 1;
}
asm(".global sub_08020984\n.thumb_set sub_08020984, RecomputeArmyVisionMasks\n");

/* Overlay writer over the Manhattan disk of radius `r` around (x, y): the outer
 * loop walks yy from y-r to y+r with a parallel s16 `dy` running -r..+r, and the
 * inner loop walks xx over x-r+|dy| .. x+r-|dy|.
 *
 * `dy` is a real source variable, not a strength-reduced giv, and its `-r` init
 * is the function's FIRST statement: the ROM negates the raw `r << 16` in place
 * (`negs r0, r5; lsrs r6, r0, #0x10`, two instructions off the same shift the
 * sign extension of `r` needs), which only happens while `r` has no
 * sign-extended form yet. Written after `yy = y - r` it costs a third
 * instruction and negates the sign-extended copy instead.
 *
 * The map reads go straight through gMap->rowOffset[] and gMap->unit[]. */
void MarkAttackableCellsInRange(s16 x, s16 y, s16 r, s16 v)
{
    s16 xx;
    s16 yy;
    s16 dy;
    struct Unk02028360 *u;
    int m;
    int n;
    int t;

    dy = -r;

    for (yy = y - r; yy <= y + r; yy++, dy++)
    {
        if (yy < 0)
            continue;
        if (yy >= gMap->height)
            continue;

        for (xx = x - r + (dy < 0 ? -dy : dy);
             xx <= x + r - (dy < 0 ? -dy : dy);
             xx++)
        {
            if (xx < 0)
                continue;
            if (xx >= gMap->width)
                continue;

            u = FindLivingInventionTargetAt(xx, yy);
            m = gUnknown_020288B4[gMap->rowOffset[yy] + xx];
            n = 0;
            if (u != NULL)
                n = u->unk02_6;
            if (m)
                n = 6;

            if (n)
            {
                CalcAttackOutcome(gUnknown_03003F38, 0, &gUnknown_03003100.pos);
                if (gBattleAttacker->attackType != 0)
                    gUnknown_03003340[yy][xx] = v;
            }
            else
            {
                t = gMap->unit[gMap->rowOffset[yy] + xx];
                if (t == 0)
                    continue;
                if (AreUnitsOnSameTeam(gUnknown_03003F38, t) == 1)
                    continue;
                CalcAttackOutcome(gUnknown_03003F38, t, &gUnknown_03003100.pos);
                if (gBattleAttacker->attackType != 0)
                    gUnknown_03003340[yy][xx] = v;
            }
        }
    }
}
asm(".global sub_08020B88\n.thumb_set sub_08020B88, MarkAttackableCellsInRange\n");
