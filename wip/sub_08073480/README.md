# sub_08073480

0x08073480, 244 bytes, THUMB, parked.

Best score so far: 87.3%.

## What it does

A per-scanline wave effect on background 3. It fills a scanline buffer with a horizontal and vertical scroll value for each of the 160 lines (a base scroll plus a sine wobble), then sets DMA channel 0 to copy one entry into BG3's scroll registers at every horizontal blank.

## How close it is

Compiles to the right size (244 bytes) with the same instructions in the same order; 31 bytes differ (87% match), all register names: two scratch registers (the row pointer and a multiply temporary) are swapped throughout the loop body.

## What is left

Nothing in the source has reached the swap, and it is decided inside one loop body with no calls, where the usual register-ranking tricks have no effect. Next: a permuter run from this draft (the last run started from an older, worse version), or reading the allocator's choice in the compiler's per-pass debug dumps.

## Already tried

- Automatic permuter, three runs: took an older draft from 50% to 76%, then found nothing; the last produced only invalid code that reused the loop counter as a temporary (the 91% in best.json is also inflated by scoring header-expanded output).
- Giving the second store's product its own dead variable (`a`) instead of `b`: much worse (46%).
- A fresh variable for the product, or a named local for the sine value: the compiler merges it back and the two copies the original has disappear.
- Adding the global first in each sum: much worse (47%); it moves the global's address out of the loop.
- Adding the row index to the base in integer space: matches the original's operand order but is one byte worse; the swap stays.
- Every compiler configuration: none matches; the older compiler is one byte better (30 differ) but still not a match.
- `do { } while (0)` in five places: no effect on the swap; it only moved registers that were already right the wrong way.

## Files

- `sub_08073480.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

87.3% identical, SIZE-EXACT at 244 bytes, 31 of 244 bytes differ (wave 73, W73-D). NOTE: the draft and best.c now AGREE. Until this wave the draft held the 50.4% / -4 byte pre-permuter version while best.c held the 76.2% size-exact one, from wave 46 to wave 73, and this field reported best.c's score the whole time.

### What still differs

31 bytes, and every one is a register NAME on an instruction stream that is otherwise 1:1 with the ROM -- the pure register-name class. The ROM holds the `sin * unkNN >> 20` chain in r1 and reloads gUnknown_0300200C / gUnknown_03002000 into r0 inside the loop; the candidate uses r3 for the chain and r1 for the row pointer. First difference at +0x52.

### Why it is close

Size-exact with the instruction order correct and both of the ROM's `adds rD, r0, #0` copies now present. The mechanism that produces them: the THUMB multiply ties its destination to operand 1, so `x * y` compiles into whichever register holds x unless the PRODUCT is given its own set. Assigning the product to an already-dead variable inside the consuming expression (`(b = sin * unk34) >> 20`) forces the copy. A named local does NOT work -- agbcc coalesces it straight back.

### Already ruled out

