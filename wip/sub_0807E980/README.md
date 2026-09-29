# sub_0807E980

0x0807E980, 1040 bytes, THUMB, parked.

Best score so far: 99.4%.

## What it does

One frame of an animated screen run by a proc. It draws the screen's sprites and slides them into place with Interpolate over its frame counter; after 15 frames it loads the screen's sprite graphics and palette (as sub_0807F434 does) and moves the proc to its next step.

## How close it is

Compiles to the right size (1040 bytes); only 6 bytes differ (99.4% match), all in the set-up of the final copy loop: the loop counter's start value is set after the destination address instead of between the source and destination addresses.

## What is left

The original's order means its compiler turned the destination address into a stepping pointer only on its second pass over the loop, after setting up the counter; every spelling so far does it on the first pass. Find what delays it; the recipe for the compiler's per-pass debug dumps that show this is in work/sub_0807E980/NOTES.md.

## Already tried

- Both addresses computed from the loop counter: the compiler always sets them up and steps them in the same order, but the original steps them in the opposite order to their set-up.
- A descending counter (7 down to 0): the counter is set first, but the addresses then step backwards through extra constants; 8 bytes too long.
- Explicit source and destination pointer variables stepped in the body: right order, but they cost registers and the rest of the function shifts (12 bytes too long).
- Binding the buffer address to a local, in several places including inside a call argument: the value gets its register too early and registers shuffle elsewhere; worse.
- The older compiler: worse (14 bytes differ).
- About 150 variants of the current stepping-pointer form, and `do { } while (0)` around parts of the loop: all set the counter last.
- Automatic permuter, several runs (about 50,000 attempts): found one earlier register fix, nothing for this order.

## Files

