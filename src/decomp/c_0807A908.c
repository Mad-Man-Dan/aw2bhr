#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A908.
 * sub_0807A908 @ 0x0807A908
 */

/* Return the first team index whose IsPlayerAliveAndActive predicate holds, or 0.
 *
 * The preamble's three tests are one short-circuit `||`: the guarded block runs
 * when the mode byte is not 1 OR either GetCampaignMissionId reading is 0x21/0x22, and
 * the loop is the fall-through of all three.
 *
 * `i + 1` is written twice on purpose. It is the loop's only non-test use of
 * the counter, and it is what puts the `adds r4, #1` at the TOP of the body;
 * IsPlayerAliveAndActive's `u8` parameter then makes `(i + 1) << 24` a giv, which
 * strength_reduce accumulates in r5 (init `movs #0x80; lsls #0x11` = 1 << 24,
 * step the same constant rematerialised in the loop) so the narrowing at the
 * call is a lone `lsrs r0, r5, #0x18` instead of a shift pair. Hoisting the
 * `+ 1` into a second variable, or writing the counter as `u8`, loses the giv
 * and costs 8 bytes -- see docs/agbcc-codegen.md. */
int GetResultsArmy(void)
{
    int i;

    if (gPlaySt.gameMode != 1 || GetCampaignMissionId() == 0x21 || GetCampaignMissionId() == 0x22)
        if (IsPlayerAliveAndActive(gUnknown_030033EC))
            return gUnknown_030033EC;

    i = 0;
    while (i < GetLoadedMapArmyCount())
    {
        if (IsPlayerAliveAndActive(i + 1))
            return i + 1;
        i++;
    }

    return 0;
}
asm(".global sub_0807A908\n.thumb_set sub_0807A908, GetResultsArmy\n");
