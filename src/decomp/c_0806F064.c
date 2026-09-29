#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F064.
 * sub_0806F064 @ 0x0806F064
 */

/* A "pick the next entry, skipping the one we already have" helper over a
 * little u16 table whose FIRST element is its own length: list[0] is the
 * modulus and list[1..] the entries.
 *
 * The two divisions are different calls -- __umodsi3 then __modsi3 -- and that
 * is the whole type argument. gGameClock % list[0] reaches __umodsi3
 * only if the dividend is unsigned; the declaration in unknown-globals.h is
 * s32 for src/title-screen.c's sake, so the cast carries it here. The second
 * division's dividend is the plain int index, hence the signed helper. */
u16 PickRandomCoFromList(u16 a, u16 *list)
{
    int i;
    u16 r;

    i = (u32)gGameClock % list[0] + 1;
    r = list[i];

    if (r == a)
    {
        i++;
        r = list[i % list[0]];
    }

    return r;
}
asm(".global sub_0806F064\n.thumb_set sub_0806F064, PickRandomCoFromList\n");
