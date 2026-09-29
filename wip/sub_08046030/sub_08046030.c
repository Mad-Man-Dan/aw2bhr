#include "global.h"

/* sub_08046030 @ 0x08046030, 1556 bytes, THUMB.
 *
 * The results screen's setup: census the terrain, then draw one row per army.
 *
 * The map is reached through agbcc's own -fforce-addr word at 0x0812A104
 * (the ROM word there is 0x08499590), so the honest spelling of the symbol is
 * what produces the double load. Per the wave-34 rule the plane and the row
 * table must be COMPONENT_REFs of a struct laid over the `u8 *` base, not
 * arithmetic on the pointer -- the ROM computes `(map + 0x1432) + idx` and
 * only a member reference preserves that association.
 *
 * NOT MATCHED. size -8, first difference at +0x188, and the residual is
 * EXACTLY four instructions in one place -- everything from the prologue
 * through +0x188 is byte-identical, and every later difference is a knock-on
 * of the shift plus two swapped spill slots.
 *
 * The residual: after the `a` chain at _0804627E/_08046284 the ROM RE-CREATES
 * all three of the values the five `sub_08014A5C` calls share --
 * `ldr r6,=gUnknown_08499578`, `movs r5,#128; lsls r5,#8` (0x8000) and
 * `movs r4,#0` -- while this candidate keeps the block-A pseudos alive in
 * r6/r7/r5 across the two `if`s and reuses them. So the ROM has TWO pseudos
 * per value where this has one; the ROM's block A uses r4/r6/r5 and its
 * block B r6/r5/r4, which is what proves them distinct rather than merely
 * differently allocated. It is not register pressure: r7 is free in the ROM
 * across that whole region and it declines to use it.
 *
 * Ruled out: `a = 0x17; if (x <= 9) a = 0x18;` -- that emits `movs #23`
 * BEFORE the `ldrh`, and `a = x <= 9 ? 0x18 : 0x17;` is what puts the load
 * first (do_store_flag presets the false arm), which is the spelling used
 * here and it fixed 4 bytes of the diff. Also ruled out by isolated probe:
 * binding the compare operand to a local (`t = gUnknown_03004080;`) restores
 * the load-first order but costs an `adds r0,r1,#0` the ROM does not have,
 * and neither spelling reproduces the three-value split in isolation -- both
 * probe variants keep one pseudo each. The CFG is identical in both (one
 * label per `if`, no extra predecessor), so the split is not a block-boundary
 * effect either.
 *
 * The second symptom, and probably the same root cause: the two spilled
 * temporaries in the k-loop are swapped. The ROM puts `k * 0x3c` at sp+0x10
 * and the `unk14` value at sp+0x14; this candidate has them the other way
 * round, even though `k * 0x3c` is created first in source order.
 */
struct Unk46030Map
{
    /* 0x0000 */ u16 unk00;
    /* 0x0002 */ u16 unk02;
    /* 0x0004 */ u8 filler_0004[0x142e];
    /* 0x1432 */ u8 plane[0x2d48];
    /* 0x417a */ u16 rowOffset[1];
};

#define MAP ((struct Unk46030Map *)gUnknown_08499590)

/* Draws one icon row of the tally page with the shared text buffer.
 *
 * gBG0TilemapBuffer is the same variable as gUnknown_08499578 (the linker
 * script defines it as an alias). Naming it by the second name here keeps the
 * compiler from reusing the earlier address load for these calls, as the ROM
 * does not. */
static inline void DrawIcon(int x, int y, int id)
{
    sub_08014A5C(x, y, gBG0TilemapBuffer, id, 0x8000, 0);
}