- WAVE 77 (W77-K): GIVING THE SECOND STORE'S PRODUCT ITS OWN DEAD VARIABLE IS A STRONG NEGATIVE, which closes W73-G's open question from the other side. Assigning dst[1]'s product to `a` (dead after its own read in that expression) instead of to `b` is 87.3% -> 45.9%. It destroys the ROM's `adds r2,r0,#0` copy in front of `a`'s ACCUMULATOR multiply -- a copy the ROM has on `a` and does NOT have on `b`. `a` must keep a SINGLE set: its own multiply copy depends on it. The two accumulators are not symmetric and must not be spelled symmetrically.
- WAVE 77 (W77-K): THE POINTER-ADD OPERAND ORDER IS REACHABLE AND IS NOT THE LEVER. The ROM computes the row pointer as `lsls r3,r5,#2; adds r3,r3,r1` -- the INDEX owns the destination -- while the draft emits `lsls r0,r5,#2; adds r1,r1,r0`, base-owns. Splitting the base into its own local and adding in integer space (`base = gUnknown_0202FDE4; dst = (u16 *)(i * 4); dst = (u16 *)((int)dst + (int)base);`) reproduces the ROM's operand order EXACTLY and keeps the `ldr` in the ROM's position -- and scores 86.9%, one byte WORSE, with dst still in r1. So W73-G's `pointer_int_sum canonicalises every pointer/integer addition` note is true only of a SINGLE expression; two statements in integer space do reach the operand order. It just does not decide the register.
- WAVE 77 (W77-K) VERDICT: THE RESIDUAL IS A TWO-REGISTER PERMUTATION, r1 AND r3 SWAPPED THROUGHOUT THE LOOP BODY, AND NOTHING ELSE DIFFERS. ROM dst->r3 and multiply/chain temp->r1; draft dst->r1 and temp->r3, with the consequential `movs r7,#0` / `movs r3,#0` and `ldr r7,[pc,#104]` / `ldr r3,[pc,#104]` scratch choices following from it. Same opcodes, same order, same count, same relocations, size exact. No constant, no operand order, no width and no statement position is left in it.
- Binding the sin value to a named local (`s = gSinLut[...]`) -- byte-identical, agbcc coalesces it anyway.
- TOOLCHAIN. data/compiler-overrides.json has no entry in 0x08073 and sub_08073714 matched on the default toolchain this wave.
- 'The permuter cannot fix this' -- REFUTED, see settled_by_this_attempt.
- WAVE 73 (W73-D): SECOND PERMUTER ROUND, 300 s / 4 threads, started from the 76.2% best.c exactly as the wave-59 repeat-from-best.c method prescribes. It found NOTHING -- best candidate 58.2%, well below the starting point, draft restored unchanged. The wave-46 note 'the first run was still improving when its 300 s expired' did not survive a second round, so the repeat-from-best.c method is not automatic: it worked on sub_080344F0 this wave and did not work here.
- WAVE 73 (W73-G): A FRESH VARIABLE FOR THE PRODUCT IS NOT THE LEVER, which sharpens W73-D's own chapter. Declaring `int c;` and assigning the product to it in exactly the position W73-D used for `b`, in both stores, LOSES BOTH `adds rD, r0, #0` copies and comes out 4 bytes SHORT -- the same failure as binding the sin value to a named local. The dead in-expression assignment only works when the target pseudo ALREADY CONFLICTS with the multiplicand's; `b` carries an earlier live range in the same iteration, `c` carries none, so regmove coalesces it away. Measured by compile_probe. The chapter in docs/agbcc-codegen.md has been amended.
- WAVE 73 (W73-G): THE RESIDUAL IS NOW A PURE REGISTER-NAME ONE and should be read as a member of that class. With W73-D's lever in the draft the instruction stream is 1:1 with the ROM -- same opcodes, same order, both copies present -- and all 31 bytes are register numbers: ROM b=r0 / product=r1 / dst=r3 against the draft's b=product=r3 / dst=r1. The draft's `b` reuse buys the copies but FUSES b and the product into one pseudo, and that is what pushes dst out of r3. The lever wanted is one that gives the product its own pseudo while still blocking coalescing.
- WAVE 73 (W73-G): `dst = (u16 *)(i * 4 + (int)dst)` to force the index into operand 1 of the address add (the ROM has `adds r3, r3, r1` with the index first, the draft `adds r1, r1, r0` with the base first) -- BYTE-IDENTICAL to the draft. c's pointer_int_sum canonicalises every pointer/integer addition to PLUS(ptr, int) regardless of source order, so source operand order is not reachable this way. And `dst -= -(i * 2)` (W73-B's subtraction-of-a-negation) is much WORSE: it spills the negated index to a stack slot (`sub sp,#0x8`, `ldr r7,[sp,#0x4]`, `sub r1,r1,r7`) inside the loop. Both measured by compile_probe.
- WAVE 73 (W73-G): THE 91.4% IN best.json IS NOT REACHABLE FROM ANY DRAFT, and this cost two agents time. best.c's function body is BYTE-IDENTICAL to the 87.3% draft, yet scores higher, because best.c holds permute.py's RAW cpp-EXPANDED output and the expanded unit compiles differently from the same text behind `#include "global.h"`. A permuter run this wave printed both numbers for one candidate: 87.7% spliced onto the original preamble, 91.8% raw expanded. Always compare on the SPLICED number. Full write-up in docs/agbcc-codegen.md; the harness discrepancy itself is unexplained and is worth a coordinator's attention.
- WAVE 73 (W73-G): decomp-permuter THIRD round, 300 s / 4 threads, from best.c. Its one candidate is worth +0.4 points spliced (87.3 -> 87.7) and its mutation is SEMANTICALLY WRONG in the W73-E sense: it assigns the store value to `i`, the loop counter, which is then incremented -- `i = (((b = sin * unk34) >> 20) + (unk34 >> 16)) + g; dst[0] = i;`. Not adopted. The readable half of it is that forcing the SUM into a pseudo that is live elsewhere splits it from the chain register, which is the same mechanism as the `b` trick and points at the same open question.
- WAVE 73 (W73-D): REORDERING THE THREE ADDENDS of each store so the global comes FIRST (`gUnknown_0300200C + (unk34 >> 16) + (sin * unk34 >> 20)`), on the theory that referencing the global earlier would make its pseudo win r0 and push the multiply chain into a fresh register. MUCH WORSE: 46.7%, first difference moved BACKWARDS from +0x52 to +0x28. It hoists the global's ADDRESS into sl in the preheader, where the ROM rematerialises `ldr r7,[pc,#104]` inside the loop every iteration, and it reorders the literal pool (gUnknown_03002000 and gUnknown_0300200C swap slots). The addend order is NOT the lever on which register the multiply chain lands in. Draft restored from snapshot.
- WAVE 73 (W73-D): a fresh `int t; t = <dst[0] expression>; dst[0] = t;` is BYTE-IDENTICAL to writing it inline -- same coalescing as the named-local case. Only an assignment to a variable that already exists and is dead moves it.
- WAVE 73 (W73-D): THIRD PERMUTER RUN, from the new 87.3% best.c. It reported 91.4% and then 91.8% AND BOTH BODIES ARE SEMANTICALLY INVALID: the permuter reused `i`, THE LOOP COUNTER, as the temporary for dst[0]'s value (`i = <expr>; dst[0] = i;`), destroying the induction variable so the loop no longer runs 0..0x9f. Corrected honestly with a fresh temp it is byte-identical to the 87.3% draft, so the mutation was worth nothing either way. CORRECTED BY W73-G SAME WAVE: the 4.5 points were NOT the corruption -- the same candidate scored 87.3% SPLICED onto the preamble and 91.4% as RAW EXPANDED output, so the gap is the header-expansion artefact W73-G characterises, not the invalid body. Two independent defects: the body was invalid AND the score was unreachable from any draft. Compare permuter candidates on the SPLICED number. best.c and best.json have been overwritten with the valid draft (they read 91.8% and would have propagated a false score into the next wave); the invalid sources are preserved under work/sub_08073480/permuter/. This independently corroborates W73-E's wave-73 finding that the randomizer is not always semantics-preserving and that the invalid results tend to be the HIGHER-scoring ones. Audit any non-matching best.c before installing it as a draft; a byte MATCH needs no audit.
- W83-D: SEVEN-PROFILE SWEEP completed on the W77-K draft: configured == default 31 of 244 (87.3%); o1 216, o1-no-force 208 (size-mismatched); no-force / old-agbcc-no-force 43.9%; old_agbcc is SIZE-EXACT 244/244 with 30 of 244 differing (87.7%) -- ONE BYTE better than configured but not a byte match, so it does not justify a compiler-overrides.json entry on its own (an override needs an actual match). No toolchain configuration closes this function.

