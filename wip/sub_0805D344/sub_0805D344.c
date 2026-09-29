#include "global.h"

/* WAVE 87 (W87-A). IMPROVED, still not matched: 16.4% / 236 bytes (-8 of 244)
 * under the `configured` profile, up from wave 57's 13.5%. The whole fill loop
 * now matches the ROM instruction for instruction and the ENTIRE remaining -8
 * is one fact: `n` lives in r5 where the ROM keeps it in r8.
 *
 * TWO PARK CLAIMS REFUTED -- read these before doing anything here.
 *
 * 1. "gcc gives gUnknown_030045F0 a pointer giv and gives gUnknown_030046E0
 *    NONE ... whatever makes the second array ineligible is the thing that
 *    frees r5". THE ASYMMETRY WAS ALREADY REPRODUCED, in the wave-57 draft,
 *    and nobody had checked. That draft emits `add r4,r4,#1` for
 *    gUnknown_030045F0 and rematerialises gUnknown_030046E0 from the pool into
 *    `add r1,r5,r1` every iteration -- exactly the ROM's split. The asymmetry
 *    was never the residual, so the wave-59 analysis built on it is void.
 *
 * 2. "The hoist and n's register are the same fact; do not chase them
 *    separately." They are two facts. The `&gUnknown_08499594` hoist is now in
 *    the ROM's preheader position WITH `n` still in r5.
 *
 * WHAT CLOSED THE HOIST -- a VOLATILE-QUALIFIED READ OF THE POINTER GLOBAL:
 *
 *     x = (*(struct Unit *volatile *)&gUnknown_08499594)
 *             [gUnknown_030045F0[n]].unk00;
 *     gUnknown_030046E0[n] = sub_08042D1C(gUnknown_030033EC, x);
 *
 * Measured, all four, in this function:
 *     plain `gUnknown_08499594[k].unk00`                -> address NOT hoisted
 *     `(*(struct Unit **)&g)[k].unk00`           -> address NOT hoisted
 *     `(&g)[0][k].unk00`                                -> address NOT hoisted
 *     `(*(struct Unit *volatile *)&g)[k].unk00`  -> address HOISTED into
 *                                                          the preheader, in
 *                                                          the ROM's position
 * A non-volatile load of a pointer global keeps the global's address folded
 * into the load and re-emits the pool load every iteration; a VOLATILE load
 * force_regs the address into its own pseudo, which is loop-invariant, and
 * LICM then hoists it while the VALUE still reloads each iteration. That is
 * exactly the ROM's `ldr r5,=gUnknown_08499594` (preheader) + `ldr r1,[r5]`
 * (body) pair. Taking the address is NOT the lever; the qualifier is.
 * Written up in docs/agbcc-codegen.md.
 *
 * MODEL NOTE, honestly flagged: the shared declaration in
 * include/unknown-globals.h was NOT retyped. `gUnknown_08499594` has 20+
 * promoted users and the ROM does not force the retype -- the local cast gets
 * the same code. If a later wave wants the natural spelling
 * (`extern struct Unit *volatile gUnknown_08499594;`) it must re-verify
 * every one of those units with tools/verify_batch.py first.
 *
 * ALSO SETTLED (w87): binding the second call argument to a local BEFORE the
 * call is required. agbcc otherwise emits arg1 (`gUnknown_030033EC`) first and
 * the loop body is 11 instructions against the ROM's 10; with `u8 x;` bound
 * first it is 10.
 *
 * THE WHOLE REMAINING -8 IS `n`'s REGISTER, and it is now itemised:
 *     movs r0,#0 / mov r8,r0   vs  movs r5,#0          +2
 *     movs r1,#1 / add r8,r1   vs  adds r5,#1          +2
 *     mov r2,r8 / cmp r2,#1    vs  cmp r5,#1           +2
 *     mov r0,r8                vs  (in place)          +2
 * Everything else -- both loop guards, the stack spills, the ?: with its arms
 * the right way round, the duplicated in-loop direction test, the inner
 * descending loop, both six-access swaps with all four dead volatile loads,
 * and the epilogue -- is byte-exact.
 *
 * PERMUTER, w87: FIRST EVER RUN on this function. 480 s, 4 threads, from this
 * draft, RAN TO COMPLETION (exit 0, not killed). One candidate surfaced and
 * scores 16.0%, worse than this draft; nothing matched at the byte level.
 * Draft restored and re-verified unchanged. A real negative about THIS base,
 * but one run from one base is weak evidence -- wave 83 needed a chain of
 * three. CHAIN from best.c next.
 *
 * NEXT: this is a PURE REGISTER-ASSIGNMENT residual -- one loop counter that
 * has to land in a hi register with every instruction around it correct. Any earlier
 * permuter negative on this function is void (wave-86 rule) because the draft
 * changed. Do NOT author the pointer walk (W59-E: it is a giv).
 *
 * Wave 57's still-valid note: the `volatile u8 []` typing of gUnknown_030045F0
 * this needs was made and re-verified in wave 57 against the twelve promoted
 * builders (sub_0805CA60, sub_0805D2A0, sub_0805CDF0, sub_0805CE20 all still
 * match after it).
 */

void sub_0805D344(u32 a1)
{
    int n;
    int i;
    int j;
    u8 t;
    u8 x;

    n = 0;

    while (gUnknown_030045F0[n] != 0)
    {
        x = (*(struct Unit *volatile *)&gUnknown_08499594)[gUnknown_030045F0[n]].type;
        gUnknown_030046E0[n] = sub_08042D1C(gUnknown_030033EC, x);
        n++;
    }

    if (n > 1)
    {
        for (i = 0; i <= n - 2; i++)
        {
            for (j = n - 2; j >= i; j--)
            {
                if (a1 == 0
                        ? gUnknown_030046E0[j] < gUnknown_030046E0[j + 1]
                        : gUnknown_030046E0[j] > gUnknown_030046E0[j + 1])
                {
                    t = gUnknown_030046E0[j];
                    gUnknown_030046E0[j] = gUnknown_030046E0[j + 1];
                    gUnknown_030046E0[j + 1] = t;

                    t = gUnknown_030045F0[j];
                    gUnknown_030045F0[j] = gUnknown_030045F0[j + 1];
                    gUnknown_030045F0[j + 1] = t;
                }
            }
        }
    }
}