- `sub_0807E980.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

size-exact (1040 bytes), 99.0% identical, 10 of 1040 bytes differ, first difference at +0x370. W81-B re-measured by exit code: unchanged (do/while variant did not exceed it; snapshot restored).

### What still differs

Entirely inside the CpuFastSet double loop at the tail; everything outside it is byte-exact. The ROM initialises src, then the loop COUNTER, then dst -- but increments dst before src. So the giv init order (src,dst) and the giv increment order (dst,src) disagree, and the counter init sits BETWEEN the two giv inits. WAVE 83 (W83-A): comma-depth binding variant authored and compile-checked (not yet trymatched): base=(int)&gUnknown_0200FC50 assigned INSIDE the first sub_08043BA4 third argument (comma creates the reference at a point no statement boundary reaches), loop base then &((u8 *)base)[i*0x100] instead of &gUnknown_0200FC50[i*0x100]. Compiles clean under configured profile; diff its tail against target.s before further levers. Nothing else changed.

### Why it is close

The whole function body, both jump-free arms and every call sequence are exact. The residual is loop-preheader instruction ORDER, not instruction selection: the size is exact and the multiset is right.

### Already ruled out

- old_agbcc -- MEASURED BY THE ORCHESTRATOR, wave 56, and it is WORSE: 98.7% (14 bytes) against the default's 99.0% (10 bytes), and the first difference moves EARLIER, +0x78 against +0x370. Tested because the wave-38 discriminator says the two allocators diverge exactly where a loop body contains a CALL, and this loop body calls CpuFastSet -- so the hypothesis was specific and well-founded. It is still negative.
- -fforce-addr removal -- inert in this address range. data/compiler-overrides.json's own measurement: no promoted file above 0x08063A3C differs without the flag, and this function is at 0x0807E980.
- Both src and dst as loop givs -- when both are givs, agbcc always emits the init order and the increment order the SAME way. Measured both directions: writing dst as a hoisted statement gives src,dst / src,dst; writing neither gives dst,src / dst,src. Neither reproduces the ROM's disagreeing pair.
- check_dbra_loop always emits the counter init LAST, after every giv init -- so the counter cannot be made to land between the two giv inits while both remain givs.
- `gUnknown_0200FC50 + i*0x100 + j*0x400` (plain pointer arithmetic) -- makes agbcc force the symbol into a .rodata address constant and do a DOUBLE load inside the loop, which blocks the src strength reduction entirely. `&gUnknown_0200FC50[...]` does not. Documented in docs/agbcc-codegen.md (wave 56, W56-C).
- decomp-permuter, 12,480 iterations / 300 s from the 98.65% draft. It found the new_var hoist that fixed the src/dst REGISTER assignment (98.65% -> 99.0%) and found nothing for the remaining order.
- W81-B: ROM window re-measured against the diff convention (- = target): preheader order is SRC group (sl-bound base + i<<8), CNT movs r6,#7, DST group (pool 0x06015000 + i<<11); loop bottom steps DST +0x100 then SRC +0x400. The parked description is confirmed.
- W81-B: descending for (j = 7..0) is ACTIVELY HARMFUL -- cnt init moves BEFORE the address groups, the negative pointer steps are emitted as three extra pool words (0x06015700/0xfffffc00/0xffffff00) and the whole outer-loop allocation reshuffles.
- W81-B partially positive result: a source-level do/whose preheader statements written in the ROM's own textual order -- src_p = (int)&gUnknown_0200FC50[i*0x100]; j = 7; vram_p = 0x06015000 + i*0x800; do { CpuFastSet((void*)src_p,(void*)vram_p,0x40); vram_p += 0x100; src_p += 0x400; j--; } while (j >= 0); -- reproduces ALL FOUR orderings exactly (init S,C,D and bottom D,S appear as COMMON diff lines; first spelling ever measured to do so). Residual after it is pure allocation: the base is NOT held in sl across iterations (candidate reloads gUnknown_0200FC50 from the pool each i-iteration instead of reusing the Decompress destination register via mov sl,r4), the i-counter home shifts r5->r6 with its adds-i+1/r8 copy moved after the address groups, and proc access swaps r9<->sl downstream. Anchoring the base through an int local assigned BEFORE the two sub_08043BA4 calls regresses further (registers displace everywhere); a u8*/int base bound immediately before Decompress loses the ldr-r4 passthrough as well. Open question narrowed to: what keeps the Decompress force-address pseudo alive into the loop while three source-level pointer locals exist.
- W85, descending source counter `for (j = 7; j >= 0; j--)` keeping the j-indexed addresses: 95.9% (+8). The counter init position IS fixed (movs r6,#7 first, before the giv inits), but the address givs then fold +7*0x100 into their inits and step NEGATIVE via pool words (0x06015700 init, -0x100 / -0x400 steps) -- the ROM keeps positive steps with no offset fold.
- W85, same with (7 - j) indexing: 83.5%, first difference moves to +0x67 -- breaks the pre-loop region.
- W85, explicit body-stepped pointers (u8 *src / *dst locals, CpuFastSet(src,dst,0x40) then dst += 0x100; src += 0x400; inside the body): 54.1% (+12, first difference +0xc) -- the two extra pointer locals change the frame. The ROM's implied shape (source counter init before the giv inits, positive dst-first steps) is the pointer-variable form, but the frame cost is real; needs a spelling where the pointers do not add pseudos.

### Settled

- Source-level `u8 *` locals incremented in the loop body DO reproduce the ROM's instruction order exactly -- verified, a descending `for (j = 7, dst = ...; j >= 0; j--)` matches the ROM instruction for instruction in that loop. It costs a register: `proc` is evicted from r7, the base address stops living in sl, and that is +12 bytes spread over the rest of the function. Binding the base to its own local to buy sl back made it worse. So the loop shape is SOLVED and the open question is purely the register budget.
- The struct is derived and in the draft (struct Unk807E980, fields at 0x2c/0x30/0x34/0x48/0x4c/0x52/0x58/0x5c/0x60/0x64).
- Everything outside the tail loop is byte-exact and must not be re-derived.

### Why it is parked

W77-M re-measured 2026-08-18 and the park stands: making `src` alone a source-level u8 * with dst left as a giv is 33 bytes; making both src and dst source-level in a descending `for (j = 7, dst = ..., src = ...; j >= 0; j--)` is 477 and no longer size-exact. The register-budget diagnosis reproduces exactly and no new idea was found. Wave 56 (W56-C, one agent on this one function; toolchain axis added by the orchestrator). The open question is narrow and named: what source shape gives source-level ordering in that inner loop WITHOUT spending the extra register. Do NOT hand this back as a generic near-miss -- wave 55 measured rework at 2/10 and both wins carried a correct specific diagnosis. This one has a correct specific diagnosis, so it qualifies, but only with a NEW idea about the register budget, not a repeat of the axes above.

### Wave 82

WAVE 82 (W82-C): W81-B do/while spelling reconstructed from the parked text and re-measured configured; below the old-shape champion, not accepted, draft/best.c restored. It DOES reproduce all four orderings again this wave (preheader init S,C,D; loop-bottom D,S appear as common diff lines) and the residual reproduces as the three recorded facts: base reloads gUnknown_0200FC50 from the pool per i-iteration instead of mov sl,r4; the i+1 giv (adds rN,#1/mov rM,rN) lands AFTER the two address groups instead of right after i-init; proc access swaps r9<->sl downstream. NEW allocation lever measured and CLOSED: binding the VRAM pointer to the dead-by-then x local (decl set back to four ints i/j/x/src_p, identical count to the champion) leaves the residual shape unchanged while allocno homes reshuffle (src_p r5, vram r7, j r4) -- pseudo COUNT is therefore not the blocker; the blocker is which pseudo wins r4/sl across the Decompress call. Variant retained in work/sub_0807E980/w82-snapshot.c.

### Wave 84

WAVE 84 (W84-A): the staged comma-depth lead was MEASURED AND REJECTED twice. Anchor inside the first sub_08043BA4 third argument = miss (base materialises ~4 instructions early as ldr r3/mov sl,r3 before both calls, shifts +4, flips the 08234B10/0200FC50 pool order, swaps globals r4<->r5). Corrected anchor at Decompress's second argument also misses: an explicit int local crossing bl Decompress gets its callee-saved home AT THE ASSIGNMENT, while the ROM homes its CSE pseudo to sl only via the late movs r5,#0; mov sl,r4 pair. The unification mechanism is real; NO binding placement reproduces that home-timing. Comma-depth axis CLOSED on this function.

### Wave 90

WAVE 90 (W90-A): 99.0% -> 99.4%, 10 -> 6 bytes, size-exact, first difference +0x370 (draft = work/sub_0807E980/w90-R6-994.c). The inner loop is a WALKER plus a GIV OF THE WALKER: `for (j = 0, new_var = 0; j < 8; j++) { x = (0x06015000 + (i * 0x800)) + new_var; CpuFastSet(&gUnknown_0200FC50[(i * 0x100) + (j * 0x400)], (void *) x, 0x40); new_var += 0x100; }`. The dst giv's increment then goes before the walker's body increment, i.e. before the src giv's (which goes before j++): the ROM's dst-then-src loop bottom, which two givs of one biv can never give (they increment in init order). The D sum in its own statement before the call puts i<<11 ahead of i<<8 as in the ROM (inline in the call: 8 bytes). ONLY RESIDUAL: `movs r6,#7` after the dst init instead of between src and dst. RTL dumps (-dL/-dG, recipe in NOTES.md) pin it: i+1 and i<<8 are GCSE/PRE insertions; src is a pass-1 giv init; `j = 7` is check_dbra_loop in the SECOND loop pass, which runs before that pass's giv reduction; so the ROM's dst init is a giv first reduced in PASS 2, and every spelling measured reduces it in pass 1. 144 compiled variants (D temp x/fresh/inline x walker increment end/for-increment/first x init for-init/statement x do/while(0) on each of three statements) all emit the counter last. Ruled out this wave: the matched twin c_080790D0.c's walker form (+12, proc evicted to r9 and p rematerialised per outer iteration; with/without p, int walkers, off walker all the same), dst-walker-only (96.5%, dst init is SOURCE and lands first), src as a source walker with dst a giv (85.7%), do/while(0) anywhere in the body (byte-identical to R6). Permuter: directed 9,242 it from the old base, directed 14,347 it from R6 (PERM_GENERAL walker forms + RANDOMIZE; w90-R6-directed.perm.txt), and undirected 13,173 it from R6: nothing.

