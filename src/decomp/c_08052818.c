#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08052818.
 * sub_08052818 @ 0x08052818
 */

/* Wave 50, W50-F.
 *
 * Two pool words, both written HONESTLY: gUnknown_08136110 holds 0x03004580 and
 * gUnknown_08136114 holds 0x02029A10, so the source names gUnknown_03004580 and
 * gUnknown_02029A10 directly and agbcc parks the addresses itself. Note it does
 * so ASYMMETRICALLY -- the first arm reaches gUnknown_02029A10 through a direct
 * `ldr =sym` and the second through its own .rodata word -- which reproduces
 * without any spelling difference between the two arms.
 *
 * The entry address must NOT be bound to a local pointer. With
 * `e = &gUnknown_02029A10[a1].entries[a2]` the address associates as
 * `(a2*0x24 + base) + a1*0xb4` and costs a `mov` copy at each of the three
 * sites; written out in full at every use, CSE associates it the ROM's way,
 * `(a2*0x24 + a1*0xb4) + base`, and the constant 0xb4's register becomes the
 * multiply's destination so no copy is needed. Six bytes over three sites.
 *
 * The 085D6A48 ROW, by contrast, IS a local: the ROM holds &row[0] across the
 * whole function and reads it at [r3] and [r3,#4].
 */
void DeathHandler_CommonTail(u16 a1, u16 a2)
{
    u16 *row;
    u16 i;

    row = gUnknown_085D6A48[gUnknown_03004580[a1][1]];

    if (row[2] == 0)
    {
        if (gUnknown_02029A10[a1].entries[a2].unk01 == 0)
        {
            if (row[0] <= 1)
            {
                gUnknown_02029A10[a1].entries[a2].unk1c = 1;
                gUnknown_02029A10[a1].entries[a2].unk00 = 0;
            }
            else
            {
                gUnknown_0300454C[0] = a1;
                gUnknown_0300454C[1] = a2;
                gUnknown_02029A10[a1].entries[a2].unk00 = 0;

                if (gUnknown_02029A10[a1].entries[a2].unk18 != -1)
                {
                    if (IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
                    {
                        sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                                     gUnknown_02029BA8[a1].unk14);
                        gUnknown_02029A10[a1].entries[a2].unk18 = -1;
                    }
                }
            }
        }
    }
    else
    {
        if (gUnknown_02029808[a1].unk02[gUnknown_02029808[a1].unk00 + 1] == 0xff)
        {
            for (i = 0; i < 5; i++)
                gUnknown_02029A10[a1].entries[i].unk00 =
                    gUnknown_02029A10[a1].entries[i].unk01;

            gUnknown_03004580[a1][5] = gUnknown_03004580[a1][6];
        }

        if (gUnknown_02029A10[a1].entries[a2].unk01 == 0)
        {
            gUnknown_02029A10[a1].entries[a2].unk00 = 0;

            if (gUnknown_02029A10[a1].entries[a2].unk18 != -1)
            {
                if (IsSlotScriptActiveAt(gUnknown_02029A10[a1].entries[a2].unk18))
                    sub_080156E8(gUnknown_02029A10[a1].entries[a2].unk18,
                                 gUnknown_02029BA8[a1].unk14);
            }
        }
    }
}
asm(".global sub_08052818\n.thumb_set sub_08052818, DeathHandler_CommonTail\n");
