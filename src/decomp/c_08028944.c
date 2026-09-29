#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028944.
 * sub_08028944 @ 0x08028944
 */

/* "Is army `a` still playable?" -- only asked at all when gPlaySt's
 * +0x04 flag byte has bit 1 set, and then it is `IsPlayerAliveAndActive(a) && !unk32`.
 *
 * The RETURN-BLOCK ORDER is a readout: the ROM's `movs r0,#1; b` sits inline
 * where the tests fall through and the `movs r0,#0` is at the very end, which
 * is the early-`return TRUE` spelling. Nesting the two inner tests inside the
 * flag test and returning TRUE at the bottom emits them the other way round --
 * same size, different bytes. */
bool8 CheckArmySurvivesHqRule(u16 a)
{
    if ((gPlaySt.event20 & 2) == 0)
        return TRUE;

    if (IsPlayerAliveAndActive(a) == 0)
        return FALSE;

    if (gPlayers[a].killOnEndTurn != 0)
        return FALSE;

    return TRUE;
}
asm(".global sub_08028944\n.thumb_set sub_08028944, CheckArmySurvivesHqRule\n");
