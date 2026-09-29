#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041D40.
 * sub_08041D40 @ 0x08041D40
 */

/* Orders two units for a queue: by the cell column unk02 first, then by the
 * low seven bits of their army's gPlayers record.
 *
 * `(p - gUnits) >> 6` is the matched IsIndirectFireUnitArmed's army idiom --
 * exact division by the 0x0c stride (`mul 0x55555555; neg; asr #2`) with the
 * `>> 6` merged into the single `asr #8`. The `+ 1` then indexes
 * gPlayers ONE-BASED, which is what puts the member at a runtime
 * 0x3c + 0x2d == 0x69: exactly the reach struct PlayerStruct's own unk2a note
 * records for AreUnitsOnSameTeam/IsTerrainOwnedByUnitsTeam, which read element [n + 1] the same
 * way.
 *
 * The last test is written `<= ... return 0` and NOT `> ... return 1`. Both are
 * the same predicate, but only this polarity leaves `return 1` inline ahead of
 * the literal pool with `return 0` after it; the other spelling swaps the two
 * blocks. Same lever c_0804203C.c documents for its own pair of returns. */
int GetBattleSceneAttackerSide(struct Unit *a, struct Unit *b)
{
    if (a->x > b->x)
        return 1;

    if (a->x != b->x)
        return 0;

    if ((gPlayers[((a - gUnits) >> 6) + 1].hqX & 0x7f)
     <= (gPlayers[((b - gUnits) >> 6) + 1].hqX & 0x7f))
        return 0;

    return 1;
}
asm(".global sub_08041D40\n.thumb_set sub_08041D40, GetBattleSceneAttackerSide\n");
