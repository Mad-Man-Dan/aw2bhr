#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08053520.
 * sub_08053520 @ 0x08053520
 */

/* MATCHED in wave 86 (W86-C) -- parked at 84.9% since wave 36, re-measured in
 * waves 51, 59 and 77. THE RESIDUAL WAS A WRONG GLOBAL TYPE, NOT AN EXPRESSION
 * SHAPE AND NOT A COMPILER FLAG.
 *
 * The one remaining instruction was where the constant 4 of the second half's
 * table access goes:
 *   ROM   ldr r1,=sym / ... / adds r1,#4 / adds r0,r0,r1 / movs r1,#0 / ldrsh
 *   old   ldr r2,=sym / ... / adds r0,r0,r2 / movs r1,#4 / ldrsh
 *
 * gUnknown_085643A8 was modelled as `struct { const s16 unk00[2]; const s16
 * unk04[2]; }`. It is an ARRAY: `const s16 [][2]`. Controlled probe, identical
 * address arithmetic (constant 4 plus a variable m*2), the two base kinds:
 *   ARRAY  gUnknown_085643B0[0][1][m] -> ldr r2,=sym / adds r2,#4 /
 *                                       adds r0,r0,r2 / movs r1,#0 / ldrsh
 *   STRUCT gUnknown_085643A8.unk04[m] -> ldr r3,=sym / adds r0,r0,r3 /
 *                                       movs r1,#4 / ldrsh
 * -fforce-addr FORCE_REGs an ARRAY's address BARE at the subscript, so any
 * later constant offset must be a runtime add on that register and the ldrsh
 * indexes with zero. A scalar struct's member offset is a link-time constant
 * and folds into the ldrsh index. That is the whole difference, and it also
 * explains for free why the FIRST half already matched: its base,
 * gUnknown_085D6C88, is an array.
 *
 * The header comment on the type asserted the OPPOSITE of this ("which only a
 * struct member array gives") and three waves of probes were run against it.
 * It had generalised from `const s16 *p = g.unk04;` -- a pointer to the member,
 * a different construct from a 2-D array with a constant outer index.
 * include/unknown-globals.h now records the probe.
 *
 * The retype also touches the promoted src/decomp/c_08051454.c, whose
 * `gUnknown_085643A8.unk00[..]` became `gUnknown_085643A8[0][..]`; that
 * function re-verifies byte-identical.
 *
 * Ruled out and still ruled out, do not re-measure: the wave-36 term-count
 * theory; `const s16 *p = gUnknown_085643A8.unk04;` (sinks the 4 into the
 * relocation); a pointer local with `q->unk04[m]` (wave 51 said cse
 * const-propagates it -- wave 86 measured that it does NOT, the pointer
 * survives in r8, but the 4 still folds into the ldrsh index); the toolchain
 * axis (`no-force` is byte-identical to the default, W77-G).
 *
 * Both guards are `>= g[a1] + 1` on a u8 member, which is what gives the
 * SIGNED `blt` -- the `+ 1` makes the right-hand side plain int. */
void StepRepeatedShotAndHitSfx(u16 a1)
{
    if (gUnknown_020296B0[a1].unk1b >= gUnknown_030045A4[a1] + 1)
    {
        sub_0803B48C(gUnknown_085D6C88[gUnknown_03004580[a1][1]]
                         .unk0c[gUnknown_03004580[a1][2] - 1]
                               [gUnknown_020296B0[a1].unk1a & 1]);
        gUnknown_020296B0[a1].unk1a++;
        gUnknown_020296B0[a1].unk1b = 0;
    }

    if (gUnknown_020298E0[a1].unk8d >= gUnknown_03004510[a1] + 1)
    {
        sub_0803B48C(gUnknown_085643A8[1][gUnknown_020298E0[a1].unk8c & 1]);
        gUnknown_020298E0[a1].unk8c++;
        gUnknown_020298E0[a1].unk8d = 0;
    }
}
asm(".global sub_08053520\n.thumb_set sub_08053520, StepRepeatedShotAndHitSfx\n");
