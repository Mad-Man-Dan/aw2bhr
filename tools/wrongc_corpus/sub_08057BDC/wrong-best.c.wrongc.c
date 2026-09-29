#include "global.h"

/* PARKED. 4 bytes SHORT of 360. The whole residual is in the SECOND loop and
 * it is one fact: the ROM hoists gUnknown_085D6A48 and gUnknown_03004582 into
 * callee-saved registers in that loop's preheader (`ldr;mov sb` / `ldr;mov r8`,
 * then `add r0,r8` and `add r0,sb` in the body) and rematerialises
 * gUnknown_08551A04's address inside the body once per iteration
 * (`ldr r4,=gUnknown_08551A04`, then `ldr r0,[r4]` twice). This candidate does
 * the opposite: it hoists gUnknown_08551A04 and rematerialises the other two,
 * which is -6 bytes of preheader and +4 of body, net -4, and it also reorders
 * the last nine pool words. Everything before that -- the whole first loop, the
 * guards, the fixed-point step and both calls -- is byte-exact.
 *
 * Levers TESTED on that hoist, all worse or neutral, do not retry:
 *  - `rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;` before the second
 *    loop (as written here): the assignment is const-propagated straight back
 *    into the body, so it changes nothing.
 *  - additionally binding `u16 (*slots)[8] = gUnknown_03004582;`: GCC
 *    strength-reduces it into a `+0x10` cursor, which the ROM does NOT have in
 *    this loop (it does in loop 1, off gUnknown_03004580).
 * The lever that is left untried is anything that stops gUnknown_08551A04's
 * address winning the hoist -- it is the only one of the three used twice per
 * iteration, which is presumably why loop.c prefers it here.
 *
 * Two findings that ARE settled and cost an attempt each:
 *  - `p->unk00 + (14 - c)` and NOT `p->unk00 + 14 - c`. The latter is folded to
 *    `x - (c - 14)`, three instructions; the parenthesised form gives the ROM's
 *    `adds r1,#0xe / subs r1,r1,r4`. That alone was 4 bytes.
 *  - `(u32)gUnknown_030005D8[i] >> 16`. gUnknown_030005D8 is `int` (W48-G's
 *    evidence, do not retype it), so the bare `>> 16` is `asrs`; the ROM has
 *    `lsrs`, and the cast is byte-neutral everywhere else in the statement.
 *
 * The two gUnknown_085D6A48 lookups use DIFFERENT spellings on purpose and both
 * are right: loop 1 reads `gUnknown_03004580[i][1]` (the ROM's pool word is
 * =gUnknown_03004580 plus `adds #2`, strength-reduced to a +0x10 cursor) and
 * loop 2 reads `gUnknown_03004582[i][0]` (its own pool word). The row cast is
 * c_080579B8.c's, and it must stay inside loop 1 for the same reason that file
 * records.
 * Wave 70, W70-B: configured re-verdict remains 356/360. Pointer self-use,
 * self-assignment, and pointee `+= 0` do not stop gUnknown_08551A04 winning
 * the hoist. Dead local uses can restore size through allocation punctuation,
 * but keep that wrong hoist and broadly permute the first loop. */

struct Unk8057Pos
{
    u16 x;
    u16 y;
};

struct Unk085D6A48Row
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x14];
};

void sub_08057BDC(void)
{
    struct Unk085D6A48Row *rows;
    struct Unk8553A18 *p;
    int i;
    int idx;
    int c;

    if (gUnknown_03004508 <= 8)
    {
        c = gUnknown_08553B04[gUnknown_03004508];
        for (i = 0; i <= 1; i++)
        {
            rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;
            idx = rows[gUnknown_03004580[i][1]].unk02 * 2 + i;
            p = &gUnknown_08553A18[idx];
            if (i != 0)
                sub_08071900(gUnknown_08499578 + i * 0x100,
                             gUnknown_08551A04 + ((p->unk02 << 5) + (p->unk00 + (14 - c))),
                             c, 6);
            else
                sub_08071900(gUnknown_08499578 + (14 - c),
                             gUnknown_08551A04 + ((p->unk02 << 5) + p->unk00),
                             c, 6);
        }
    }

    rows = (struct Unk085D6A48Row *)gUnknown_085D6A48;

    for (i = 0; i <= 1; i++)
    {
        if (gUnknown_030005E8[i] != 0
         && gUnknown_02029B78[i] != gUnknown_02029B7C[i])
        {
            gUnknown_030005D8[i] -= gUnknown_030005E0[i];
            gUnknown_02029B78[i] = (u32)gUnknown_030005D8[i] >> 16;
            gUnknown_08551A04[0] += 0;
            gUnknown_08551A04 += 0;
            rows += 0;
            gUnknown_03004582[i][0] += 0;
            c = c;
            idx = rows[gUnknown_03004582[i][0]].unk02 * 2 + i;
            p = &gUnknown_08553A18[idx];
            sub_0805772C(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);
            sub_080577E4(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);
        }
    }
}







