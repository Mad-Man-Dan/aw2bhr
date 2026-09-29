#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077214.
 * sub_08077214 @ 0x08077214
 */

/* MATCHED, wave 79 (W79-B). Parked at 91.7% since wave 32 with a ten-byte,
 * size-exact, ORDER-ONLY residual: the ROM narrows the three stack arguments in
 * argument order (a6 `lsls`, a7 `lsls; lsrs` -> r7, then a8 `lsls; lsrs` -> r9)
 * and the draft narrowed a8 first, because `v = a8;` was the first statement.
 * W32-A concluded "statement order cannot reach that, because the guard's
 * operands are only evaluated inside the `if`". That was the one wrong step.
 *
 * THE LEVER: bind the guard's two operands to their own `int` locals, in
 * argument order, BEFORE `v`. The narrowings are then created as ordinary
 * statements in source order, and `combine` still produces the ROM's compare of
 * the two `<<16` forms and still kills a6's redundant `lsrs`, because `w` has
 * no other use. `h` keeps its `lsrs` (it is compared, then returned).
 *
 * THE LOCALS MUST BE `int` WITH A `(u16)` CAST, NOT `u16`. Declaring them u16
 * -- which is the same statement order -- is 8 bytes WORSE: the compare is then
 * between two already-narrow pseudos, `combine` cannot rewrite it into the
 * `<<16` form, and both narrowed values stay live to the compare, which costs a
 * third high register (sl) and pushes it. That is the same 128-byte failure the
 * wave-32 note recorded for `u16` PARAMETERS and it has the same cause.
 * (Ruled out again here from both directions.)
 *
 * The fourth argument to StartTextBox must stay the raw `a7`, not `h`: the
 * callee re-narrows it, CSE supplies r7, and the copy lands in the ROM's
 * position at the end of the argument group. Passing `h` explicitly is 15 bytes
 * -- it hoists `adds r3,r7,#0` above the stack-argument stores.
 *
 * Rebuilds a text box only when its two size arguments disagree: it clears the
 * old one, redraws through FillTilemapRect and allocates a fresh StartTextBox slot
 * whose unk3a it seeds from the last argument. Returns the seventh argument
 * narrowed to a halfword, which its one caller (WorldMapMissionInfo_InputLoop) discards.
 *
 * Settled earlier and unchanged: it RETURNS a value (wave 31 called it void
 * from a call site that discards the result); StartTextBox's first two
 * parameters are `s16`, not `u16`; the eighth argument must be a bound `u16`
 * local, because read straight off the stack at its use it needs no narrowing
 * at all and the frame comes out two registers smaller. */
int ReplaceTextBoxIfTextChanged(u16 *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    int w;
    int h;
    u16 v;

    w = (u16)a6;
    h = (u16)a7;
    v = a8;

    if (w != h)
    {
        sub_08014878();
        FillTilemapRect(a1, a2, a3, a4, a5, 0);
        StartTextBox((s16)a2, (s16)a3, a1, a7, 0, 0x280)->unk3a = v;
    }

    return h;
}
asm(".global sub_08077214\n.thumb_set sub_08077214, ReplaceTextBoxIfTextChanged\n");