### Wave 92

W92-C: unchanged 99.42%/6 bytes, size-exact, first difference +0x370. A 900 s permuter run from --current (12,235 iterations) found nothing -- the third 900 s run from this base across two waves with no result. THE +12 EXPLANATION IN W77-M/W85 IS REFUTED: the pointer/do-while form is NOT +12 because it 'adds two pointer locals'. Rewriting the same do-while using ONLY locals the function already declares (x, new_var, j -- zero new declarations) is still +12 at 52.9%. Read off the PROLOGUE diff, the real cause is one placement: `proc` lands in r9 (`mov r9,r0`) where the ROM keeps it in r7 (`adds r7,r0,#0`), and that single difference cascades to all 12 bytes; every instruction of the copy loop itself is present and correct with only different registers. NEWLY RULED OUT, each measured: (a) `register int ... asm("r4")` / `asm("r5")` / `asm("r6")` on the source pointer, destination pointer and counter is BYTE-IDENTICAL to the unpinned form in all three variants -- the pin is inert here, not harmful; (b) pinning `proc` to r7 (parameter renamed, copied into a pinned local) is destructive: 1032 bytes (-8), 18.56%, first difference +0xa. NEW BASES: work/sub_0807E980/w92-ptr.c is the W81-B pointer form written out in full (53.52%, +12), and a 900 s permuter run from it climbed to 94.42% SIZE-EXACT with first difference +0xff, kept as work/sub_0807E980/w92-ptr-perm1.c. That is the first search this starting point has ever had, and chaining from w92-ptr-perm1.c is a better use of the next search budget than a fourth run from the 99.4% draft. Precedent: sub_0807F8FC (src/decomp/c_0807F8FC.c) is in the same region, MATCHES, and uses deliberate register pins plus a volatile save slot and an empty asm barrier; a new chapter in docs/agbcc-codegen.md records what pins do and do not reach.

