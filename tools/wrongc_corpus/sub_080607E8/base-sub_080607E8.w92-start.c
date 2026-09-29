#include "global.h"

/* sub_080607E8 @ 0x080607E8, 172 bytes.  NOT MATCHED -- see the residual note
 * at the bottom of this comment.
 *
 * Places up to three units for scenario slot 7: read the slot's (x, y) out of
 * sub_0803E354, then for each of three columns check that the map cell is empty
 * and that the per-terrain-band table gFactoryUnitSchedule names a unit type for
 * that column, and if so spawn it and seed its three state bytes.
 *
 * Settled, and each of these reproduces exactly:
 *
 *  - the map is reached through a FILE-LOCAL struct cast onto the `u8 *`
 *    gUnknown_08499590, per the wave-34 rule in include/unknown-globals.h. The
 *    ROM's `adds r0, r1, r3(0x417A); adds r0, r0, r2` and `adds r1, #0x12;
 *    adds r1, r1, r0` are the COMPONENT_REF association, which raw byte
 *    arithmetic on the pointer does not produce -- it reassociates the constant
 *    outward and folds 0x12 into the ldrb displacement.
 *
 *  - `lsls #0x10; asrs #0x10` on both `a + i` and `b` is the s16 conversion the
 *    sub_08025CC8 prototype forces, not a source cast; `d` needs none because
 *    it is u8. `subs r0, r5, #1; lsls #0x18; lsrs #0x18` likewise comes from
 *    sub_08061178's u8 parameter.
 *
 *  - `b` is the slot's second byte PLUS 4, computed once and kept in r8.
 *
 * RESIDUAL, two spots, both in the gFactoryUnitSchedule lookup and both one fact:
 * the ROM recomputes `band * 3` INSIDE the loop (`mov r3, sb; lsls r0, r3, #1;
 * add r0, sb`) with `band` itself living in sb, while agbcc hoists `band * 3`
 * to the preheader and parks it in sb, leaving `band` in the low r4. That
 * costs the `movs r1, #0x1f; mov sb, r1; mov r2, sb; ands r2, r0; mov sb, r2`
 * shuffle the ROM has for the mask, and it frees r4 early so `u` reuses it.
 * The knock-on is the second spot: with `band` dead by then, agbcc has the
 * known-zero `c` in r6 available and substitutes it for the literal 0 of
 * `unk09 = 0`, where the ROM materialises `movs r0, #0`.
 *
 * RULED OUT by compile_probe, all six reassociating to the identical tree and
 * all six hoisting: `[band * 3 + i]`, `[i + band * 3]`, `[band * 2 + band + i]`,
 * `[(band << 1) + band + i]`, `*(base + band * 3 + i)`, and
 * `((u8 (*)[3])base)[band][i]`. Also ruled out: `u8`/`int` for band, c and d,
 * the `continue` form against the nested-if form, `0x1f & g` against
 * `g & 0x1f`, and merging the two guards into one `&&`.
 *
 * WHAT DOES MOVE IT, and is the lead to follow: casting the table to a
 * 3-byte-row STRUCT (`((struct Row3 *)base)[band].v[i]`) stops the hoist dead
 * and reproduces the ROM's sb allocation and its mask shuffle EXACTLY, first
 * try -- but agbcc pads `struct { u8 v[3]; }` to 4, so the stride comes out
 * `lsls #2` and the association becomes `(base + band * 4) + i` instead of the
 * ROM's `base + (band * 3 + i)`. The discriminator this exposes is that agbcc
 * hoists a TWO-instruction invariant (the *3 synthesis) but not a
 * ONE-instruction one (the *4 shift). Anything that keeps the 3-byte stride
 * while making the multiply part of an addressing tree rather than a standalone
 * MULT_EXPR in the index should close it. */

struct Map7E8
{
    /* 0x0000 */ u8 filler_00[0x12];
    /* 0x0012 */ u8 plane[0x4168];
    /* 0x417A */ u16 rows[1];
};

void sub_080607E8(void)
{
    int band;
    struct Unk02028360 *p;
    int a;
    int b;
    int i;
    int v;
    u8 c;
    u8 d;
    struct Unit *u;

    band = gUnknown_03004080 & 0x1f;
    p = sub_0803E354(7);

    if (p == 0)
        return;

    a = p->unk00;
    b = p->unk01 + 4;

    i = 0;
_loop:
    {
        v = ((struct Map7E8 *)gUnknown_08499590)->rows[b] + (a + i);
        c = ((struct Map7E8 *)gUnknown_08499590)->plane[v];

        if (c == 0)
        {
            d = ((u8 *)gFactoryUnitSchedule)[band * 3 + i];

            if (d != 0)
            {
                u = sub_08025CC8(a + i, b, d);
                u->unk09 = 0;
                u->unk0a = c;
                u->unk0b = sub_08061178(d - 1);
            }
        }
    }

    i++;

    if (i <= 2)
        goto _loop;
}
