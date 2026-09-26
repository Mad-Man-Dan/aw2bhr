#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014910.
 * sub_08014910 @ 0x08014910, sub_0801496C @ 0x0801496C
 */

/*
 * sub_08014910 -- record slot d's parameters and start it, passing 0 as
 * sub_0801489C's fifth argument.
 *
 * gUnknown_0200BC14 holds three parallel arrays indexed by the slot: unk000 is
 * cleared, unk400 takes a1 and unk408 takes b. What a1, b and c mean is not
 * visible here; sub_0801489C gets all four unchanged.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - The 0 stored in unk000 and the 0 passed as the fifth argument are one
 *     value in the original, which is why this function uses one more
 *     callee-saved register than sub_0801496C below.
 */
u16 sub_08014910(int a1, u16 b, u16 c, u16 d)
{
    gUnknown_0200BC14.unk000[d] = 0;
    gUnknown_0200BC14.unk400[d] = a1;
    gUnknown_0200BC14.unk408[d] = b;

    return sub_0801489C(a1, b, c, d, 0);
}

/* sub_0801496C -- the same as sub_08014910, but sub_0801489C's fifth argument is
 * 1. Being two different constants they cannot share a register, which is why
 * this function is eight bytes shorter. */
u16 sub_0801496C(int a1, u16 b, u16 c, u16 d)
{
    gUnknown_0200BC14.unk000[d] = 0;
    gUnknown_0200BC14.unk400[d] = a1;
    gUnknown_0200BC14.unk408[d] = b;

    return sub_0801489C(a1, b, c, d, 1);
}
