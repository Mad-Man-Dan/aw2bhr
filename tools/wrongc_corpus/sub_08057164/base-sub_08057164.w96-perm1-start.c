#include "global.h"

/* PARKED, and the least converged of the batch. The SHAPE is right -- every
 * branch, both index expressions, the 55 * y multiply, the five stores and the
 * `adds r5,#0x24` walk all appear -- but the loop's induction variables are
 * allocated differently and that shifts most of the body.
 *
 * The one difference everything else follows from:
 *
 *   ROM   r7 is the ELEMENT index `a * 10 + y * 5 + i`, `adds r7,#1` per
 *         iteration, and each of the three tables scales it at the use
 *         (`lsls r0,r7,#1; add r0,sl` / `lsls r1,r7,#1; adds r1,r1,r0`).
 *         gUnknown_0855203C's ADDRESS is the invariant that wins the
 *         callee-saved register (sl), and gUnknown_085521DC is rematerialised
 *         inside the loop (`ldr r3,=gUnknown_085521DC` at _0805721A).
 *   here  r7 is the BYTE index (`adds r7,#2`) and GCC additionally builds an
 *         address giv r4 = &gUnknown_0855203C[a * 10 + y * 5 + i] (`adds r4,#2`)
 *         for the `y == gUnknown_0300450C` arm, then reloads a SECOND pool word
 *         for gUnknown_0855203C in the other arm. gUnknown_085521DC gets sl
 *         instead.
 *
 * So the job is to stop strength_reduce turning `gUnknown_0855203C[idx + i]`
 * into an address giv while leaving the unscaled sum as one. Binding
 * `u16 *tbl = gUnknown_0855203C;` before the loop was TESTED and does not do it
 * -- the giv forms off the pointer pseudo just as happily. Untried: a real
 * counter variable stepped at the bottom of the loop body (`k = a * 10 + y * 5;
 * ... k++;`) rather than an `i`-derived expression, which is the shape the ROM's
 * `adds r7,#1` beside `adds r5,#0x24` actually reads like.
 *
 * Secondary, and probably downstream of the above: the ROM builds r7 as
 * `(a * 10 + y * 4) + y` while reusing `y * 4` from the `y * 5` it stores at
 * sp+4, where this candidate computes `y * 5` once and adds it; and the ROM
 * materialises the zero for the three cleared members inside the loop
 * (`movs r2,#0`) where this hoists it into a register.
 *
 * SETTLED, do not re-derive:
 *  - the gUnknown_085D6A48 row must be a local STRUCT (columns 2 and 8 stay in
 *    the `ldrh` displacement, `[r1,#4]` and `[r1,#0x10]`, off one shared row
 *    address) and the cast must sit inside the loop -- c_080579B8.c's rule.
 *  - the third parameter is `u16` (the prologue's `lsls #0x10; lsrs #0x10`) and
 *    it is the ROW of gUnknown_02029A10 / gUnknown_03004582 as well as the
 *    thing compared against gUnknown_0300450C.
 *  - gUnknown_0855203C / gUnknown_08551F60 / gUnknown_08551E84 take one flat
 *    index `side * 10 + row * 5 + slot` and gUnknown_085521DC takes
 *    `row * 55 + side * 5 + slot`, read twice with the two different sides into
 *    unk00 and unk01. The declarations and the evidence are in
 *    include/unknown-globals.h.
 *  - struct Unk02029A10.unk02 was carved out of filler_02 for the `strb 0` at
 *    offset 2; byte-neutral, proto_check clean.
 * Wave 70, W70-B: configured re-verdict remains 268/268. A real `idx` counter
 * plus explicit `e++` record walk produces the desired unscaled `idx += 1`,
 * but strength_reduce still creates `gUnknown_0855203C + 2*idx` as a second
 * address GIV. Removing `tbl` does not change that decision. */

struct Unk085D6A48Row
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x0a];
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 filler_12[0x06];
};

void sub_08057164(int a, int b, u16 y)
{
    struct Unk02029A10 *e;
    struct Unk085D6A48Row *rows;
    u16 *tbl;
    int i;
    int idx;

    e = gUnknown_02029A10[y].entries;
    tbl = gUnknown_0855203C;
    idx = a * 10 + y * 5;

    for (i = 0; i <= 4; i++)
    {
        rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;
        if (rows[gUnknown_03004582[y][0]].unk04 == 1)
        {
            if (y == gUnknown_0300450C)
                e[i].unk1a = tbl[idx + i];
            else
                e[i].unk1a = tbl[b * 10 + y * 5 + i];
        }
        else if (rows[gUnknown_03004582[y][0]].unk10 == 1)
            e[i].unk1a = gUnknown_08551F60[idx + i];
        else
            e[i].unk1a = gUnknown_08551E84[idx + i];

        e[i].unk00 = gUnknown_085521DC[y * 55 + a * 5 + i];
        e[i].unk01 = gUnknown_085521DC[y * 55 + b * 5 + i];
        e[i].unk02 = 0;
        e[i].unk04 = 0;
        e[i].unk06 = 0;
    }
}






