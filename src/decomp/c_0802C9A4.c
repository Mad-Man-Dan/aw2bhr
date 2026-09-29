#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C9A4.
 * sub_0802C9A4 @ 0x0802C9A4, sub_0802C9E8 @ 0x0802C9E8
 */

/* An `if (...) return TRUE;` chain in UnitMenu_DiveUsability's layout: the merged
 * `movs r0, #1` sits after the pool.
 *
 * gPlayers is a POINTER to the 0x3c-byte records, so the index
 * expansion is the usual `lsls #4; subs; lsls #2` (i * 60) added to the loaded
 * base, and unk1d is already named on the struct. gUnknown_030033EC indexes it
 * directly as a u16, no narrowing.
 *
 * sub_0802C9E8 next door is the same three tests reordered and with the middle
 * one's sense flipped -- not a duplicate, despite identical size, call count
 * and data_refs.
 */

bool8 sub_0802C9A4(void)
{
    if (gPlaySt.coAbilities == 0)
        return TRUE;

    if (sub_0802C958())
        return TRUE;

    if (gPlayers[gUnknown_030033EC].co != 4)
        return TRUE;

    return FALSE;
}

/* sub_0802C9A4's three tests reordered, and NOT a duplicate of it despite
 * identical size, call count and data_refs -- the batch-by-locality warning in
 * the wave brief, met in practice.
 *
 * sub_0802C958 moves to the front and keeps its `return TRUE`, while the unk08
 * guard flips to `return FALSE` and the unk1d test flips to `== 4`. The layout
 * follows: `movs r0, #0` is the block before the pool here, where sub_0802C9A4
 * has `movs r0, #1` there. Two `return FALSE` sites merge into that block.
 */

bool8 sub_0802C9E8(void)
{
    if (sub_0802C958())
        return TRUE;

    if (gPlaySt.coAbilities == 0)
        return FALSE;

    if (gPlayers[gUnknown_030033EC].co == 4)
        return TRUE;

    return FALSE;
}
