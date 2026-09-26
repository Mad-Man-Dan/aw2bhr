#include "global.h"

/* PARKED, wave 49 (W49-J), 668 bytes against 672 (-4), 40.2% positional.
 *
 * Everything structural reads correct: the three-way table select, the seven
 * argument slots of sub_0801C7DC, both sub_0801F34C loops, the gUnknown_02028DD4
 * switch's two arms and their duplicated inner loop, and the
 * gUnknown_085D3DD0 cost-row chain shared with sub_08046778.
 *
 * KNOWN RESIDUALS, in order of size:
 *  1. The gUnknown_02028DD4 switch's DISPATCH. The ROM emits gcc's balanced-tree
 *     form -- `cmp #1; beq case1; cmp #1; bgt node2; b default; cmp #2; bne
 *     default; b case2` -- and this draft's `switch` collapses to `cmp #1; beq;
 *     cmp #2; bne; b`. Same source construct, leaner output; the extra
 *     low-bound test is what the missing 4 bytes are.
 *  2. Operand order in `gUnknown_084C2112[i*2] + 0x50 + a`: the ROM computes
 *     (table + 0x50) + a, agbcc's fold moves the constant onto `a` and emits
 *     (a + 0x50) + table. Same instruction count, different bytes. Written
 *     left-associated already, so parenthesising will not fix it -- this needs a
 *     different subexpression, not different brackets.
 *  3. Pool words `gUnknown_085D5ABC + 0x54` vs the ROM's `gUnknown_085D5B10`
 *     and `gUnknown_085D3DD0 + 0x50` vs `gUnknown_085D3E20`. Same addresses;
 *     the disassembly-artefact class, not a real difference.
 *
 * RULED OUT, measured: writing the three-way table select as a `switch (b)`
 * with a bound `u16 *` instead of the nested ternary. It reproduces the ROM's
 * `cmp #6; beq / cmp #8; bne` layout but costs 24 bytes elsewhere (648 vs 672)
 * -- the ternary's merge, which loads once at the join after each arm has done
 * its own `adds #8`, is what the ROM has and the switch loses it.
 */

/* Builds the whole unit-detail window: sprite, frame, the per-terrain icon row,
 * and then one of two variant blocks chosen by gUnknown_02028DD4.
 *
 * The record type is picked by the SECOND parameter and all three tables are
 * the same 20-byte struct Unk085D583C, so the member at +8 that feeds
 * sub_0801C7DC is a member of the record and not a one-table accident.
 *
 * The 0xE000 built as `movs r3,#0xe0; lsls r3,#8` is agbcc materialising a
 * constant too wide for `mov #imm8`; c_08002844.c passes the identical literal
 * to the identical parameter, so it says nothing about the type.
 *
 * Both cases of the gUnknown_02028DD4 switch share their first inner loop
 * verbatim. Writing it once and sharing it is not an option: the ROM emits both
 * copies, each with its own pool words, and case 2 even allocates the row
 * pointer to a different register (r7 rather than sl) because b is dead there.
 */

void sub_08046A84(u8 a, u8 b)
{
    u16 n;
    s16 i;

    n = 0;

    sub_0801BD00(a + 6, 8, gUnknown_084C20A0, 0xE000);

    sub_0801C7DC(b == 6   ? gUnknown_084998A4[gUnknown_02028DD7].unk08
                 : b == 8 ? gUnknown_0849982C[gUnknown_02028DD7].unk08
                          : gUnknown_085D583C[b].unk08,
                 0, 0, a + 6, 8, 0x3247, 0);

    sub_0801F34C(0xA8, a + 0x37, 0x28, 0, 0);

    for (i = 0; i <= 2; i++)
    {
        if (gUnknown_085D5ABC[gUnknown_084C20C0[i]].repairTable[b] != 0)
            sub_0801F34C(i + 0x2C, a + 0x52, 0x38, 0, 0);
    }

    switch (gUnknown_02028DD4)
    {
    case 0:
        break;

    case 1:
        for (i = 0; i < gUnknown_085D583C[b].defense; i++)
            sub_0801F34C(0x39, gUnknown_084C2112[i * 2] + 0x50 + a,
                         gUnknown_084C2112[i * 2 + 1] + 0x19, 0, 0);

        sub_08043418(a + 0x38, 0x98, 0xF);

        for (i = 0; i <= 6; i++)
        {
            if (gUnknown_085D3DD0[gPlaySt.coAbilities
                        ? gPlayers[gUnknown_030033EC].co
                        : 1]
                    .power[gPlayers[gUnknown_030033EC].coMode]
                    .movementChart[gPlaySt.weather]
                        [gUnknown_084C212A[i] * 32 + b] != -1)
            {
                sub_0801F34C(gUnknown_084C20C3[i],
                             gUnknown_084C2131[n * 2] + a + 2,
                             gUnknown_084C2131[n * 2 + 1], 0, 0);
                n++;
            }
        }
        break;

    case 2:
        for (i = 0; i < gUnknown_085D583C[b].defense; i++)
            sub_0801F34C(0x39, gUnknown_084C2112[i * 2] + 0x50 + a,
                         gUnknown_084C2112[i * 2 + 1] + 0x19, 0, 0);

        sub_08043418(a + 0x38, 0x55, 0xE);
        break;
    }
}

