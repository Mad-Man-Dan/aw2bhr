#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018194.
 * sub_08018194 @ 0x08018194
 */

/* The unconditional twin of sub_08018018: the same 6x6 fill of the
 * gUnknown_08499588 tilemap, but with no `col + a <= 5` guard, so the store
 * needs no u16 temp and the `pal << 12` hoists clear of both loops.
 *
 * BOTH counters are u16 here (`lsls #0x10; lsrs #0x10` re-narrowing), where
 * sub_08018018's are u8 (`#0x18`). Read the width off each function's own
 * narrowing shifts -- the two siblings genuinely differ.
 *
 * The `v = 9` preset with a conditional override to 1 is this block's house
 * idiom, shared with sub_0801815C and sub_08018018. */
void sub_08018194(u8 a)
{
    u16 row;
    u16 col;
    int pal;

    pal = 9;
    if (IsAnyWorldMapProcRunning())
        pal = 1;

    for (row = 0; row <= 5; row++)
        for (col = 0; col <= 5; col++)
            gUnknown_08499588[row * 32 + col] = col + 0x20B + row * 6 + (pal << 12);

    sub_0801815C(a);
    sub_080185A0();
}
