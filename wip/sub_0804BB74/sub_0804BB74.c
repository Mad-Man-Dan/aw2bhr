#include "global.h"

/* PARKED, wave 55 (W55-H). Five attempts. The SHAPE IS SETTLED -- every
 * structural question below was answered against the ROM and the answers held
 * across all five attempts; what is left is one register-allocation residual,
 * described precisely at the bottom so the next agent does not re-derive any
 * of this.
 *
 * SETTLED, do not re-litigate:
 *  - The range test is `(u32)(u8)(unk02 - 1) <= 1`, NOT a switch and NOT a
 *    signed compare. A `switch (unk02) { case 1: case 2: ... }` emits the same
 *    `subs #1; lsls #24; lsrs #24; cmp #1` and then `blt`, where the ROM has
 *    `bhi`. The `(u32)` is what makes the comparison unsigned; the `(u8)` is
 *    what keeps the truncation alive (unk02 - 1 wraps at unk02 == 0, so it is
 *    not redundant and is not elided).
 *  - `unk08` on struct Unk08555850 is the LZ77UnCompWram source, reached with
 *    the `adds rN, #8` on the -fforce-addr base that c_0804BB28.c documents.
 *  - The inner loops are `for (i = N; i >= 0; i--, k++)`. The COMMA is
 *    load-bearing: `k++` written as a body statement puts `adds r2,#1` before
 *    the giv and biv decrements, where the ROM has it after both.
 *  - blockA's outer loop is `for (j = 0; j <= 0x40 && k != 0x800; j++)` --
 *    the j test first, then k -- and blockB's is
 *    `for (j = 0; k + 15 <= 0x800 && j <= 0x2c; j++)`, k first, then j.
 *    That ORDER is read off the bottom of each loop and is what decides which
 *    biv strength_reduce eliminates: blockB's back-edge test is the j test, so
 *    it becomes the giv compare `cmp r5, r7`; blockA's back-edge test is the k
 *    test, so j survives in a register.
 *  - `k = 0` must be inside EACH arm. Hoisted above the `if`, gcc cannot prove
 *    blockB's entry condition and emits a loop-entry guard the ROM does not
 *    have.
 *  - The tail is `CpuFastSet(gUnknown_085519FC, dst, (c >> 2) & 0x1FFFFF)`.
 *    Exact, including `mov r1, sl` for dst, once the base pseudo below is
 *    single.
 *
 * REMAINING DIFF -- one fact, not several. gUnknown_085519FC is reached
 * through the -fforce-addr word 0x08136044, so the ROM holds &(that word) in
 * sb and its dereference (0x085519FC) in ip, and the loop does
 * `mov r1, ip; ldr r0, [r1]`. This body reproduces that in blockA and in the
 * tail, but in blockB agbcc const-propagates the force-addr load back to a
 * plain `ldr r1, =gUnknown_085519FC` literal, which adds ONE pool word to
 * blockB's pool (`R_ARM_ABS32 .rodata` AND `R_ARM_ABS32 gUnknown_085519FC`
 * where the ROM has only the former). That is the +4 the size line reports.
 * This is the "CSE const-propagates the address and emits the pool word twice"
 * case in the literal-pool chapter of docs/agbcc-codegen.md; the doc says it
 * needs four uses of the address to hold and this function has one per arm.
 *
 * The four spellings tried, and what each cost:
 *  1. `src` local assigned AFTER the LZ77 call -- force-addr survives, but a
 *     second base pseudo spills `dst` to the stack (+4 frame).
 *  2. `src` local assigned BEFORE the call and passed to it -- one base pseudo
 *     and the dst spill goes away, but the force-addr load is then lost in
 *     blockA as well (ip takes `c >> 2` instead).
 *  3. inline `((u16 *)gUnknown_0200FC50)[...]` everywhere (THIS BODY) --
 *     force-addr survives in blockA and the pool there is the right 3 words in
 *     the right order; costs one `mov r9, r7` per arm because the LZ77
 *     argument and the loop base become two pseudos.
 *  4. blockB as an explicit pointer walk (`row += 32`) -- eliminates j the way
 *     the ROM does, but gcc cannot fold the pointer entry test and emits a
 *     loop-entry guard, which is worse than the index form.
 *  5. Wave 66 (W66-D): naming 0x08136044 explicitly was compile-probed both
 *     in-place and through the documented c_local `pp` binding. In-place adds
 *     one indirection because -fforce-addr fires on the pool symbol itself.
 *     Per-arm c_local bindings either hoist gUnknown_085519FC's loaded value
 *     (losing the ROM's per-iteration reload), or, with a read-site volatile
 *     pointer, spill the loaded address and rematerialise 0x4400 in the loop.
 *     The temporary header declaration used for the probe was removed.
 * The lever wanted is one that keeps a SINGLE base pseudo (2) while keeping the
 * force-addr load (3). Nothing in the current doc supplies it.
 *
 *  6. Wave 77 (W77-D): the wave-73 ROW 3 lever (`u16 **p = &gUnknown_085519FC;`
 *     in each arm, `(*p)[k]` at the use) was measured. It BUYS 4 BYTES
 *     (+12 -> +8) and it is still a regression: it makes the deref EARLY.
 *     agbcc emits `ldr r3,[r0]; ldr r3,[r3]` in the preheader and then walks a
 *     giv (`adds r4,#2`), where the ROM reloads `mov r1,ip; ldr r0,[r1]` on
 *     every iteration. It also spills dst (`sub sp,#8; str r1,[sp]`).
 *     13.9%, first difference +0xa. Row 3 is the wrong row here: the ROM wants
 *     address-early / deref-late-AND-REPEATED, and row 3's "late" deref is
 *     late only in the sense of being outside the address expression, not
 *     inside the loop. Reverted.
 *  7. Wave 77 (W77-D): the W77-A address-expression lever does NOT apply here.
 *     The only -fforce-addr word in this function's ROM pool (gUnknown_08136044)
 *     is the address constant of a POINTER GLOBAL, not of a struct/array base;
 *     the one array base in the function (gUnknown_0200FC50) appears in the ROM
 *     pool as a PLAIN literal at _0804BC00, which is exactly the case the W77-A
 *     chapter says must keep the array spelling.
 */

