#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076E20.
 * sub_08076E20 @ 0x08076E20
 */

/* Wave 57 (W57-D). MATCHED (was parked at 93.4% since wave 35).
 *
 * The inherited draft's instruction stream was already right end to end; both
 * of its two remaining spots were CONSTANT PLACEMENT / OPERAND ORDER, and both
 * fell to a spelling change, not to a structural one.
 *
 *  +0x04, 2 bytes.  The parameter is `u16`, not `int` with a `(u16)` cast, and
 *      `k = 4;` is the FIRST statement.  A sub-word parameter's PROMOTE_MODE
 *      zero-extension is emitted as the prologue's own insn group, and the
 *      first statement's constant is emitted AFTER it -- which is exactly the
 *      ROM's `lsls r0,#0x10 / movs r3,#4 / lsrs r2,r0,#0x14`.  combine folds
 *      the prologue's `lsrs #0x10` into the statement's `>> 4` to make the
 *      single `lsrs #0x14`, so the pair the old note in unknown-functions.h
 *      said would be "a different pair" is in fact the ROM's.  With `int` plus
 *      a cast there is no prologue group and `movs #4` has nowhere to sit but
 *      in front of the `lsls`.  Probed both ways; see docs/agbcc-codegen.md.
 *      include/unknown-functions.h retyped `int` -> `u16` accordingly, and its
 *      only caller (src/decomp/c_0807703C.c, which already passes a `u16`)
 *      was re-verified: still MATCHED, byte-identical.
 *
 *  +0xb8, 14 bytes.  The max is a TERNARY, `m = a > b ? a : b;`, not
 *      `m = b; if (b < a) m = a;`.  Both keep m in its own pseudo, but only the
 *      ternary makes gcc emit the `m = b` plain copy BEFORE the compare's
 *      `lsls #0x10`, so a and b survive in their own registers and the taken
 *      arm is a second plain copy (`adds r2,r3,#0`).  The if-form computes
 *      `b << 16` for the compare first and then recovers m from it with a
 *      `lsrs #0x10`, and has to recover a the same way.  The ternary probe
 *      reproduced the ROM's register numbers (a in r3, b in r0, m in r2) as
 *      well as its instructions.  `if (a > b)` only swaps the compare operands
 *      and changes nothing else -- it is not the lever.
 *
 * Kept from the inherited draft, all of it still load-bearing:
 *   - gUnknown_08614588 is `s16 [][2]`; a struct of two s16 comes out two
 *     instructions short and a flat `s16 []` two long (recorded on the global).
 *   - Both modulo tests are __umodsi3, so gGameClock is read through a
 *     (u32) -- it is declared s32 and a signed operand would call __modsi3.
 *   - The `lsls #0xf; lsrs #0x10` pairs are `(s16)(t >> 1)` after the abs, not
 *     a division: no `lsr #31` bias, unlike the easing `/ 2`s above them.
 *
 * Eases gSmoothScroll's current pair toward the gUnknown_08614588 target,
 * hands each halved axis to its clamp, and rumbles while either is moving. */
void UpdateWorldMapCursor(u16 a1)
{
    int n;
    int k;
    int t;
    int v;
    s16 a;
    s16 b;
    s16 m;

    k = 4;
    n = (a1 >> 4) & 0xF;

    if (n != 0)
        k = 3;

    gSmoothScroll.targetX = gUnknown_08614588[n][0] * 2;
    gSmoothScroll.targetY = gUnknown_08614588[n][1] * 2;

    if ((u32)gGameClock % k == 0)
    {
        gSmoothScroll.currentX +=
            (gSmoothScroll.targetX - gSmoothScroll.currentX) / 2;
        gSmoothScroll.currentY +=
            (gSmoothScroll.targetY - gSmoothScroll.currentY) / 2;
    }

    v = MoveWorldMapCursorX(gSmoothScroll.currentX / 2)
        + MoveWorldMapCursorY(gSmoothScroll.currentY / 2);

    if (v == 0)
    {
        gSmoothScroll.frameCounter = 0;
    }
    else
    {
        t = gSmoothScroll.currentX;

        if (t < 0)
            t = -t;

        a = t >> 1;

        t = gSmoothScroll.currentY;

        if (t < 0)
            t = -t;

        b = t >> 1;
        m = a > b ? a : b;

        if (gSmoothScroll.frameCounter % (8 - m) == 0)
            PlaySeSpacial(0x1D0, gUnknown_0202FDFC.unk04);

        gSmoothScroll.frameCounter++;
    }
}
asm(".global sub_08076E20\n.thumb_set sub_08076E20, UpdateWorldMapCursor\n");
