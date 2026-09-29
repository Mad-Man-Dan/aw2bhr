#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805D2A0.
 * sub_0805D2A0 @ 0x0805D2A0
 */

/* A TWELFTH list builder of the src/decomp/c_0805CA60.c family, sitting past
 * the 0x0805CA60-0x0805D1F0 run the note in include/unknown-globals.h covers.
 * Its .rodata address-constant pair is the ROM's gUnknown_0816DA38 /
 * gUnknown_0816DA3C, which hold 0x030046B0 and 0x030045F0 -- dereferenced
 * against baserom.gba, the same values as every other pair in that block, so
 * they are NOT globals and the DIRECT `gUnknown_030046B0 = gUnknown_030045F0`
 * is what the original source said. It loops, so it gets a pair, exactly as
 * that rule predicts.
 *
 * The three filters are NESTED ifs rather than one `&&` chain, and the ROM's
 * instruction order is what settles it. Written as a chain, fold_truthop
 * merges `!(unk01 & 1)` with `unk00 == 7` into a single `ldrh; and 0x1ff;
 * cmp #7` on the halfword at +0 -- two adjacent fields compared against
 * constants in one container. The ROM keeps them apart and re-uses the r3 it
 * already loaded for the `!= 0` test, which only the nested form produces.
 * c_0805CA60.c does not need this because its third filter reads a different
 * object. Binding unk00 to a local `u8` also splits them, but costs a hoisted
 * `movs #1` and a third high register -- 4 bytes and the wrong allocation. */

void AiStartSupplyPass(void)
{
    int i;

    gUnknown_030046B0 = gUnknown_030045F0;

    for (i = gUnknown_03003F2C + 1; i < gUnknown_03003F2C + 0x40; i++)
    {
        if (gUnits[i].type != 0)
        {
            if (!(gUnits[i].flags & 1))
            {
                if (gUnits[i].type == 7)
                {
                    *gUnknown_030046B0++ = i;
                }
            }
        }
    }

    *gUnknown_030046B0 = 0;
    gUnknown_030046B0 = gUnknown_030045F0;
    gUnknown_03004778 = AiDeliberateSupply;
    gUnknown_03004780 = 2;
}
asm(".global sub_0805D2A0\n.thumb_set sub_0805D2A0, AiStartSupplyPass\n");
