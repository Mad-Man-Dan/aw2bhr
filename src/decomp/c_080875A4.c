#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080875A4.
 * sub_080875A4 @ 0x080875A4
 */

/* Wave 55 (W55-G), matched after parks by W55-D at 92.5% and an earlier agent.
 * Both of W55-D's carefully-measured negative results were beaten by ONE lever,
 * now written up in docs/agbcc-codegen.md: BIND THE SUBEXPRESSION TO A LOCAL.
 * `fold` runs on one expression tree and a local assignment is a separate
 * statement, so `associate` has nothing left to restructure.
 *   - `m` blocks the `C1 - (x - C2) * C3` fold that was costing a pool word and
 *     the +4 bytes (six in-expression spellings had been measured as folding);
 *   - `u` and `t` block the reassociation that kept moving the 0x400 onto the
 *     shift term, and the outer combine really is `|` -- the previous note's
 *     "the original must have used a macro" conclusion was wrong.
 * Two corollaries, both of which cost an attempt here: bind in the ROM's
 * EVALUATION order (statements are emitted in source order, so binding only the
 * second operand moves its computation ahead of the first), and bind
 * SINGLE-ASSIGNMENT -- `x = ...; x = f(x);` costs a `mov` because gcc gives the
 * incoming value and the local separate pseudos, which is why `v` is a fresh
 * name rather than a reuse of `x`.
 *
 * W55-D's other findings all held and were not re-derived: the record test is
 * c_08087548.c's idiom with the subscript inline for the pool order, the fifth
 * argument 2 is CSEd into r6 and re-`str`ed at each call, and the digit guards
 * are `> 0x63`, `> 9`, `>= 0` in that order with their own DivRem/Div pairs.
 *
 * The proc parameter's struct is file-local by the parameter-struct rule; only
 * unk38 is proved -- an `int` loop bound, re-read every iteration, and the load
 * for the bound test is CSEd across the back edge into the `- 2` at the top of
 * the next iteration's body.
 */

struct Unk80875A4Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
};

void DrawWarRoomRecordSummary(int a, int n, struct Unk80875A4Proc *proc)
{
    int i;
    int x;
    int m;
    int v;
    int u;
    int t;

    DrawOamObject(0x52, 0xd0, 0x88, 0, 2);
    DrawOamObject(0x53, 0, 0x88, 0, 2);
    DrawOamObject(0x60, 0x4e, 0x94, 0, 2);

    if (gUnknown_0200C078[gUnknown_02027F74.unk04[a] - 0x6c].unk00[0].unk00_08 != 0)
    {
        if (n > 0x63)
            DrawOamObject(Div(DivRem(n, 1000), 100) + 0x55, 0x36, 0x94, 0, 2);
        if (n > 9)
            DrawOamObject(Div(DivRem(n, 100), 10) + 0x55, 0x3e, 0x94, 0, 2);
        if (n >= 0)
            DrawOamObject(DivRem(n, 10) + 0x55, 0x46, 0x94, 0, 2);

        PutCoMinimugSprite(0x18, 0xa0, 0x7490, 6);
    }

    for (i = 0; i < proc->unk38 - 1; i++)
    {
        x = i * 0x20 + 0xd8;
        m = proc->unk38 - 2;
        v = (x - m * 0x20) | 0x1000;
        u = (i + 8) << 12;
        t = ((i + 1) * 12 + 0x90) | 0x400;
        PutCoMinimugSprite(v, 0xa0, u | t, 6);
    }
}
asm(".global sub_080875A4\n.thumb_set sub_080875A4, DrawWarRoomRecordSummary\n");
