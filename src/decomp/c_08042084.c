#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08042084.
 * HasSupplyAbility @ 0x08042084
 */

/* bool8, not int, and the evidence is entirely on the CALLER side: this body is
 * byte-identical under either return type, but UnitMenu_SupplyUsability (wave 35, W35-H)
 * emits `bl HasSupplyAbility; lsls r0,r0,#0x18; cmp r0,#0` -- the narrowing agbcc
 * inserts at every call site of a sub-word-returning callee. Two instructions
 * earlier the same function calls BuildResupplyTargetList and tests it with a bare
 * `cmp r0,#0`, so the shift is not decoration. */
/* Named per Xenesis's AW2 Subroutine List: "Gathers a unit's Supplying
 * ability". The old HasSupplyAbility symbol is kept as a linker alias below so
 * every other unit keeps resolving it unchanged. */
bool8 HasSupplyAbility(u8 *p)
{
    if (gUnknown_084995C1[*p] != 0)
        return TRUE;

    return FALSE;
}

asm(".global sub_08042084\n.thumb_set sub_08042084, HasSupplyAbility\n");