### Wave 93

W93-F: WAVE 92's RECOMMENDED STARTING POINT IS WRONG C AND IS WITHDRAWN. work/sub_0807E980/w92-ptr-perm1.c (94.42%, size-exact) does not do what the function does: its copy loop sets vram_p = i and computes 0x06015000 + vram_p * 0x800 INSIDE the loop while stepping vram_p += 0x100, so the destination advances 0x80000 a pass where the ROM advances 0x100. Seven of the eight row destinations are wrong. Renamed w92-ptr-perm1.c.wrongc. WHY IT SCORED 94.42% ANYWAY, which is the part to carry forward: the compiler strength-reduces the scaled destination into a stepping pointer either way and materialises the step as a two-instruction constant -- the ROM's 0x100 is `movs r0,#0x80; lsls r0,#1`, the broken 0x80000 is `movs r0,#0x80; lsls r0,#0xc`. Same instructions, same length, one immediate field apart. A WRONG CONSTANT INSIDE A STRENGTH-REDUCED LOOP IS NEARLY FREE IN THE BYTE SCORE, so no percentage, size check or uninitialised-read check can catch this class; only reading the arithmetic can. Corrected to the ROM's semantics and keeping everything else the search found (w93f-ptr-fixed.c: vram_p = 0x06015000 + i * 0x800, passed to CpuFastSet directly), it measures 51.24% at +12 bytes -- the same +12 as w92-ptr.c before any search. So the whole 53.5% -> 94.4% climb was the broken loop, the pointer form's real cost is unchanged, and chaining from it would have searched a broken function. The open question is still wave 92's: the do-while shape needs one more callee-saved register than the original's and takes the one holding the record pointer. Draft restored to the 99.42% form, unchanged.

### Wave 94

W94-B: fourth undirected permuter run from the 99.42% draft (--current, 900 s, 4 threads), the first under the length-penalised scorer (AW2_PENALTY_SIZE=1000). 30,055 iterations, 944 errors, ZERO improving candidates -- the search never beat the draft's objective score of 60, so nothing was verified and the draft was not touched (re-verified: 1040/1040, 99.42%, first difference +0x370). The scorer fix cannot help this draft: it is already size-exact, so the length term is zero for it and for every size-exact neighbour and the ranking is unchanged. The residual is not an allocation choice at all -- it is which LOOP PASS reduces the destination pointer to a giv (the ROM's is reduced in pass 2, after check_dbra_loop wrote the counter; every spelling measured here reduces it in pass 1), so an allocation search cannot reach it. w92-ptr-perm1.c.wrongc was not used.