### Settled

- THE PERMUTER DOES HELP HERE. The previous attempt parked this as 'two instructions SHORT rather than slot-wrong, which the brief says the permuter cannot fix' and never ran it. One 300 s / 4-thread run took it from 50.4% to 76.2% and moved the first difference from +0xe to +0x52. A register-allocation residual that manifests as MISSING COPIES is still a register-allocation residual; the brief's exclusion is for a residual of one EXTRA instruction, which this is not.
- The head reaches gUnknown_0202FDE4 through agbcc's own -fforce-addr .rodata word at 0x081CC024 while the loop and the DMA tail use a plain inline pool word; the honest spelling reproduces both levels. PROMOTION MUST CARRY "rodata": ["0x081CC024"].
- `dst = gUnknown_0202FDE4; dst += i * 2;` as two separate statements is required -- one expression sinks the `ldr` past the index computation.
- `*((b & 0xff) + gSinLut)` rather than `gSinLut[b & 0xff]`: the index-first spelling creates the 0xff pseudo before gSinLut's, which is what decides that 0xff wins r8 and gSinLut gets ip.
- WAVE 73 (W73-D): READ A PERMUTER WIN AS A MECHANISM, DO NOT JUST KEEP IT. The wave-46 run produced TWO mutations and they are not equal. The `b = ...` assignment inside dst[1]'s expression is the real mechanism, and applying the same trick to dst[0] BY HAND -- which nobody had done because the mutation looked like noise -- is worth 76.2% -> 87.3%. The other mutation, `(i + 1) <= 0xa0` for `i < 0xa0`, is a pure REGRESSION costing `adds r0,r5,#1; cmp r0,#160` where the ROM has a bare `cmp r5,#159`; it was padding a candidate that was 4 bytes SHORT, so the earlier size-exactness was partly fake. 'Do not tidy a permuter win' does not mean the whole diff is load-bearing.

### Why it is parked

A pure register-name residual on a 1:1 instruction stream. The type model, the statement order, the -fforce-addr two-level access to gUnknown_0202FDE4 and now both multiply copies are settled. Three permuter runs have not reached the register naming and the third produced only invalid code.

### Wave 87

WAVE 87 (W87-F, do{}while(0) transfer test): NO EFFECT on the residual, control case of the batch -- clean negative, 0 try_match, draft unchanged (87.3%, 244/244, first difference +0x52). Five placements (both stores; one per store; whole loop body inside the for; the a/b accumulator pair; the `dst = g; dst += i*2` pair): NOT ONE moved r1/r3. Every placement that did anything moved the same four global.c allocnos (0xff, gSinLut, unk34>>16, unk2c>>16 in r8/ip/sl/sb) -- and the baseline ALREADY holds the ROM's four hi registers, so each wrapper moved them the wrong way. Mechanism: the wrapper raises the loop-depth weighting of REG_N_REFS, an input to allocno_compare in global.c; `dst` and the chain temp are born and die inside one basic block with no call and are handed out by local-alloc/reload, which runs after global.c and never reads allocno priorities. THE LEVER'S REACHABLE SET IS VALUES WHOSE LIVE RANGE SPANS A LOOP OR A CALL (the r4-r7 / r8-sl values); a permutation of low scratch registers inside one block is outside it. Next (unchanged from W77-K): a permuter run from the CURRENT 87.3% draft (the wave-73 run started from the 76.2% best.c; the draft has moved since). Do not spend another do/while probe here.

</details>
