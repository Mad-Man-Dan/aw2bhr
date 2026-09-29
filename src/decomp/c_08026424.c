#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026424.
 * GetPowerScore @ 0x08026424
 */

/* A percentage, capped at 100: this army's unk18 measured against the summed
 * GetPlayerUnitsCreated of every OTHER active army on a different team. Zero when the
 * army is not live, and zero when nothing opposes it.
 *
 * `((31x) * 4 + x) * 8` is 1000x -- write the multiply and let agbcc build the
 * shift chain. The `a * 0x3c` element offset is hoisted into r8 by LICM (it
 * sits AFTER the loop's own inits in the preheader) and is recomputed after
 * the loop because that live range has ended; neither is authored.
 *
 * `sum` is UNSIGNED and the Div result is assigned back INTO it. Both are
 * load-bearing: a fresh local puts the clamp in r0 and costs the
 * `adds r5, r0, #0`, and a signed `sum` turns the ROM's `bls` into `ble`.
 *
 * Named per Xenesis's AW2 Subroutine List: "Power Scoring Subroutine". The
 * old GetPowerScore symbol is kept as a linker alias below so every other
 * unit keeps resolving it unchanged. */
u8 GetPowerScore(u8 a)
{
    u32 sum;
    u16 i;

    if (!IsPlayerAliveAndActive(a))
        return 0;

    sum = 0;
    for (i = 1; i <= 4; i++)
    {
        if (gPlayers[i].aiControlled != 0
         && gPlayers[a].team != gPlayers[i].team)
            sum += GetPlayerUnitsCreated(i);
    }

    if (sum == 0)
        return 0;

    sum = Div(gPlayers[a].totalDestroyed * 1000, sum);
    if (sum > 100)
        sum = 100;
    return sum;
}

asm(".global sub_08026424\n.thumb_set sub_08026424, GetPowerScore\n");
