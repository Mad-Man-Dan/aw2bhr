#include "global.h"

/* STILL PARKED at 30.1% / +8 bytes (184 against 176). RE-MEASURED WAVE 58
 * (W58-B) and re-verified wave 59 (W59-B). Wave 59 tried one new axis; it made
 * things much worse, and the body below is the wave-45 draft, restored.
 * (try_match overwrites this file with whatever was last submitted -- if the
 * body here ever stops matching the description, this note is the one to trust.)
 *
 * WAVE 59 CROSS-BATCH (from W59-A, on sub_0800CAA0): this function is now the
 * THIRD example of one class, with sub_0800CAA0 (+12) and sub_08028D28 (+4).
 * W59-A isolated its residual to EXACTLY ONE LIVE VALUE -- the ROM
 * rematerialises a base every iteration where every candidate hoists it, and
 * the downstream cost is a callee-saved register saved that the ROM does not
 * save. Four spellings are ruled out across four waves there: `sym[i + K]`
 * (becomes a pointer giv), `p = sym + K; p[i]` (folds the addend into the pool
 * word, and the ROM's addend is 0), `p = sym; (p + K)[i]` (byte-identical to
 * the first), and the component-ref spelling. W59-A's conclusion is that the
 * remaining axis is loop.c's `move_movables` cost test and is NOT addressable
 * from C. **Treat this as a park with evidence, not as an attempt.** Adding a
 * fourth example is worth more than another failed spelling sweep.
 *
 * WAVE 59 -- THE ROM'S +32 IS A DUPLICATED FIRST INNER ITERATION, AND THE
 * ENTRY-`goto` LOOP SHAPE DESTROYS IT. Read the ROM's inner loop again: the
 * block at _080262D2 is a COPY of the loop body specialised to j == 1 --
 * `cmp r5,#1` where the general copy has `cmp r5,r1`, and the literal offsets
 * `+0x39` / `+0x3e` (unk38[1], unk3d[1]) where the general copy indexes by r1 --
 * ending in `b _0802631A`, which cross-jumps into the general copy's shared
 * tail. That is gcc's duplicate_loop_exit_test firing on a loop whose entry is a
 * jump to a top exit test: it copies the test and what follows into the
 * preheader and drops the entry jump.
 *   Wave 59 matched sub_08035080 by writing exactly that layout by hand --
 * `j = 1; goto _jtest; for (;;) { j++; _jtest: ... }` with the guards as
 * `continue`s -- and applied it here. It reproduces the general copy's layout
 * instruction for instruction (`b <test>`, pool, increment, test, three
 * continues, fall-through) and the candidate then measures **144 bytes, -32,
 * 5.1%**: writing the entry jump explicitly leaves loop_start immediately
 * before the INCREMENT rather than before the exit test, so
 * duplicate_loop_exit_test no longer applies and the peeled copy never appears.
 * It also loses the force-addr word entirely and hoists BOTH bases out of the
 * outer loop, so it is not a partial win either.
 *   Read as a rule: **the entry-`goto` shape and the duplicated first iteration
 * are mutually exclusive, and this function needs the duplicate.** The natural
 * `for (j = 1; j < n + 1; j++)` below is therefore RIGHT and is not the axis.
 * Do not spend another wave on the inner loop's statement form -- three
 * spellings have now been measured (continue-chain, nested-if, entry-goto), the
 * first two are identical and the third is 32 bytes short.
 *
 * WAVE 58 -- READ IT OFF THE RELOCATIONS, IT IS ONE LINE OF EVIDENCE.
 * The ROM's literal pool carries TWO WORDS FOR ONE OBJECT:
 *      .word R_ARM_ABS32 gUnknown_08090A60   <- a .rodata force-addr word,
 *            DEREFERENCED, used ONCE, only for the pre-loop unk02 read
 *      .word R_ARM_ABS32 gPlaySt   <- a PLAIN symbol address, loaded
 *            fresh at each of the two in-loop sites
 * The candidate emits only the first (`R_ARM_ABS32 .rodata`) and then holds it
 * in a register for the whole function, reaching the struct as `ldr r2,[r7]`
 * everywhere. A candidate with FEWER pool words than the ROM for the same
 * symbol is HOLDING something the ROM rematerialises, and try_match's
 * relocation list is the cheapest place to see it. This is the same inversion
 * as sub_0800CAA0 (+12) and sub_08028D28 (+4) -- three deltas, one mechanism;
 * see the wave-58 chapter in docs/agbcc-codegen.md.
 *
 * WHAT THE FUNCTION DOES
 * Deals each player slot a distinct random value. For every slot i in 1..n
 * (n = sub_0802490C(gPlaySt.unk02)) whose unk38 flag is still clear,
 * mark it 2, then re-pick sub_08026254()'s value until no OTHER marked slot
 * already holds it, and store it at unk3d[i].
 *
 * THE RESIDUAL IS A FORCE-ADDR *SPLIT*, NOT A HOIST (wave 45)
 * The goal is not to stop a hoist -- it is to get agbcc to force-addr the FIRST
 * reference and leave the other two as ordinary pool loads. That matches the
 * documented threshold ("it needs FOUR uses to hold; two is not enough -- CSE
 * const-propagates the address and emits the pool word twice"). The ROM behaves
 * like a function with ONE force-addr reference plus TWO plain ones; the
 * candidate behaves like one with 4+. The lever is therefore the NUMBER of
 * source-level references to gPlaySt, and the ROM's count is what has
 * to be reproduced -- the in-loop sites must reach the struct through something
 * that is not a fifth mention of the symbol.
 *   The body below is the wave-45 rewrite (outer test as a nested `if` rather
 * than wave 41's `continue`). It measures IDENTICALLY to the wave-41 draft at
 * 30.1% / +8, so the outer-loop statement shape is not the axis either. That is
 * a real result: two quite different spellings of the same control flow land on
 * the same byte count, so the remaining difference is not reachable from the
 * loop's statement form.
 *
 * WHERE THE +8 GOES: the held base costs a register, so `&unk38[0]` is hoisted
 * out of the OUTER loop instead of recomputed in it, `&unk3d[i]` is recomputed
 * at the store instead of CSE'd off `&unk3d[0]`, and `i + 1` spills to a 4-byte
 * frame the ROM does not have (`sub sp, #4` / `str r6, [sp]` against the ROM's
 * `mov sb, r4`).
 *
 * gUnknown_08090A60 IS ALMOST CERTAINLY THIS FUNCTION'S OWN FORCE-ADDR WORD,
 * and is NOT declared anywhere. The three neighbouring words are 0x08090A5C ->
 * 0x020288A0 (sub_08026254's), 0x08090A60 -> 0x03003FC0 (this one) and
 * 0x08090A64 -> 0x08499598 (sub_08026424's): one word per function, laid out in
 * FUNCTION ADDRESS ORDER. A force-addr pool is private to one function, so that
 * run is the signature of three private words and not of a shared table.
 * Declaring it as a `struct PlaySt *` global reproduces the first three
 * instructions exactly (wave 41 measured that) but is very likely the WRONG
 * model, and it does not fix the other five references anyway.
 *
 * AXES RULED OUT BY PROBE (wave 41, all re-confirmed as still worth skipping):
 *   - `j == i` vs `i == j`: SETTLED, it is `i == j`, already fixed below.
 *   - declaring gUnknown_08090A60 as a struct pointer (see above).
 *   - `u8 *flags = ...unk38; u8 *vals = ...unk3d;` bound inside the outer loop:
 *     kills the force-addr word ENTIRELY (both pool words come out plain) but
 *     LICM then hoists both bases out of the outer loop and folds
 *     `vals = flags + 5`. NOTE for the next agent: this is the closest anything
 *     has come to the ROM's pool split, and it is the one to build on -- it
 *     fails on hoisting, which is a different and more tractable problem than
 *     the force-addr hold.
 *   - `u8 *flag = &...unk38[i]; u8 *slot = &...unk3d[i];`: emits BOTH pool words
 *     in the ROM's order and drops the frame, but still hoists the two bases.
 *   - the same through a pointer global: agbcc starts emitting
 *     `gPlaySt+0x38` and `+0x3d` as their own pool words. Worst tried.
 *   - `struct PlaySt *p = &gPlaySt;` for the body: p is kept in
 *     one register for the whole function, the opposite of the ROM.
 *   - the outer `continue` vs a nested `if` (wave 45): byte-identical.
 *   - the inner loop in entry-`goto` form (wave 59): -32 bytes, see above.
 *
 * WAVE 87 (W87-A). Still +8, draft unchanged, 0 try_match spent. Two more
 * reference forms measured; see W87-notes.md.
 *   - `*((u8 *)&gPlaySt + (i + 0x38))` at all four loop-body sites
 *     (W86-F's bare-symbol-in-loop-body construct) INVERTS the ROM's pool
 *     split rather than reproducing it: the pre-loop `.unk02` read goes
 *     DIRECT and the loop sites acquire .rodata force-addr words reached by an
 *     extra `ldr r1,[r6]` indirection every iteration. NEW FACT: the
 *     -fforce-addr word follows the POINTER-ARITHMETIC reference, not the
 *     member reference. (It does preserve the duplicated first inner
 *     iteration, and it drops the prologue to TWO hi registers where the ROM
 *     and this draft both push three.)
 *   - `extern struct PlaySt g[]; g[0].unk38[i]` -- a constant-indexed
 *     STRUCT-ARRAY subscript, the form that produces W86-C's array tell
 *     (bare pool word + runtime `adds #0x38`, which is exactly what the ROM
 *     has) -- is BYTE-FOR-BYTE IDENTICAL to this draft. Force-addr word still
 *     held, `sl = 0x38 + base` still hoisted out of the outer loop, frame
 *     still there. A constant outer subscript does not defeat the hold.
 *   Ten reference spellings are now measured across waves 41/45/59/87 with no
 *   movement. The next step is NOT another spelling: see the sub_0800CAA0
 *   W87 note, where the same hold/rematerialise inversion flipped when one
 *   extra value was live across the loop. Look for a fifth live value.
 *
 * TYPES, all settled: n, i, j and v are u8 (`lsls #0x18; lsrs #0x18` on each,
 * and `cmp r5, r8; bls` is unsigned). The inner bound is `n + 1` computed as
 * its own int (`mov r4, r8; adds r4, #1`) and compared `bge`, i.e. SIGNED --
 * so `j < n + 1`, not an unsigned `j <= n`. The retry test is `j != n + 1`. */
void sub_08026290(void)
{
    u8 n;
    u8 i;
    u8 j;
    u8 v;

    n = sub_0802490C(gPlaySt.mapID);

    for (i = 1; i <= n; i++)
    {
        if (gPlaySt.aiControlled[i] == 0)
        {
            gPlaySt.aiControlled[i] = 2;

            do
            {
                v = sub_08026254();

                for (j = 1; j < n + 1; j++)
                {
                    if (i != j && gPlaySt.aiControlled[j] != 0
                        && gPlaySt.co[j] == v)
                        break;
                }
            } while (j != n + 1);

            gPlaySt.co[i] = v;
        }
    }
}