### Wave 97

wave 97
Base: the 99.42% draft (`sub_0807E980.w97-start.c`), restored as the final source. It is unchanged.

New finding on the residual (the `movs r6,#7` position): making the SOURCE pointer a walking local
(`u8 *src = &gUnknown_0200FC50[i * 0x100]; ... src += 0x400;` inside the `for (j...)` body, declared in a block around the loop,
with the destination left as the `(0x06015000 + i*0x800) + new_var` giv) moves `movs r6,#7` to the ROM's place:
after the source init and BEFORE the pool load of 0x06015000 and the destination sum. Reason: the source is then an
ordinary biv whose init is an original insn in source order, the loop counter's `j = 0` (rewritten to 7 by check_dbra_loop)
follows it, and only the destination giv init is emitted afterwards at loop start. This CONFIRMS the wave-90 reading that the
order is the ROM's giv/biv split, and shows the lever is which of the two pointers is a biv. The variant
(`sub_0807E980.q1.c`, increments in the order `new_var += 0x100; src += 0x400;`, which matches the ROM's step order) is
size-exact but scores 97.21% (first diff +0x364): the two increment constants take r2/r3 instead of r0/r1, and the
preheader is ordered `lsls r0,#8; add r4; add r0,r5,#1; mov r8; lsls r0,#0xb; movs r6,#7; ldr; adds` where the ROM has
`adds r3,r5,#1; mov r8,r3; lsls r1,#0xb; lsls r0,#8; mov r2,sl; adds r4; movs r6,#7; ldr; adds`. So the trade is now
between ONE misplaced constant and a shuffled preheader plus two register numbers. Making both pointers walkers (w1-w4 in
build/probe/w97j.py) costs 12 bytes (an extra register). A 900 s permuter chain from q1 (2 threads): NO-IMPROVEMENT: the text score fell from 2160 to 1280, but every candidate that was verified scored below q1 (best 93.1%, others 80-92%), so nothing was adopted.
Pre-registration (this batch): none for this function. Proposed summary: unchanged 99.42%; add to `tried`: source pointer as a
walking local (moves the counter init to the ROM's place but shuffles the preheader and constant registers, 97.2%).

wave 97 (second pass)
Base: `sub_0807E980.q1.c` (source pointer as walker, 97.21%). Draft `sub_0807E980.c` unchanged (99.42%).
Read the RTL (-da, .greg): the wrong constant registers are RELOAD hand-outs, not allocation. `r5 += 0x100` and `r4 += 0x400` need a register operand, so reload creates insns 1399/1402 and takes spill regs round-robin: the 99.42% draft gets r0, r1 (ROM), q1 gets r2, r3 because the hand-outs earlier in the preheader (`mov r3,sl` for the base) already advanced the rotation. So the constant registers follow the preheader ORDER, and the preheader order is the thing to fix. ROM order: [i+1 copy][i<<11][i<<8][base+ (mov r2,sl)][movs r6,#7][pool 0x06015000 + add]. q1 order: [i<<8][mov r3,sl; add][i+1][i<<11][movs r6,#7][pool][add] - the source init is an ordinary insn in source position and precedes the hoisted invariants.
Tried making the `i+1` and `i*0x800` come first in source (copy-back outer loop `for (i = 0; i < 4; ) { int ni = i + 1; ...; i = ni; }`, with and without `int d = i * 0x800`, with the base bound to `u8 *base`): the counter init sits in the ROM's place and the constants get r0/r1, BUT global allocation changes (proc moves to r8, `ni` takes r7, the base is no longer held in sl, +8 bytes, 54%). Swapping the two increments' order, `j = 0, new_var = 0` order: 97.21% unchanged.
The sibling sub_0807F434 has the same loop; the copy-back outer loop worked there (see its NOTES), so the difference is register pressure in this bigger function (proc, base and ni compete for r7/r8/sl).
Untried: copy-back outer loop plus something that lowers `ni`'s weight below proc's (e.g. compute `ni` after the inner loop from `i`, which is what the draft already does).

wave 97 (W97-PG)
Permuter chain: 1 link (540s), 99.42% -> 99.42%, NO-IMPROVEMENT. Draft unchanged.

</details>
