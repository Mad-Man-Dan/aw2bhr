#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080536D8.
 * sub_080536D8 @ 0x080536D8
 */

/* Wave 50, W50-F. THREE parameters -- see the note on the declaration in
 * include/unknown-functions.h. The middle one is never read.
 *
 * Two spellings are load-bearing, and both are about where a constant offset
 * gets folded:
 *
 *  - The slot index MUST be spelled `gUnknown_02029808[a1].unk02[...]`, not
 *    through the gUnknown_0202980A overlay declared for the same address. With
 *    the overlay there is only ONE reference to the gUnknown_02029808 address
 *    constant in the loop, so agbcc folds `+0x30` into a second pool word
 *    (`gUnknown_02029808+0x30`) and the base pseudo dies in the preheader. With
 *    the member spelling there are two, the base becomes a loop invariant that
 *    LICM keeps and reload spills at [sp], and the unk30 access becomes
 *    `ldr r1,[sp]; adds r1,#0x30` -- which is what the ROM has, together with
 *    the fourth stack slot and the `adds r6,r1,#0` copy that keeps the base
 *    alive past r6.
 *
 *  - `row` must be a local. Written out as
 *    `gUnknown_085D6A48[gUnknown_03004580[a1][1]][2]` the +4 folds into the base
 *    (`adds r2,#4; ldrh r0,[r0]`); bound to a pointer first it stays a load
 *    displacement, `ldrh r2,[r0,#4]`, as in the ROM.
 *
 * The `= 1` store into gUnknown_02029B80 reuses the register the `== 1` test
 * loaded, which is why the ROM has no `movs #1` there.
 */
void RunDeathBatch(u16 a1, u16 a2, u16 count)
{
    void (*fn)(u16, u16, int);
    u16 i;
    u16 idx;

    i = 0;
    gUnknown_03004548[a1] = 1;

    while (count != 0)
    {
        fn = gUnknown_085535C0[gUnknown_02029808[a1].unk30[gUnknown_02029808[a1].unk2e]];
        idx = gUnknown_02029808[a1].unk02[gUnknown_02029808[a1].unk00 + i];

        gUnknown_03004580[a1][5] -= gUnknown_02029A10[a1].entries[idx].unk00
                                  - gUnknown_02029A10[a1].entries[idx].unk01;
        gUnknown_02029A10[a1].entries[idx].unk00 =
            gUnknown_02029A10[a1].entries[idx].unk01;

        gUnknown_0300454C[0] = idx;
        gUnknown_0300454C[1] = i;

        fn(a1, idx, gUnknown_02029808[a1].unk2e);

        if (idx == gUnknown_08552148[a1])
        {
            u16 *row;

            row = gUnknown_085D6A48[gUnknown_03004580[a1][1]];
            if (row[2] == 1 && gUnknown_03004580[a1][6] == 0)
                gUnknown_02029B80[a1][idx] = 1;
        }

        i++;
        count--;
        gUnknown_02029808[a1].unk2e++;
    }

    gUnknown_02029808[a1].unk00++;
}
asm(".global sub_080536D8\n.thumb_set sub_080536D8, RunDeathBatch\n");
