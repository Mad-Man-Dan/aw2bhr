#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08087514.
 * sub_08087514 @ 0x08087514
 */

/* MATCHED, wave 79 (W79-B). Parked since wave 30 at 48 of 52 bytes, with every
 * instruction and every register assignment identical to the ROM except the
 * first argument. The ROM builds it in four instructions --
 *     movs r3,#2 / movs r0,#5 / subs r3,r0,r3 / adds r0,r3,#0 / adds r0,#0xb8
 * -- while agbcc folded every spelling tried to the two-instruction
 * `movs r0,#0xbd; subs r0,r0,r3`.
 *
 * THE LEVER, and it was written down in the park's own diagnosis without being
 * recognised as a spelling: `subs r3, r0, r3` writes the difference back into
 * the PARAMETER'S OWN register. The source is `a = 5 - a;`, an assignment to
 * the parameter, not a fresh temporary. The seven ruled-out spellings are all
 * either literal forms (folded in the front end) or FRESH single-set locals
 * (`int t = 5 - a;`), which agbcc propagates and then combines. Assigning back
 * to `a` makes `a` a MULTI-SET pseudo -- it is already set by the clamp -- and
 * a multi-set pseudo is not propagated into its use, so the sum survives as its
 * own value and the argument needs the ROM's separate `adds r0,r3,#0;
 * adds r0,#0xb8`.
 *
 * Same family as the wave-57 rule that `nonzero_bits` is tracked only for a
 * SINGLE-SET pseudo, so masking a parameter IN PLACE keeps a later truncation
 * alive. Recorded as a general rule in docs/agbcc-codegen.md: when agbcc folds
 * a constant into an expression that the ROM keeps separate, try assigning to
 * an ALREADY-ASSIGNED variable rather than to a fresh one. The park's own
 * conclusion -- "blocking it needs the subtraction's result to have a SECOND
 * use, and nothing in this function gives it one" -- was looking for the wrong
 * property: what blocks the fold is a second SET, not a second use.
 *
 * Everything else was settled in wave 30 and cost real probes:
 *   - the clamp is UNSIGNED (`bhi`), hence `u32` on the first parameter;
 *   - the row index must be written `b * 2 + 5 + c` and NOT `b * 2 + (c + 5)`;
 *   - the byte offset must sit INSIDE the pointer add, `p + (idx * 32 + 9)`. */
void DrawMapListRankIcon(u32 a, int b, int c)
{
    if (a < 2)
        a = 2;

    a = 5 - a;

    PutTilePoolGraphicTilemap(a + 0xB8,
                 gBG0TilemapBuffer + ((b * 2 + 5 + c) * 32 + 9));
}
asm(".global sub_08087514\n.thumb_set sub_08087514, DrawMapListRankIcon\n");
