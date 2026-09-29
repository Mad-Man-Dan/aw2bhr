#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019688.
 * sub_08019688 @ 0x08019688
 */

/* Skip `count` records in a byte stream, where a record ends at the first
 * `term` byte found by stepping `stride` at a time, and return the cursor just
 * past the last one. A leaf with two nested loops and no globals.
 *
 * The inner `while` is the rotated zero-trip form: the outer body loads *p and
 * branches straight to the `p++` when it already equals `term`, so the inner
 * block is only reached for a non-terminating byte. The `adds r1,#1` sitting
 * at the TOP of the outer body is that rotation too -- `i++` is scheduled into
 * the slot before the inner loop because r1 is dead across it -- and is not
 * evidence of a pre-increment in the source.
 *
 * `i` is `u8`, not `int`: the ROM re-truncates it (`lsls #0x18; lsrs #0x18`)
 * before every bottom test, which an `int` counter against a u8 bound does
 * not do. */
u8 *SkipTerminatedRecords(u8 *p, u8 term, u8 count, u8 stride)
{
    u8 i;

    for (i = 0; i < count; i++)
    {
        while (*p != term)
            p += stride;
        p++;
    }
    return p;
}
asm(".global sub_08019688\n.thumb_set sub_08019688, SkipTerminatedRecords\n");
