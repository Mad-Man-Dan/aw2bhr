#include "global.h"

#include "proc.h"

/* PARKED at 140/144 bytes, -4, wave 38 (W38-B, re-attempted W38-E). The
 * semantics are settled and every operand, constant and struct offset below is
 * confirmed against the ROM; what is left is one CSE/layout residual, written
 * out in full at the bottom so the next agent does not re-derive any of it.
 *
 * What it does: `dist` becomes the larger of |unk2c - unk30| and
 * |unk2e - unk32| (unk40 records which axis won), then it fills
 * gUnknown_0202FEF8[] with a ramp of increasing half-steps until the remainder
 * no longer covers the next one, stores the remainder as the last element, and
 * publishes the element count in unk3c and the total in unk3a. sub_08074B60 is
 * the consumer -- it walks gUnknown_0202FEF8 by that same unk3c countdown.
 *
 * SETTLED, do not re-test:
 *   - unk2c/2e/30/32 are u16 (plain `ldrh`, never `ldrsh`), unk38 is s16 (read
 *     back with register-offset `ldrsh`), unk3c is a word, unk40 is a byte.
 *   - `step` is `s8`. agbcc's PROMOTE_MODE sets unsignedp for EVERY sub-word
 *     type, so an s8 local is held zero-extended: assignments truncate with
 *     `lsls #24; lsrs #24` and each READ re-sign-extends with `lsls #24;
 *     asrs #24`. That asymmetry is what pins s8 -- u8 would need three
 *     explicit (s8) casts, and s8-with-sign-extending-stores does not exist.
 *   - `step >> 1` is a SHIFT, not `/ 2`: a signed divide would emit the
 *     add-one-if-negative bias. The `asrs #0x19` off the same `lsls #24` is
 *     the cast and the shift fused into one pair.
 *   - The test really is `dist - (step >> 1) >= 0`, not `dist >= step >> 1`:
 *     the ROM subtracts into a scratch and compares against 0 in both the
 *     guard and the bottom test.
 *   - gUnknown_081CC4D4 is NOT a table. It is agbcc's own -fforce-addr word
 *     holding 0x0202FEF8; naming gUnknown_0202FEF8 directly is correct and
 *     produces the same word. See include/unknown-globals.h.
 *
 * THE REMAINING DIFF, all of it one cause. The ROM holds the .rodata pointer
 * in r7 from BEFORE the guard, ENTERS the loop by branching to it (`bge`) with
 * the zero-trip path on the fall-through, reloads the table base `ldr r5,[r7]`
 * on every iteration, and recomputes `lsls r1,r6,#24` inside the body. The
 * draft below instead falls through into a preheader that hoists the base
 * load, and CSE then carries the guard's `lsls` into the body across the back
 * edge -- two instructions fewer in the loop, hence -4.
 *
 * RULED OUT by compile_probe (7 variants, none moved the shape):
 *   - `for (i = 0; cond; i++)`, a plain `while`, and an explicit `if` wrapping
 *     a `do/while` all produce the identical hoisted-preheader shape.
 *   - binding the table to a local `s8 *` before the loop, and spelling the
 *     casts as `(int)step` / `(s8)((int)step + 1)`, are both byte-neutral.
 * MEASURED, and the lead worth taking: writing it as
 *     if (dist - (step >> 1) < 0) { g[i] = dist; }
 *     else { do { ... } while (...); g[i] = dist; }
 *   buys three of the four facts -- the `bge`-into-the-loop branch sense, the
 *   recomputed in-body `lsls`, and the cross-jumped shared `strb r3,[r0]` tail
 *   with a `b` to it. It fails only because gcc const-folds i == 0 on the
 *   zero-trip arm all the way down to the bare symbol and gives that arm its
 *   OWN `ldr r0,=gUnknown_0202FEF8` pool word instead of reusing r7, which in
 *   turn lets the loop keep its hoisted base. That version is +8 (152 bytes)
 *   and is deliberately NOT the one saved here. Getting the zero-trip arm to
 *   reach the table through the same force-addr register is the whole
 *   remaining problem.
 *
 * ---- WAVE 38, W38-E: one axis REFUTED, and the mechanism named ----
 *
 * REFUTED -- gUnknown_081CC4D4 IS NOT A POINTER OBJECT. The obvious reading of
 * the ROM's `ldr r5,[r7]` sitting INSIDE the loop is that the table is reached
 * through a real `s8 *` global in ROM, because a -fforce-addr constant-pool MEM
 * is RTX_UNCHANGING_P and LICM would hoist it. Tested directly by declaring
 * `extern s8 *gUnknown_081CC4D4;` and indexing it: agbcc stacks force-addr ON
 * TOP of the pointer variable and emits THREE levels --
 * `ldr r7,=<pool>; ldr r6,[r7]; ldr r0,[r6]` -- for 53.5% and a spurious
 * `R_ARM_ABS32 .rodata` word the ROM does not have. The header's force-addr
 * reading stands, `gUnknown_0202FEF8[i]` is the right spelling, and this axis
 * is closed. (The same probe incidentally confirms that the guard-lsls CSE and
 * the fall-through loop entry are independent of how the table is named: the
 * pointer version had `blt` out of the loop and reused the guard's `lsls` too.)
 *
 * THE MECHANISM, which the -4 description above states as a symptom. The ROM's
 * tail is ONE source statement's address computation duplicated into TWO
 * predecessors and then cross-jumped back together at `strb r3,[r0]`: the
 * loop-exit edge reaches it as `adds r0,r4,r5` (i + the base the last iteration
 * loaded) and the zero-trip edge as a bare `ldr r0,[r7]` (i folded to 0, so the
 * add disappears but the INDIRECTION survives). The indirection survives there
 * only because r7 is already live on that edge. In every draft so far the
 * `ldr r7,=.LC0` lands after the guard, r7 is dead on the zero-trip edge, and
 * CSE is then free to replace `MEM(constant_pool)` with the pool's own constant
 * -- which is exactly `get_pool_constant` folding, and it is what produces the
 * second, direct pool word. So the four symptoms are ONE fact and the lever is
 * upstream of all of them: MAKE r7 LIVE BEFORE THE GUARD.
 *
 * That is docs/agbcc-codegen.md's "Wave 35 (W35-E): binding a global's ADDRESS
 * to a local before a guard is what hoists its pool `ldr` above the branch",
 * and it is in tension with "Binding a -fforce-addr address to a POINTER LOCAL
 * destroys the force-addr level (wave 37, W37-G)" -- W35-E's case had no
 * force-addr level to lose. What is needed here is a spelling that hoists the
 * pool `ldr` WITHOUT collapsing the indirection, and neither chapter supplies
 * one. That, and not the loop shape, is what the next attempt should attack;
 * the loop shape is already solved by the if/else + do/while form above.
 *
 * NOT a permuter case: the residual is an instruction COUNT difference, which
 * docs/agbcc-codegen.md's "A residual of one EXTRA instruction is not a
 * permuter case either" rules out explicitly.
 *
 * ---- WAVE 73, W73-A: THE TOOLCHAIN AXIS IS RULED OUT WITHOUT SPENDING A PROBE
 *
 * parked.json listed "not tried: the toolchain axis" as this function's one
 * remaining lead. It is not a lead, and the reason is a mechanism test rather
 * than an experiment. Wave 38's measurement was never about the 0x0807xxxx
 * address range: the two functions that genuinely needed `old_agbcc` were the
 * two WHOSE LOOP BODY CONTAINS A CALL. This loop body is
 *
 *     dist -= step >> 1;  gUnknown_0202FEF8[i] = step >> 1;
 *     if (step <= 0xf) step = step + 1;  i++;
 *
 * -- four statements, no `bl` of any kind. The trigger mechanism is absent, so
 * `old-agbcc` / `old-agbcc-no-force` were deliberately NOT probed here; wave 56
 * already ran four such profile probes on mechanism-free candidates and got
 * zero positives, including one that met the documented address-range trigger
 * exactly and changed not one byte. Blind-probing profiles is what this axis
 * looks like when it is being misused.
 *
 * So the ONLY open lever remains the one W38-E named: make r7 (the force-addr
 * pool pointer) live BEFORE the guard without collapsing the indirection, so
 * the zero-trip edge reaches the table through the same register instead of
 * const-folding to its own direct pool word. Neither of the two relevant
 * chapters in docs/agbcc-codegen.md supplies such a spelling.
 */
struct Unk8074AD0Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 filler_34[0x38 - 0x34];
    /* 0x38 */ s16 unk38;
    /* 0x3a */ s16 unk3a;
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 unk40;
};

void sub_08074AD0(struct Unk8074AD0Proc *proc)
{
    s8 step;
    int dx;
    int dy;
    int dist;
    int i;

    step = 1;

    dx = proc->unk2c - proc->unk30;
    if (dx < 0)
        dx = -dx;

    dy = proc->unk2e - proc->unk32;
    if (dy < 0)
        dy = -dy;

    if (dx > dy)
    {
        proc->unk40 = 1;
        proc->unk38 = dx;
    }
    else
    {
        proc->unk40 = 0;
        proc->unk38 = dy;
    }

    dist = proc->unk38;
    i = 0;

    while (dist - (step >> 1) >= 0)
    {
        dist -= step >> 1;
        gUnknown_0202FEF8[i] = step >> 1;

        if (step <= 0xf)
            step = step + 1;

        i++;
    }

    gUnknown_0202FEF8[i] = dist;
    proc->unk3c = i;
    proc->unk3a = proc->unk38;
}
