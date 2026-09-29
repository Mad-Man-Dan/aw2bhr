#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080723DC.
 * sub_080723DC @ 0x080723DC
 */

#include "hardware.h"

/* MATCHED in wave 60 (W60-B), byte-for-byte, after three waves parked at 88.3%.
 * Parked by wave 35 (W35-D), narrowed by wave 57 (W57-D), re-verified by exit
 * code in wave 59 (W59-G).
 *
 * THE LEVER IS THE `do { } while (0)` BELOW, AND IT SHOULD NOT BE TIDIED AWAY.
 * decomp-permuter found it in 937 iterations of a single 300 s run.
 *
 * The residual was ONE INSTRUCTION'S POSITION and nothing else -- 14 of 120
 * bytes, all between +0x26 and +0x36:
 *
 *   ROM    ... orrs r0,r1 / strh r0,[r2] / ldrb r0,[r2] / movs r1,#0x20 /
 *              orrs r0,r1 / strb r0,[r2] / ldr r1,=gPal / movs r0,#0 /
 *              strh r0,[r1]
 *   was    ... orrs r0,r1 / MOVS R3,#0 / strh r0,[r2] / ldrb r0,[r2] /
 *              movs r1,#0x20 / orrs r0,r1 / strb r0,[r2] / ldr r0,=gPal /
 *              strh r3,[r0]
 *
 * agbcc emitted `gPal[0] = 0;`'s constant two statements EARLY, MID-STATEMENT,
 * between the `.raw` assignment's own `orrs r0,r1` and its own `strh r0,[r2]`,
 * so the constant took r3 and the address took r0; the ROM materialises the
 * constant between the address load and the store, so the value is r0 and the
 * address r1.
 *
 * WHY THE `do { } while (0)` REACHES IT, and this is the generalisable part.
 * W59-G's analysis was right and its conclusion was one step short. It observed
 * that the hoisted insn lands mid-statement and concluded that cornering it
 * "would need a barrier between two operations of a single C statement, which C
 * cannot express" -- so the axis read closed. But the fix does not need a
 * barrier INSIDE the statement. A zero-trip `do { } while (0)` wraps the group
 * in `NOTE_INSN_LOOP_BEG` / `NOTE_INSN_LOOP_END`, and those notes bound the
 * region the hoist may move within: the constant can no longer leave the gPal
 * statement's group to land inside the `.raw` group, because the loop notes sit
 * between them. **A `do { } while (0)` is a region marker, not a statement
 * barrier, and that is why it reaches motion that `__asm__ __volatile__ ("")`
 * between the same two statements did not.** Wave 57 probed the barrier, found
 * it did nothing, and correctly concluded the placement is decided upstream of
 * sched -- which is true, and is exactly why a loop note works where a
 * scheduling barrier does not.
 *
 * It is also plausible original source rather than an artefact: three
 * consecutive writes that configure the blend registers, wrapped in the
 * canonical function-like-macro idiom, is what a `SetBlend(...)`-style macro
 * expands to. The same lever is recorded in the soft-float chapter of
 * docs/agbcc-codegen.md, and `StartHeaderBanner` carries one too.
 *
 * REFUTED EARLIER AND STILL REFUTED -- do not re-spend attempts on either:
 *  - NOT cse sharing the zero with the two earlier `= 0` stores. Changing
 *    gUnknown_03002020 and gUnknown_03002B28 to `= 1`, so no zero exists
 *    anywhere earlier, STILL emitted `mov r3,#0` inside the `.raw` group.
 *  - `*gPal = 0;` is byte-identical to `gPal[0] = 0;`.
 *  - Moving the gPal store earlier fixes the hoist and breaks other things:
 *    before the `target1_enable_bd` write it reorders the two stores, and
 *    before the `.raw` write it also reorders the literal pool (gPal ahead of
 *    0x0000FFE0, where the ROM has it after). The ROM's pool order pins the
 *    statement order to raw -> bd -> gPal, and the `do { } while (0)` keeps
 *    that order while still confining the hoist.
 *
 * WHAT WAVE 35 MEASURED AND IS STILL CORRECT -- the three writes to
 * gUnknown_030030E0 must be THREE DIFFERENT KINDS of access:
 *  - all `.raw`: agbcc folds all three into ONE halfword read-modify-write.
 *    -8 bytes, 22.5%.
 *  - all bitfield: the six single-bit writes do NOT combine -- six
 *    `movs #k; orrs` pairs. Size exact, 37.5%.
 *  - the mix below: `bits.effect = 3` gives the bare `orrs #0xc0` (the OR
 *    covers every bit the clear would have cleared, so agbcc drops the AND),
 *    the five target1 bg/obj bits are a genuine masked assignment on the u16
 *    container, and target1_enable_bd is a separate byte OR.
 * A 1-bit bitfield write and a `.raw` write to the same union are separate
 * accesses to agbcc, but two `.raw` writes are not. */
void Fade_CommonCallBack(void)
{
    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    do
    {
        gUnknown_03001FFC = 0x10;
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 0x1f;
        gUnknown_030030E0.bits.target1_enable_bd = 1;
    }
    while (0);
    gPal[0] = 0;
    EnablePaletteSync();
    gDispIo.disp_ct.bg0_enable = 0;
    gDispIo.disp_ct.bg1_enable = 0;
    gDispIo.disp_ct.bg2_enable = 0;
    gDispIo.disp_ct.bg3_enable = 0;
    gDispIo.disp_ct.obj_enable = 0;
}
asm(".global sub_080723DC\n.thumb_set sub_080723DC, Fade_CommonCallBack\n");
