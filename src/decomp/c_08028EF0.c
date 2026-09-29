#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028EF0.
 * sub_08028EF0 @ 0x08028EF0
 */

/* MATCHED in wave 79 (W79-A). PROMOTION MUST CARRY "rodata": ["0x08090B58"]
 * -- the pool word holds 0x020237B0 and trymatch reports it as "different
 * symbols that resolve to the same address", which is the wave-18 pool
 * placement working as designed, not a difference.
 *
 * Parked across waves 38/42/52/58 at 92.6% (the 96.6% in those entries was
 * best.c's score, not the draft's). The recorded residual was exact: the ROM's
 * second ABS block compares and negates the ORIGINAL (`adds r2,r0,#0;
 * cmp r0,#0; bge; rsbs r2,r0,#0`) where the candidate compared and negated the
 * COPY, giving abs_b five references against the ROM's three, so global_alloc
 * ranked abs_b above `a` and the two swapped hard registers. ~40,000 permuter
 * iterations from two different starting points and six spellings of ABS could
 * not reach it, because the permuter is a register-level tool and the cause is
 * upstream of allocation.
 *
 * The whole fix is `register int t asm("r0");` on the ternary result. Pinning
 * the result forces the ternary's target into the ROM's register, and the ABS
 * expansions fall out in the ROM's form -- original-compared second block
 * included. (Pinning the two ABS results to r4/r2 instead is a second match of
 * the same shape; this one-line form is the one kept.)
 *
 * STILL VALID from the parked evidence:
 *   - +0x26 and +0x28 are s16 (the `movs rI,#0x26; ldrsh` object tell); the
 *     rest of the struct is u16.
 *   - The loop condition is a SIGNED int compare of the u16 difference.
 *
 * WHAT THE FUNCTION IS: snapshots the map scroll origin into +0x1e/+0x20,
 * stores the two axis deltas from the target at +0x22/+0x24 into +0x26/+0x28,
 * then writes a 1,2,3,...,8,8,8 acceleration ramp into gUnknown_020237B0 whose
 * terms sum to the larger delta, with the remainder as a final short step.
 * p->unk2a ends holding the number of steps written. */
#include "global.h"
#include "map.h"
struct Unk08028EF0
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1E */ u16 unk1e;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u16 unk2a;
    /* 0x2C */ u16 unk2c;
};

void CameraScroll_Init(struct Unk08028EF0 *p)
{
    register int t asm("r0");
    u16 d;
    u16 step;

    p->unk1e = gMap->scrollX;
    p->unk20 = gMap->scrollY;
    p->unk26 = p->unk22 - gMap->scrollX;
    p->unk28 = p->unk24 - gMap->scrollY;

    t = ABS(p->unk26) > ABS(p->unk28) ? p->unk26 : p->unk28;
    d = ABS(t);

    step = 1;
    p->unk2a = 0;

    while (d - step >= 0)
    {
        d -= step;
        gUnknown_020237B0[p->unk2a] = step;

        if (step <= 7)
            step++;

        p->unk2a++;
    }

    gUnknown_020237B0[p->unk2a] = d;
    p->unk2c = 0;
}
asm(".global sub_08028EF0\n.thumb_set sub_08028EF0, CameraScroll_Init\n");
