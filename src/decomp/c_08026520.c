#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026520.
 * sub_08026520 @ 0x08026520
 */

/* Refreshes the four per-army summary bytes each turn: unk34/unk35/unk36 are
 * three independent scores, unk38 is GetClampedRankScoreSum's clamped total of them and
 * unk33 is GetRankFromScore's grade of that total.
 *
 * The `strh` at +0x38 is immediately followed by an `ldrh` of the same slot:
 * the source stores the field and then passes the FIELD, not the register.
 * Written as `GetRankFromScore(GetClampedRankScoreSum(i))` the reload disappears. */
void UpdateAllArmyScores(void)
{
    u8 i;

    for (i = 1; i <= 4; i++)
    {
        gPlayers[i].speedScore = GetSpeedScore(i);
        gPlayers[i].powerScore = GetPowerScore(i);
        gPlayers[i].techScore = GetTechScore(i);
        gPlayers[i].totalScore = GetClampedRankScoreSum(i);
        gPlayers[i].rank = GetRankFromScore(gPlayers[i].totalScore);
    }
}
asm(".global sub_08026520\n.thumb_set sub_08026520, UpdateAllArmyScores\n");
