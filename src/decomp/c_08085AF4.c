#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08085AF4.
 * sub_08085AF4 @ 0x08085AF4
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

void ResetMapSelectState(void)
{
    u8 *base;
    u8 *p;
    u8 v;
    int z;

    /* The house reverse-fill idiom -- see docs/agbcc-codegen.md. The three
     * statements have to stay in this order: it is what puts the `movs #0`
     * between the base `ldr` and the cursor computation. */
    base = gUnknown_03005990;
    v = 0;
    p = base + 8;

    do
    {
        *p = v;
        p--;
    } while ((int)p >= (int)base);

    /* `z` is load-bearing: the ROM's second `movs r2, #0` is shared by all
     * three of these stores, and the intervening `= 2` clobbers whichever
     * register a re-materialised literal would have landed in. Spelling the
     * zeroes as literals costs two extra `mov`s. */
    z = 0;
    gUnknown_03005900 = z;
    gUnknown_0300596C = 2;
    gUnknown_03005980 = z;
    gUnknown_03005930 = z;
}

asm(".global sub_08085AF4\n.thumb_set sub_08085AF4, ResetMapSelectState\n");
