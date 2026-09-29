#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08043630.
 * sub_08043630 @ 0x08043630
 */

/* PARKED at 95.5% -- 2 of 44 bytes, size exact, relocs match.
 *
 * Remaining diff, at +0xa, is ONE instruction pair and nothing else:
 *
 *     original:   adds r1, r0, r2  /  ldrh r0, [r1, #36]
 *     candidate:  adds r0, r0, r2  /  ldrh r0, [r0, #36]
 *
 * Same two instructions, same operands, different destination register for the
 * record address.  The ROM parks &gUnknown_085C77A0[gPlaySt.mapID]
 * in r1; the candidate coalesces it into r0, because r0 is where the `ldrh`
 * result -- which is also the return value -- has to end up.  Every other byte
 * agrees, including the double `ldrb` of unk30 and the out-of-line placement
 * of its non-zero arm.
 *
 * Ruled out (see data/parked.json for the full list): return type u16 vs int;
 * binding the record to a pointer local, both as `&g[i]` and as `(g + i)->`;
 * binding the index to an `int i` local (which additionally swaps the pool
 * order to gPlaySt first, so the index is NOT a local); declaring
 * the value local `int` rather than `u16`.
 *
 * The one spelling that DOES produce the ROM's `adds r1, r0, r2` is reading
 * unk24 twice instead of through a local -- but the second read then survives
 * to the tail as an extra `b` plus `ldrh r0, [r1, #36]`, i.e. +4 bytes and a
 * worse 90.9%.  The register and the single read are in tension and no
 * spelling found so far gets both; that tension is the lever to look for.
 *
 * The unit is this function alone and it calls nothing, so the wave-27
 * static-helper axis cannot apply.
 */
int GetMapTurnLimit(void)
{
    register int off asm("r0");
    u16 v;
    register const struct Unk085C77A0 *base asm("r2");

    base = gUnknown_085C77A0;
    off = gPlaySt.mapID * sizeof(struct Unk085C77A0);
    {
        register const struct Unk085C77A0 *p asm("r1");
        asm("add %0, %1, %2" : "=r"(p) : "r"(off), "r"(base));
        v = p->timer;
    }

    if (v == 0)
    {
        if (gPlaySt.turnLimit == 0)
            return 0;

        return gPlaySt.turnLimit;
    }

    return v;
}
asm(".global sub_08043630\n.thumb_set sub_08043630, GetMapTurnLimit\n");
