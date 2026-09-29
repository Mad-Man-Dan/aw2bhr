#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037FD0.
 * EndOfGame_StartResultFlow @ 0x08037FD0
 */

/* End-of-battle dispatch: picks the winning slot for StartVictoryFlow or falls
 * back to StartDefeatFlow.
 *
 * The savingEnabled tail is written c-arm FIRST, each arm with its own fail
 * call. That is the order reload sees, and reload's spill-register choice is
 * ROUND-ROBIN in insn order (here over r0/r1), so the a, c, d tests take r1,
 * r0, r1. jump2 then cross-jumps the calls and its "if (foo) bar; else
 * break;" range swap moves the d-arm above the c-arm, which is the ROM's
 * layout. Written d-arm first, the tests take r1, r0, r1 in the other order
 * and the two arms trade r0/r1 (16 bytes).
 *
 * Load-bearing, measured in earlier waves: `int m` with an explicit `(u8)m`
 * at each use; `a = b = c = d = 0;` as one chained assignment (stores
 * descending); `do { } while (0);` before `flag = 1;`; and a bare `break` on
 * `flag == 1` with the one StartDefeatFlow call after the loop. */
#include "global.h"
#include "proc.h"

void EndOfGame_StartResultFlow(ProcPtr parent)
{
    int a;
    int b;
    int c;
    int d;
    int flag;
    int i;
    int j;
    int m;

    for (i = 1; i <= 4; i++)
    {
        if (sub_080266DC(i))
            AwardSpeedBonusPoints(i, gPlaySt.mapID);
    }
    UpdateAllArmyScores();
    InsertBestScoreRecord(gPlayers[GetResultsArmy()].co,
                 gPlaySt.mapID,
                 gPlayers[GetResultsArmy()].totalScore,
                 gUnknown_03004080);
    sub_08030574();
    switch (gPlaySt.gameMode)
    {
    case 0:
    case 1:
    case 2:
        if (sub_080266DC(1))
            StartVictoryFlow(1, parent);
        else
            StartDefeatFlow(parent);
        break;
    case 3:
        a = b = c = d = 0;
        flag = 0;
        for (i = 1; i <= 4; i++)
        {
            if (gPlayers[i].aiControlled != 0)
            {
                if (gPlayers[i].defeated == 0)
                {
                    for (j = 1; j <= 4; j++)
                    {
                        if (i != j
                         && gPlayers[j].aiControlled != 0
                         && gPlayers[j].defeated == 0
                         && gPlayers[i].team != gPlayers[j].team)
                        {
                            do { } while (0);
                            flag = 1;
                            break;
                        }
                    }
                }
                if (flag == 1)
                    break;
            }
        }
        if (flag == 1)
        {
            StartDefeatFlow(parent);
            return;
        }
        for (m = 1; m <= 4; m++)
        {
            switch (gPlayers[m].aiControlled)
            {
            case 0:
                break;
            case 1:
                a = (u8)m;
                if (sub_080266DC(m))
                    c = a;
                break;
            case 2:
                b = (u8)m;
                if (sub_080266DC(m))
                    d = b;
                break;
            default:
                if (sub_080266DC(m))
                    d = (u8)m;
                break;
            }
        }
        if (gPlaySt.savingEnabled != 0)
        {
            if (a != 0)
            {
                if (c != 0)
                {
                    StartVictoryFlow(c, parent);
                    return;
                }
                StartDefeatFlow(parent);
                return;
            }
            else
            {
                if (d != 0)
                {
                    StartVictoryFlow(d, parent);
                    return;
                }
                StartDefeatFlow(parent);
                return;
            }
        }
        {
            if (b == 0)
                StartVictoryFlow(c, parent);
            else if (a == 0)
                StartVictoryFlow(d, parent);
            else if (c != 0)
                StartVictoryFlow(c, parent);
            else
                StartDefeatFlow(parent);
        }
        break;
    }
}

asm(".global sub_08037FD0\n.thumb_set sub_08037FD0, EndOfGame_StartResultFlow\n");