void sub_0804BB74(int a, void *dst, u32 c, int d)
{
    int i;
    int j;
    int k;
    int wide;

    LZ77UnCompWram(gUnknown_08555850[a].unk08, gUnknown_0200FC50);

    if (d != 0)
    {
        wide = (u32)(u8)(gUnknown_08555850[a].unk02 - 1) <= 1;
        if (wide)
        {
            k = 0;
            for (j = 0; j <= 0x40 && k != 0x800; j++)
            {
                for (i = 31; i >= 0; i--, k++)
                    gUnknown_085519FC[k] = ((u16 *)gUnknown_0200FC50)[j * 32 + i] + 0x4400;
            }
        }
        else
        {
            k = 0;
            for (j = 0; k + 15 <= 0x800 && j <= 0x2c; j++)
            {
                for (i = 16; i >= 0; i--, k++)
                    gUnknown_085519FC[k + 13] = ((u16 *)gUnknown_0200FC50)[j * 32 + i] + 0x4400;
                k += 15;
            }
        }
    }
    else
    {
        for (k = 0; k <= 0x7ff; k++)
            gUnknown_085519FC[k] = ((u16 *)gUnknown_0200FC50)[k] + 0x1000;
    }

    CpuFastSet(gUnknown_085519FC, dst, (c >> 2) & 0x1FFFFF);
}

/* WAVE 78 (W78-B).  Configured baseline reverified at 336/324 (+12), 30.6%,
 * 225 differing target bytes, first difference +0x10.  No pointer-global row
 * was retried: W77-D already proved both the row-3 pointer binding and the
 * nearby force-address rule inapplicable.  The remaining package is unchanged:
 * one base pseudo must serve the LZ77 destination, both loop arms and the tail
 * while the force-address dereference remains late and repeated in each loop.
 * The documented source spellings already bracket those mutually coupled
 * requirements. */







