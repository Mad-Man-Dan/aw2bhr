#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C684.
 * sub_0802C684 @ 0x0802C684, sub_0802C69C @ 0x0802C69C
 */

/* A bool8 pass-through of IsLinkGame, written the way src/decomp/c_0802C5F0.c
 * writes its predicate -- test for the FALSE case and return FALSE, then return
 * TRUE -- and NOT as `if (IsLinkGame()) return TRUE; return FALSE;`.
 *
 * The two are distinguishable and this is the cheap place to read the rule:
 * agbcc always emits the `if`'s THEN arm as the block after the fallthrough
 * return. So `if (c) return TRUE; return FALSE;` puts `movs #0` first and
 * `movs #1` last, which is the mirror of the ROM, while the negated form puts
 * `movs #1` first. sub_0802C69C in this same block is the other side of the
 * same coin: its THEN arms return TRUE, so its `movs #0` comes first, and the
 * matched family F046 next door agrees.
 *
 * `lsls #0x18` with no `lsrs` is the truth test on IsLinkGame's declared
 * bool8 return.
 */

bool8 OptionsMenu_ExitMapUsability(void)
{
    if (!IsLinkGame())
        return FALSE;

    return TRUE;
}
asm(".global sub_0802C684\n.thumb_set sub_0802C684, OptionsMenu_ExitMapUsability\n");

/* Family F046's shape (src/decomp/c_0802C6CC.c) with its third test inverted:
 * the four matched members ask `unk09 != K` for K = 0..3, this one asks
 * `unk09 == 0`. Everything else -- the two callee narrowings, the explicit
 * `if (...) return TRUE;` chain, the pool sitting between the `movs #0` and the
 * `movs #1` -- is byte-identical to them, so nothing here had to be re-derived.
 *
 * `lsls #0x18; lsrs #0x18; cmp #1` is IsLinkGame's bool8 result kept for a
 * compare against a non-zero constant; the bare `lsls #0x18; cmp #0` on
 * IsMapCategoryZero's is a truth test.
 *
 * Nothing in the ROM calls this, so bool8 is the family's convention rather
 * than a proved return type -- the body returns literal 0/1, which is
 * byte-identical for `int`.
 */

bool8 sub_0802C69C(void)
{
    if (IsLinkGame() == TRUE)
        return TRUE;

    if (IsMapCategoryZero())
        return TRUE;

    if (gPlaySt.animOpts == 0)
        return TRUE;

    return FALSE;
}
