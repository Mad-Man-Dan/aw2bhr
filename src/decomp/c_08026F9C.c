#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08026F9C.
 * sub_08026F9C @ 0x08026F9C, sub_08026FD0 @ 0x08026FD0
 */

/* `>> 6` on an s16, then element [n + 1]: agbcc folds the +1 element (0x3c)
 * into the member offset (0x2a) and emits a single `adds r2, #0x66`.
 */
bool8 AreUnitsOnSameTeam(s16 a, s16 b)
{
    if (gPlayers[(a >> 6) + 1].team == gPlayers[(b >> 6) + 1].team)
        return TRUE;

    return FALSE;
}
asm(".global sub_08026F9C\n.thumb_set sub_08026F9C, AreUnitsOnSameTeam\n");

/* Two separate `if`s, not `&&`: the shared `return FALSE` block has to sit
 * directly after the second compare with the TRUE block behind the pool, and
 * `if (t != 0 && ...)` inverts that second branch instead.
 *
 * `a` is s16 and not u16 (corrected in wave 18): the body is byte-identical
 * either way, but three call sites load the argument with `ldrsh`, which a u16
 * parameter cannot produce. See include/unknown-functions.h.
 */
bool8 IsTerrainOwnedByUnitsTeam(s16 a, u8 b)
{
    u32 t = b >> 5;

    if (t == 0)
        return FALSE;

    if (gPlayers[(a >> 6) + 1].team == gPlayers[t].team)
        return TRUE;

    return FALSE;
}
asm(".global sub_08026FD0\n.thumb_set sub_08026FD0, IsTerrainOwnedByUnitsTeam\n");
