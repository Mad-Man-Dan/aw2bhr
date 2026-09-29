#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080265D0.
 * sub_080265D0 @ 0x080265D0
 */

/* MATCHED in wave 65 (W65-N), 260/260 bytes under the configured compiler.
 * The historical wave-45 fixpoint was +4 bytes / TWO instructions. The whole body is
 * byte-exact from the first loop's preheader to the end of the function; the
 * residual is entirely in the entry block. Do NOT read the 15.8% as a shape
 * error -- the score is positional and the two extra instructions sit at +0x18,
 * so every later byte is shifted. The 33.9% in best.c is an EARLIER and
 * structurally WORSE draft; THIS file is the one to continue from.
 *
 * WHAT THE FUNCTION DOES
 * Scales a payout for army a2 by how far gUnknown_03004080 has drifted from
 * that chapter record's unk20, then hands it to AddArmyBonusPointsNoOp with a1. Three
 * mutually exclusive comparisons, each re-reading both globals:
 *   X <  Y : start at 500 and grow by 1.2 per step, capped at 9999
 *   X == Y : 500 flat
 *   X >  Y : start at 500 and shrink by 150 per step, floored to 0
 * where X = gUnknown_03004080 and Y = gUnknown_085C77A0[a2].unk20.
 *
 * FOUR THINGS SETTLED HERE, none of them guessable from the shape:
 *
 * 1. `i` is s16, NOT u16. The increment is `lsls #0x10; adds #0x10000; lsrs
 *    #0x10; asrs #0x10` -- the value is kept zero-extended and SIGN-extended
 *    again at the use, which is the s16 local pattern. u16 emits the 3-insn
 *    `adds #1; lsls; lsrs` and compares the zero-extended value.
 *
 * 2. AddArmyBonusPointsNoOp's SECOND PARAMETER IS u16, and this is the only caller that
 *    can prove it -- the `lsls #0x10; lsrs #0x10` on the value immediately
 *    before the `bl`, on a quantity held at full width everywhere else.
 *    include/unknown-functions.h retyped from `int`; c_080265B0.c re-verified
 *    as still MATCHED (it passes literals, where the two types are identical
 *    code, which is why the wrong reading survived).
 *
 * 3. `v` is u32/unsigned. `__floatsidf` followed by a CONDITIONAL `__adddf3` of
 *    2^32 is agbcc's inline unsigned-to-double conversion, not a signed one,
 *    and the result returns through `__fixunsdfsi`. Both compares (9999, 149)
 *    are unsigned.
 *
 * 4. The loop bottom tests the COUNTER before the value, and the clamp hangs
 *    off the value test only -- so the counter-exhausted exit leaves v
 *    UNCLAMPED. That is not what `for (...) { v = v*1.2; if (v > 9999) {...;
 *    break;} }` produces (that puts the value test BEFORE the increment), and
 *    the difference is semantic, not cosmetic. Writing the counter test as an
 *    explicit `break` after `i++` is what puts the two in the ROM's order and
 *    lets the value test own the back edge. `&&`-ing them into the condition
 *    gets the order right but leaves a redundant bottom test.
 *
 * THE REMAINING TWO INSTRUCTIONS
 * The ROM REUSES the gUnknown_03004080 value it already loaded for the `X < Y`
 * test when it computes `Y - X` for the loop guard:
 *      ldrh r0, [r6]        <- X, r6 = the deref'd force-addr word
 *      ldrh r2, [r1, #0x20] <- Y
 *      cmp r0, r2 ; bcs
 *      ...
 *      ldrh r1, [r1, #0x20] <- Y again
 *      subs r0, r1, r0      <- reuses X, still live in r0
 *      ...
 *      adds r6, r1, #0      <- r6 now holds Y; THE X POINTER IS DEAD
 * The candidate re-loads X instead, so that pointer stays live, cannot have r6,
 * and gets parked in ip -- costing `mov ip, r0` plus one `mov rN, ip` (+4).
 * Everything from the guard onwards, including the entire first loop body with
 * its `subs r0, r6, r0`, is already byte-identical.
 *
 * RULED OUT, do not re-measure:
 *   - binding Y to a local inside the `if` (as here) vs not binding it: the
 *     binding is REQUIRED. Without it agbcc holds the ELEMENT POINTER across
 *     the loop and re-loads unk20 every iteration -- a different and worse
 *     residual. Two loads of unk20 before the loop is correct; the ROM does it.
 *   - binding the DIFFERENCE `Y - X` before the loop: wrong. The ROM re-reads X
 *     inside the loop (the soft-float calls clobber memory) while holding Y, so
 *     only Y can be the local.
 *   - `u16 i`, and every value-test-before-increment loop shape (see 1 and 4).
 *
 * MATCHING LEVER: put the entry guard before `n = Y`, then bind `n` inside the
 * guarded body. This makes `Y - X` the first post-compare use and preserves X
 * in r0, while the following `adds r6, r1, #0` becomes the binding of Y. The
 * resulting source matches; the two force-addr pool words at 0x08090A68 and
 * 0x08090A6C must be carried during promotion. */
void AwardSpeedBonusPoints(u8 a1, u8 a2)
{
    u32 v;
    s16 i;
    u16 n;

    v = 0;

    if (gUnknown_03004080 < gUnknown_085C77A0[a2].speedRankTurnLimitNc)
    {
        v = 500;
        i = 0;
        if (i < gUnknown_085C77A0[a2].speedRankTurnLimitNc - gUnknown_03004080)
        {
            n = gUnknown_085C77A0[a2].speedRankTurnLimitNc;

            while (1)
            {
                v = v * 1.2;
                i++;
                if (i >= n - gUnknown_03004080)
                    break;
                if (v > 9999)
                {
                    v = 9999;
                    break;
                }
            }
        }
    }

    if (gUnknown_03004080 == gUnknown_085C77A0[a2].speedRankTurnLimitNc)
        v = 500;

    if (gUnknown_03004080 > gUnknown_085C77A0[a2].speedRankTurnLimitNc)
    {
        v = 500;
        i = 0;
        while (i < gUnknown_03004080 - gUnknown_085C77A0[a2].speedRankTurnLimitNc)
        {
            v -= 150;
            i++;
            if (i >= gUnknown_03004080 - gUnknown_085C77A0[a2].speedRankTurnLimitNc)
                break;
            if (v <= 149)
            {
                v = 0;
                break;
            }
        }
    }

    AddArmyBonusPointsNoOp(a1, v);
}
asm(".global sub_080265D0\n.thumb_set sub_080265D0, AwardSpeedBonusPoints\n");