void sub_08046030(void)
{
    s16 i;
    s16 j;
    u16 k;
    u16 y;
    u16 v;
    int a;

    sub_08071948(gUnknown_08499580, 0, 1, gUnknown_08125530, 0x8360);
    sub_0801F114();
    sub_0801F150(1, (void *)0x06010000, 0x1d5, 0x14);
    sub_0801F234(0x3e);
    sub_0801F234(0x3f);
    sub_0801F234(0x40);
    sub_0801F234(0x41);
    sub_0801F234(0x42);
    sub_0801F150(0, (void *)0x06010000, 0x1e9, 0x15);
    sub_0801F234(2);
    sub_0801F150(2, (void *)0x06010000, 0x20f, 0x11);
    sub_0801F234(0xa9);
    sub_0801B780(0);
    ApplyPaletteExt(gUnknown_0809165C, 0x260, 0x20);
    sub_08011C68(gUnknown_0812B49C, (void *)0x060146a0, 0x180);
    sub_08011C68(gUnknown_0812B61C, (void *)0x06014820, 0xe0);
    sub_08011C68(gUnknown_0812B6FC, (void *)0x06014900, 0x120);

    for (i = 0; i <= 4; i++)
        gUnknown_084C1430->unk28[i] = 0;

    for (i = 0; i < MAP->unk00; i++)
    {
        for (j = 0; j < MAP->unk02; j++)
        {
            switch (MAP->plane[MAP->rowOffset[j] + i] & 0x1f)
            {
            case 6:
            case 8:
            case 10:
            case 11:
            case 14:
                gUnknown_084C1430->unk28[MAP->plane[MAP->rowOffset[j] + i] >> 5]++;
                break;
            }
        }
    }

    gUnknown_084C1430->unk24 = 0;
    for (i = 0; i <= 4; i++)
        gUnknown_084C1430->unk24 += gUnknown_084C1430->unk28[i];

    sub_080149C0(3, 2, gUnknown_08499578, sub_080248E4(), 0x8000, 0);
    sub_08014B0C(0x1c, 2, gUnknown_08499578, gUnknown_03004080, 0x8000, 0);

    a = gUnknown_03004080 <= 9 ? 0x18 : 0x17;
    if (gUnknown_03004080 <= 0x63)
        a++;
    DrawIcon(a, 2, 0x967);
    DrawIcon(2, 6, 0x95d);
    DrawIcon(8, 6, 0x95e);
    DrawIcon(0xf, 6, 0x968);
    DrawIcon(0x16, 6, 0x96e);

    y = 8;
    for (k = 1; k <= sub_080248F8(); k++)
    {
        sub_08014B0C(6, (s16)y, gUnknown_08499578, gPlayers[k].unitCount,
                     0x8000, 0);
        sub_08014B0C(0xa, (s16)y, gUnknown_08499578, gPlayers[k].unitsLost,
                     0x8000, 0);
        v = gPlayers[k].defeated;
        if (v != 0)
        {
            sub_08014A5C(1, (s16)(k * 2 + 6), gUnknown_08499578, 0x95e, 0x8000,
                         0);
            gUnknown_084C1430->unk3c[k] = 0xff;
            sub_080149C0(0xd, (s16)y, gUnknown_08499578, gUnknown_084C3F40,
                         0x8000, 0);
            sub_080149C0(0x14, (s16)(k * 2 + 6), gUnknown_08499578,
                         gUnknown_084C3F40, 0x8000, 0);
            sub_080149C0(0x1c, (s16)(k * 2 + 6), gUnknown_08499578,
                         gUnknown_084C3F40, 0x8000, 0);
        }
        else if (gPlayers[k].aiControlled == 0)
        {
            sub_080149C0(3, (s16)(k * 2 + 6), gUnknown_08499578,
                         gUnknown_084C3F40, 0x8000, 0);
            gUnknown_084C1430->unk3c[k] = 0xff;
            sub_080149C0(0xd, (s16)y, gUnknown_08499578, gUnknown_084C3F40,
                         0x8000, 0);
            sub_080149C0(0x14, (s16)(k * 2 + 6), gUnknown_08499578,
                         gUnknown_084C3F40, 0x8000, 0);
            sub_080149C0(0x1c, (s16)(k * 2 + 6), gUnknown_08499578,
                         gUnknown_084C3F40, 0x8000, 0);
        }
        else
        {
            gUnknown_084C1430->unk3c[k] = gUnknown_084C1430->unk28[k] * 100
                                        / gUnknown_084C1430->unk24;
            if (gPlaySt.fog == 0
             || gPlayers[k].team
                    == gPlayers[gUnknown_030033EC].team)
            {
                sub_08014B0C(0xd, (s16)y, gUnknown_08499578,
                             gPlayers[k].captures, 0x8000, v);
                sub_08014B0C(0x14, (s16)(k * 2 + 6), gUnknown_08499578,
                             gPlayers[k].income, 0x8000, v);
                sub_08014B0C(0x1c, (s16)(k * 2 + 6), gUnknown_08499578,
                             gPlayers[k].funds, 0x8000, v);
            }
            else
            {
                if ((gPlaySt.captureLimit | gPlaySt.turnLimit) != 0)
                {
                    sub_08014B0C(0xd, (s16)y, gUnknown_08499578,
                                 gPlayers[k].captures, 0x8000, v);
                    sub_08014B0C(0x14, (s16)(k * 2 + 6), gUnknown_08499578,
                                 gPlayers[k].income, 0x8000, v);
                }
                else
                {
                    sub_080149C0(0xd, (s16)y, gUnknown_08499578,
                                 gUnknown_084C3F4C, 0x8000, v);
                    sub_080149C0(0x14, (s16)(k * 2 + 6), gUnknown_08499578,
                                 gUnknown_084C3F4C, 0x8000, v);
                }
                sub_080149C0(0x1c, (s16)(k * 2 + 6), gUnknown_08499578,
                             gUnknown_084C3F4C, 0x8000, 0);
            }
        }
        y += 2;
    }

    if (gPlaySt.fog == 0
     || (gPlaySt.captureLimit | gPlaySt.turnLimit) != 0)
        sub_08014B0C(0xd, 0x10, gUnknown_08499578, gPlayers->captures,
                     0x8000, 0);
    else
        sub_080149C0(0xd, 0x10, gUnknown_08499578, gUnknown_084C3F4C, 0x8000,
                     0);

    sub_08014A5C(3, 0x10, gUnknown_08499578, 0x963, 0x8000, 0);
    gUnknown_084C1430->unk3c[0] = gUnknown_084C1430->unk28[0] * 100
                                / gUnknown_084C1430->unk24;
}


