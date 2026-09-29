#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A5C4.
 * sub_0802A5C4 @ 0x0802A5C4
 */

/* The long lsls/adds/rsbs chain is agbcc's exact division of a POINTER
 * DIFFERENCE by the 12-byte stride: multiply by inv(3) = 0xAAAAAAAB, then
 * `asr #2`. `asr #8` is that same division followed by `>> 6`, i.e.
 * `(p - gUnits) >> 6` -- the army number, the 64-entries-per-army
 * grouping recorded on gUnits. Write the pointer arithmetic; never
 * the shifts.
 *
 * gUnits is named the same honest way in both halves. The bit-6
 * store gets a plain inline pool word (one reference) while the bit-7 half
 * gets agbcc's -fforce-addr `.rodata` word -- what the ROM prints as
 * `gUnknown_08090B84` -- because its address is referenced across a
 * control-flow merge. That is the wave-18 reference-count rule, not two
 * different globals, and one honest spelling produces both.
 *
 * `q` must be its own statement, and it is worth exactly one byte. Writing
 * the scan as `gUnits[i].type != 0 && &gUnits[i] != p
 * && ...` is 203 of 204 bytes: the slot address then comes out
 * `adds r1, r0, r2` (index, base) where the ROM has `adds r1, r2, r0`
 * (base, index). Binding the address once puts the base pointer first, which
 * is what expand_expr does for a plain ARRAY_REF that is not already a CSE
 * of three sibling reads. */

void NoteFlaggedUnitRemoved(struct Unit *p)
{
    struct Unit *q;
    int base;
    u16 i;

    if (p->flags & 0x40)
        gUnknown_030030F8 = ((p - gUnits) >> 6) + 1;

    if (p->flags & 0x80)
    {
        base = (p - gUnits) & 0xc0;

        for (i = base + 1; i < base + 0x33; i++)
        {
            q = &gUnits[i];

            if (q->type != 0 && q != p && (q->flags & 0x80))
                return;
        }

        gUnknown_030030F8 = ((p - gUnits) >> 6) + 1;
    }
}
asm(".global sub_0802A5C4\n.thumb_set sub_0802A5C4, NoteFlaggedUnitRemoved\n");
