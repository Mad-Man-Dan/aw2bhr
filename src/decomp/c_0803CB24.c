#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803CB24.
 * sub_0803CB24 @ 0x0803CB24
 */

/* Return re-typed from `int` to `u8` in wave 44 (W44-C) -- byte-identical here
 * (the `ldrb`-derived operand already has nonzero_bits <= 0xff, so the
 * narrowing is a no-op), but HaveAllMapsBeenPlayed tests the result with a bare
 * `lsls r0, r0, #0x18` at both call sites, which an `int` return cannot
 * produce. See include/unknown-functions.h next to IsCoSelectable. */
u8 HasMapBeenPlayed(u32 id)
{
    struct Unk02028030 *s = &gUnknown_02028030;
    u32 idx = id >> 3;
    u8 *b = s->unk30;
    u8 *p = b + idx;

    return (1 << (id & 7)) & *p;
}
asm(".global sub_0803CB24\n.thumb_set sub_0803CB24, HasMapBeenPlayed\n");
