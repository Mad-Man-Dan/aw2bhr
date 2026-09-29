#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057BDC.
 * sub_08057BDC @ 0x08057BDC
 */

/*
 * StepBattleHud -- per-frame update of the two-side display.
 *
 * For the first eight frames (gUnknown_03004508 counts them) it slides a
 * widening slice of tiles from gUnknown_08551A04 into the two tilemap buffers
 * at gUnknown_08499578, one side per pass, the slice width coming from
 * gUnknown_08553B04.
 *
 * After that, each side whose shown value gUnknown_02029B78 has not reached
 * its target gUnknown_02029B7C has its fixed-point accumulator
 * gUnknown_030005D8 stepped by gUnknown_030005E0, the top 16 bits become the
 * new shown value, and the side is redrawn by DrawHpGaugeStrip and DrawHpNumber.
 *
 * The two row-table lookups use different spellings on purpose and both are
 * right: the first loop reads gUnknown_03004580[i][1] and the second reads
 * gUnknown_03004582[i][0], which is the same memory two bytes in.
 *
 * Why the C looks odd:
 *   - `p->unk00 + (14 - c)` must keep its brackets; without them the
 *     subtraction is folded the other way and costs three instructions.
 *   - The `(u32)` cast before `>> 16` gives an unsigned shift. The
 *     accumulator is signed, so the bare shift would be arithmetic.
 *   - The second loop's row table (`rows2`) and selector table (`sel`) are
 *     bound after `i = 0` so the compiler keeps both addresses in registers
 *     for the whole loop. `rows2` is a second variable, not the `rows` of the
 *     first loop, and the loop test is written plainly.
 */

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

void StepBattleHud(void)
{
    struct Unk085D6A48Row *rows;
    struct Unk085D6A48Row *rows2;
    u16 (*sel)[8];
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
            {
                sub_08071900(gUnknown_08499578 + i * 0x100,
                             gUnknown_08551A04 + ((p->unk02 << 5) + (p->unk00 + (14 - c))),
                             c, 6);
            }
            else
            {
                sub_08071900(gUnknown_08499578 + (14 - c),
                             gUnknown_08551A04 + ((p->unk02 << 5) + p->unk00),
                             c, 6);
            }
        }
    }

    i = 0;
    rows2 = (struct Unk085D6A48Row *)gUnknown_085D6A48;
    sel = gUnknown_03004582;
    for (; i <= 1; i++)
    {
        if (gUnknown_030005E8[i] != 0 && gUnknown_02029B78[i] != gUnknown_02029B7C[i])
        {
            gUnknown_030005D8[i] -= gUnknown_030005E0[i];
            gUnknown_02029B78[i] = (u32)gUnknown_030005D8[i] >> 16;
            idx = rows2[sel[i][0]].unk02 * 2 + i;
            p = &gUnknown_08553A18[idx];
            DrawHpGaugeStrip(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);
            do { } while (0);
            DrawHpNumber(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);
        }
    }
}
asm(".global sub_08057BDC\n.thumb_set sub_08057BDC, StepBattleHud\n");
