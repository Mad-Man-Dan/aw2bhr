#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028BAC.
 * sub_08028BAC @ 0x08028BAC
 */

/* MATCHED byte-for-byte, first attempt (re-verified after the header edits).
 *
 * "Is exactly one team still eligible, and is it alone in its +0x2a team byte?"
 * Three passes over slots 1..4 sharing one u8[8] stack scratch array.
 *
 * Pass 1 writes `buf[i] = 0` unconditionally and overwrites it with 1 on the
 * far side of the && chain -- the ROM's `movs r0,#0; strb r0,[r6]` at the TOP
 * of the body, before the first `bl`, is what forces that spelling rather than
 * `buf[i] = (a && b && ...)`. The five predicates take three DIFFERENT widths
 * and every narrowing comes from the declared prototype, not from a cast here:
 * `lsls/lsrs #0x18` for IsPlayerAliveAndActive's u8, ONE `lsls/lsrs #0x10` pair CSEd
 * across all three u16 predicates (r4), and a bare `adds r0,r5,#0` for
 * CheckArmySurvivesHumanNoUnitsRule's int. Flattening them to one width loses the r4 reuse.
 *
 * Pass 2's `buf[i] = 0` reuses the register the `!buf[j]` test just loaded
 * (`strb r0,[r7]` where r0 is the zero from `ldrb r0,[sp+j]`) -- ordinary CSE
 * off a plain constant store, nothing special in the source.
 *
 * i and j are all `int`: bare `adds #1` and a signed `ble` with no narrowing
 * anywhere. r6 = i << 16 in pass 2 and the two walking `* 0x3c` offsets in
 * pass 3 (r2, r8) are strength_reduce givs and LICM hoists, not source --
 * pass 3 is written with the ordinary gPlayers[i] subscript.
 *
 * AreArmiesOnSameTeam had no declaration before this function; added to
 * include/unknown-functions.h from the promoted src/decomp/c_08026F28.c. An
 * implicit declaration compiles but default-promotes both arguments to int and
 * drops the `lsls/lsrs #0x10` pairs the ROM has in front of that `bl`.
 */
u8 IsOnlyOneTeamLeft(void)
{
    u8 buf[8];
    int i;
    int j;
    int count;

    for (i = 1; i <= 4; i++)
    {
        buf[i] = 0;

        if (IsPlayerAliveAndActive(i) && CheckArmySurvivesTeamRule(i) && CheckArmySurvivesHumanNoUnitsRule(i)
         && CheckArmySurvivesNoUnitsRule(i) && CheckArmySurvivesHqRule(i))
            buf[i] = 1;
    }

    if (gPlaySt.event20 & 4)
    {
        for (i = 1; i <= 4; i++)
        {
            if (buf[i])
            {
                for (j = 1; j <= 4; j++)
                {
                    if (i != j && IsPlayerAliveAndActive(j) && AreArmiesOnSameTeam(i, j) && !buf[j])
                    {
                        buf[i] = 0;
                        MarkPlayerYielded(i);
                    }
                }
            }
        }
    }

    for (i = 1; i <= 4; i++)
    {
        if (buf[i])
        {
            count = 0;

            for (j = 1; j <= 4; j++)
            {
                if (i != j && buf[j]
                 && gPlayers[i].team != gPlayers[j].team)
                    count++;
            }

            if (count == 0)
                return 1;
        }
    }

    return 0;
}
asm(".global sub_08028BAC\n.thumb_set sub_08028BAC, IsOnlyOneTeamLeft\n");
