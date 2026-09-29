#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08059050.
 * sub_08059050 @ 0x08059050
 */

void AiConsiderEnemyHq(int a1, s16 *best, void *out)
{
    u8 army;
    s8 v;

    army = a1;

    if (IsPlayerAliveAndActive(army) && DoesArmyHaveHq(army))
    {
        if ((s8)gUnknown_03003340[gPlayers[army].hqY & 0x7f]
                                 [gPlayers[army].hqX & 0x7f] >= 0)
        {
            v = gUnknown_03003340[gPlayers[army].hqY & 0x7f]
                                 [gPlayers[army].hqX & 0x7f];

            if (v < *best)
            {
                struct PlayerStruct *base;

                *best = v;
                base = gPlayers;
                ((u16 *)out)[0] = base[army].hqX & 0x7f;
                ((u16 *)out)[1] = base[army].hqY & 0x7f;
            }
        }
    }
}
asm(".global sub_08059050\n.thumb_set sub_08059050, AiConsiderEnemyHq\n");
