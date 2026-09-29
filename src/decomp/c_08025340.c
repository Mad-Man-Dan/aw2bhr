#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08025340.
 * sub_08025340 @ 0x08025340, sub_08025378 @ 0x08025378
 */

/* gUnits is a POINTER to the 12-byte records, not an array, and the
 * `lsls #1; adds; lsls #2` index is the x*3<<2 = x*12 multiply
 * docs/agbcc-codegen.md describes -- write it as a struct subscript.
 *
 * Binding the element to a local pointer is load-bearing: written inline as
 * `gUnits[a1].flags |= 2` agbcc dereferences the pool word FIRST
 * (`ldr r0,[r0]`) and then computes the index, which is the reverse of the ROM.
 * With the local the index is computed while the pool word is still
 * undereferenced in r1, exactly as the ROM has it -- the same ordering effect
 * as the `p[X + C]` rule.
 *
 * `|= 2` and not a bitfield: the sibling sub_08025378 clears the same bit with
 * a BARE `movs r0,#0xfd`, and a 1-bit field's clear path emits the
 * negate-and-subtract chain instead. */

void sub_08025340(u8 a1)
{
    struct Unit *p = &gUnits[a1];

    p->flags |= 2;

    if (gPlaySt.fog != 0)
        StampUnitVision(a1, -1);
}

/* The clearing half of sub_08025340 -- see there for the pointer-local
 * ordering. `movs r0,#0xfd` is a BARE mask, which is what proves unk01 is a
 * plain byte and not a bitfield container: a 1-bit field would be cleared
 * through `movs r0,#2; rsbs r0,r0,#0; ands`. */

void sub_08025378(u8 a1)
{
    struct Unit *p = &gUnits[a1];

    p->flags &= ~2;

    if (gPlaySt.fog != 0)
        StampUnitVision(a1, 1);
}
