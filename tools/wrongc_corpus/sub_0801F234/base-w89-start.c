/* WAVE 86 (W86-F) -- MIRROR-TWIN AXIS. Full measurement in
 * work/sub_0801F234/W86-notes.md. Twin: src/decomp/c_0801F34C.c
 * (sub_0801F34C, matched, adjacent). It is a TRUE SHAPE TWIN of the
 * producer/consumer kind -- same `e = &gUnknown_0200F920[sub_0801F3D4(id)]`
 * prologue, same `e->unk08[]` cursor array (this one appends, the twin scans),
 * same gUnknown_0848B780 dimension table -- and its header documents EXACTLY
 * this park's residual as that function's whole match: "the fresh local is a
 * new pseudo ... and costs a second hi register (+4 bytes); assigning back over
 * the DEAD PARAMETER ... is the ROM's allocation exactly".
 *
 * THE CONSTRUCT IS NAMED BUT HAS NO HOST HERE. The lever needs a dead parameter
 * to accumulate into; sub_0801F234 has one parameter and `a` is live to the
 * third statement from the end. e, tile and n are all fresh by necessity (the
 * park already pinned that tile must be a local and n is used twice). This is
 * W81-D's conclusion on the sibling sub_0801F19C stated from the other side --
 * "the park target must carry an ALREADY-LIVE range; freshness defeats the
 * lever" -- and this function satisfies neither half.
 *
 * ONE PROBE SPENT, REFUTED: STATEMENT order of `tile` and `n` swapped (n first),
 * on the mechanism that the pseudo created first wins the callee-saved register
 * via allocno number = RTL creation order. BYTE-NEUTRAL -- the emitted
 * allocation is this draft's exactly (a r6, e r4, tile r5, n r8, r8 round trip
 * intact). So STATEMENT order joins DECLARATION order (W77-K) as byte-neutral
 * for agbcc's allocno numbering, and that is the stronger of the two results
 * because statements do create RTL where declarations do not. Do not spend a
 * probe permuting independent statements to move a register.
 *
 * NEXT AXIS, from the twin's shape: the value to remove from the set live
 * across the two calls is `i`, not `n`. The twin's prologue has NO `i` local at
 * all; the ROM here carries i in the r1 scratch for free (established on the
 * sibling by W77-K) while every draft makes it a pseudo that pushes a fourth
 * value out to r8. No try_match spent this wave. */
#include "global.h"

/* PARKED at 124/120 bytes (+4), 31.7% identical. RE-MEASURED wave 77 (W77-K):
 * both recorded facts still hold exactly -- `n` lands in r8 where the ROM uses
 * r7 (costing `mov r6,r8; push {r6}`, `mov r8,r3` after the multiply, `mov
 * r2,r8` at the use and `pop {r3}; mov r8,r3`), and the sub_0801F444
 * result-parking pair (`adds r3,r0,#0` / `adds r0,r3,#0`) is missing.
 *
 * THE PARKING HALF NOW HAS A LEVER, found on the sibling sub_0801F19C this
 * wave: a dead in-expression assignment on the call result,
 * `(void *)(i = (int)sub_0801F444(a, i))`, produces the ROM's park AND moves
 * the 0x3ff mask into the register the ROM uses. IT IS NOT APPLIED HERE. On
 * sub_0801F19C it costs one extra `adds r1,rX,#0` because reusing `i` merges
 * i's two live ranges; here `i` is additionally live through
 * `&gUnknown_0200F920[i]`, so the merge is longer and the r7/r8 fact -- worth
 * five instructions against the parking pair's two -- dominates. Fix the
 * register assignment first, then apply the lever; applying it now would add
 * +4 on top of the existing +4.
 *
 * THE REGISTER FACT, stated precisely from the wave-77 diff:
 *      ROM     a -> r5, e -> r4, n -> r6, tile -> r7   push {r4,r5,r6,r7,lr}
 *      draft   a -> r6, e -> r4, tile -> r5, n -> r8   push {r4,r5,r6,lr}+r8
 * Both keep exactly four values live across both calls, so this is not one
 * live value too many (the wave-39 hypothesis, already refuted). agbcc reaches
 * PAST r7 for r8 by default -- sub_0801F19C's ROM does the same -- so what
 * needs explaining is why the ROM here stops at r7. THUMB `muls` cannot target
 * a hi register, which is why n-in-r8 also costs `mov r8,r3` after the
 * multiply where the ROM multiplies straight into r6.
 *
 * MEASURED THIS WAVE, do not repeat:
 *   - LOCAL DECLARATION ORDER IS BYTE-NEUTRAL. All four permutations of
 *     {e, i, tile, n} compile to identical bytes. Pseudo creation order from
 *     declarations does not reach this allocation, and by extension it does
 *     not reach any allocation in this project (independently replicated on
 *     sub_08052BBC the same wave).
 *
 * Ruled out in earlier waves and still ruled out: a plain result local
 * (coalesced), integer-space destination arithmetic (byte-identical),
 * old_agbcc (its `size: match` is fake -- it CSEs away the third
 * `ldrb r0,[r4,#5]` re-read the ROM actually performs), every spelling of the
 * count arithmetic, and the `lsls #5; lsrs #2` -> `lsls #3` fold, which is
 * byte-neutral. Statement order, the `tile` local and the three separate
 * unk05 re-reads in the tail are settled. */
void sub_0801F234(int a)
{
    struct Unk0200F920 *e;
    int i;
    u16 tile;
    u32 n;

    i = sub_0801F3D4(a);
    e = &gUnknown_0200F920[i];
    tile = e->unk08[e->unk05].unk00;
    n = gUnknown_0848B780[a].unk00 * gUnknown_0848B780[a].unk01;

    CpuFastSet(sub_0801F444(a, i),
               (u8 *)e->unk00 + (tile & 0x3FF) * 32,
               (n & 0x3FF) * 32 / 4);

    e->unk08[e->unk05].unk02 = a;
    e->unk05++;
    e->unk08[e->unk05].unk00 = tile + n;
}
