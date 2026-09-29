#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080240B4.
 * sub_080240B4 @ 0x080240B4, sub_0802419C @ 0x0802419C
 */

/* The map header is reached through a struct declared LOCALLY and cast onto the
 * `u8 *` symbol (the wave-34 W34-F rule): the ROM computes every plane address
 * as `(map + K) + idx`, which only a COMPONENT_REF preserves. Writing the cast
 * inline at all four sites rather than binding one `map` local is what makes
 * agbcc re-read gUnknown_08499590 after the `bl __divsi3`, exactly as the ROM
 * does -- a single binding would park the pointer in a callee-saved register
 * across the call instead.
 *
 * gUnknown_08499B0C appears TWICE in the literal pool under two spellings: as
 * `.LC2` (a -fforce-addr .rodata word, the ROM's 0x08090A24 == 0x08499B0C) for
 * the scan, and as a plain inline pool word for the final read. Both come out
 * of the one honest name; see include/unknown-globals.h. */

/* Near-twin of SetPropertyTileForOwner: same scan of the 0xFFFF-terminated
 * gUnknown_08499B0C list against the map's +0x0A22 tile halfword, same
 * `i / 5 * 5`, same `(s16)n` return. Three differences -- it does not write
 * gUnknown_030033F8, it guards the store with `n <= 4`, and its table index
 * carries the +0xa base of the second table packed behind the terminator.
 *
 * THE `(u8)` CAST ON THE INDEX IS LOad-BEARING and is the whole difference
 * between 79.4% and a match. The ROM computes `(a3 >> 5) + 0xa` FIRST and adds
 * n to it (`lsr; add #0xa; add r0,r3,r0`). Written as plain arithmetic that
 * grouping cannot survive fold: `n + (a3 >> 5) + 0xa` associates to
 * `(n + (a3 >> 5)) + 0xa`, and `n + ((a3 >> 5) + 0xa)` is rewritten by fold's
 * `A + (B + C)` -> `(A + C) + B` rule into `(n + 0xa) + (a3 >> 5)`. Both are
 * the right value and the wrong instruction order. A cast to a narrower type
 * interposes a NOP_EXPR that fold will not associate through, so the grouping
 * is preserved -- and it is value-safe here, since `(a3 >> 5) + 0xa` is
 * 0xa..0x11 for any u8 a3. Binding the sub-expression to an `int` local
 * instead (probed) preserves the grouping too but schedules the shift up above
 * the map address computation, which is a different miss. */

s16 SetPropertyTileForOwner(s16 a1, s16 a2, u8 a3)
{
    int i;
    int n;

    gUnknown_030033F8 = gMap->tile[
        gMap->rowOffset[a2] + a1];

    for (i = 0; gUnknown_08499B0C[i] != 0xffff; i++)
    {
        if (gUnknown_08499B0C[i] == gMap->tile[
                gMap->rowOffset[a2] + a1])
            break;
    }

    n = i / 5 * 5;
    gMap->tile[
        gMap->rowOffset[a2] + a1] =
            gUnknown_08499B0C[n + (a3 >> 5)];
    return n;
}
asm(".global sub_080240B4\n.thumb_set sub_080240B4, SetPropertyTileForOwner\n");

s16 sub_0802419C(s16 a1, s16 a2, u8 a3)
{
    int i;
    int n;

    for (i = 0; gUnknown_08499B0C[i] != 0xffff; i++)
    {
        if (gUnknown_08499B0C[i] == gMap->tile[
                gMap->rowOffset[a2] + a1])
            break;
    }

    n = i / 5 * 5;
    if (n <= 4)
        gMap->tile[
            gMap->rowOffset[a2] + a1] =
                gUnknown_08499B0C[n + (u8)((a3 >> 5) + 0xa)];
    return n;
}
