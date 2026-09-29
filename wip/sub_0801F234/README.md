# sub_0801F234

0x0801F234, 120 bytes, THUMB, parked.

Best score so far: 99.2% (best.c).

## What it does

Loads the tile graphics for sprite id `a` into the next free place in a tile area (records in gUnknown_0200F920), copying width x height tiles with CpuFastSet. It then records the id and advances the area's next-free-tile cursor.

## How close it is

Compiles 4 bytes too long (124 against 120). Every statement is present and in the original's order; the difference is register choice. The draft puts the tile count in a high register, which costs extra save, restore and move instructions, where the original uses the last free low register (r7). A smaller second difference, the original moving the graphics-source result into a spare register and back, partly cancels it.

## What is left

Find out why the compiler will not use r7 for the tile count, even though neither version needs a frame pointer. Once that is fixed, a known trick for the second difference (a dead assignment of the call result, which works on the sibling sub_0801F19C) can be applied.

## Already tried

- All four declaration orders of the locals, and swapping the order of the `tile` and `n` statements: identical code.
- Storing the graphics-source call result in its own local before the CpuFastSet call: identical, because the compiler merges the copy.
- Integer arithmetic for the CpuFastSet destination (the trick that matched sub_0801FD9C): identical.
- The older compiler build: right size but a worse match, because it merges a re-read of the list cursor that the original really does, and it still uses the high register.
- The dead-assignment trick on the call result (`i = sub_0801F444(a, i)`) applied now: 4 more bytes, because `i` stays live longer here than in the sibling.
- Different ways of writing the copy-size arithmetic: no change.
- A `do { } while (0)` around the tile-count statement (in best.c): same size, slightly more bytes line up.

## Files

- `sub_0801F234.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

124 bytes (+4), 31.7% identical. RE-MEASURED wave 77 (W77-K): unchanged, and both recorded facts hold exactly.

### What still differs

TWO INDEPENDENT REGISTER-ALLOCATION FACTS, which is why this scores worse than its sibling despite the same shape. (1) `n` LANDS IN r8 INSTEAD OF r7. Four values are live across both calls (a, e, tile, n); the ROM puts them in r5, r4, r7, r6 and pushes {r4,r5,r6,r7,lr}, while the candidate takes r4, r5, r6 and then reaches PAST r7 for r8, paying `mov r6,r8; push {r6}` in the prologue, `mov r8,r3` after the multiply, `mov r2,r8` at the use and `pop {r3}; mov r8,r3` in the epilogue. It also forces the extra copy after `muls`: THUMB `muls` cannot target a hi register, so the product takes a lo scratch and is moved out, where the ROM multiplies straight into r6. (2) THE SAME MISSING RESULT-PARKING PAIR as sub_0801F19C (`adds r3,r0,#0` / `adds r0,r3,#0`), which cancels 4 of the bytes fact (1) adds.

### Why it is close

Every statement is present and in the ROM's order, and the whole tail -- the unk05 cursor bump and the three separate re-reads of it -- reproduces. Its sibling sub_0801F19C is byte-exact up to the second call from the same derivation, and the promoted src/decomp/c_0801F1EC.c supplies the type model.

### Already ruled out

- WAVE 77 (W77-K): THE PARKING HALF NOW HAS A LEVER AND IT IS DELIBERATELY NOT APPLIED HERE. A dead in-expression assignment on the call result -- `CpuFastSet((void *)(i = (int)sub_0801F444(a, i)), ...)` -- produces the ROM's `adds r3,r0,#0` / `adds r0,r3,#0` pair and moves the 0x3ff mask into the ROM's register. Demonstrated on sub_0801F19C this wave. Applying it here would add +4 on top of the existing +4, because `i` is additionally live through `&gUnknown_0200F920[i]` so the pseudo merge is longer, and because residual (1) is worth five instructions against the pair's two. FIX THE r7/r8 ASSIGNMENT FIRST, THEN APPLY THE LEVER.
- WAVE 77 (W77-K): LOCAL DECLARATION ORDER IS BYTE-NEUTRAL. All four permutations of {e, i, tile, n} compile to identical bytes. This is the clean measurement of an axis several waves have speculated about; replicated the same wave on sub_08052BBC. Pseudo creation order from declarations does not reach register allocation in agbcc.
- WAVE 77 (W77-K): the r7-vs-r8 fact stated precisely from the diff -- ROM a/e/n/tile -> r5/r4/r6/r7 with push {r4,r5,r6,r7,lr}; draft a/e/tile/n -> r6/r4/r5/r8 with push {r4,r5,r6,lr} plus the r8 round trip. Note that agbcc reaching PAST r7 for r8 is its NORMAL behaviour -- sub_0801F19C's own ROM does exactly that -- so what needs explaining is why this ROM stops at r7, not why the draft does not.
- THE `lsls #5; lsrs #2` -> `lsls #3` FOLD IS NOT WHAT COSTS THE BYTES HERE, and three waves of drafts plus the docs/agbcc-codegen.md section "`nonzero_bits` range analysis is ONE residual wearing three faces" all say it is. Counted instruction by instruction: the ROM's `ands rX,rC; lsls rD,rX,#5; lsrs rD,rD,#2` is 3 insns and the folded `ands rX,rC; lsls rX,rX,#3; adds rD,rX,#0` is also 3 -- the saved shift is spent on a copy, because the folded form computes the count into a callee-saved register and must move it to the argument register while the ROM's two-shift form writes the first shift straight into it. Defeating nonzero_bits would therefore not close either function. Doc corrected in wave 39.
- Binding sub_0801F444's result to its own local before the CpuFastSet statement (`src = sub_0801F444(a, i); CpuFastSet(src, ...)`) -- BYTE-IDENTICAL to the nested form. gcc coalesces the copy, so this does not create the pseudo that would let the mask constant win r0. This was the obvious reading of the ROM's `adds r3,r0,#0` and it is wrong.
- Integer-space address arithmetic on the CpuFastSet destination (`(void *)((int)base + (v & 0x3FF) * 32)`) -- BYTE-IDENTICAL. This is the wave-39 lever that closed sub_0801FD9C, and it does nothing here: it only flips a commutative add whose operands are both non-constant registers, and these destination adds already have the ROM's operand order.
- old_agbcc with -fprologue-bugfix removed -- READ THIS BEFORE CHASING IT. It reports `size: match (120 bytes)`, which looks like a breakthrough and is not: it scores WORSE (28.3% against 31.7%), it still pays the r8 round trip, and it is size-exact only because it CSEs away the third `ldrb r0,[r4,#5]` re-read that the ROM actually performs after the cursor bump. Default agbcc reproduces that re-read and is the correct compiler for this function. The toolchain is not the axis.
- Every spelling of the count arithmetic -- retracted twice (waves 33 and 36) and byte-neutral regardless.

### Settled

- sub_0801F3D4 is called ONCE and its result bound: r1 stays live across all the address arithmetic and is sub_0801F444's second argument. Arguments are evaluated LEFT to right.
- `tile` and `n` are computed as statements BEFORE the CpuFastSet call because both are live across it AND across sub_0801F444; that placement is what the ROM's instruction order requires, not a stylistic choice.
- `tile` must be a local, not a re-read: the ROM loads `e->unk08[e->unk05].unk00` once into r7, and a second source read would land on a different entry because `e->unk05` is incremented in between.
- The ROM genuinely re-reads `e->unk05` from memory three times in the tail. Default agbcc reproduces this; old_agbcc does not, which is an independent confirmation of the compiler choice.
- The frame is NOT one live value too many, which was the wave-39 orchestrator's hypothesis for the +4. Both the ROM and the candidate keep exactly four values live across the calls; they differ only in WHICH registers, r7 against r8.

### Why it is parked

Register allocation, not source semantics, and it shares residual (2) with sub_0801F19C. Park and unpark the two together. Residual (1) looks like a REG_ALLOC_ORDER/cost effect rather than anything source-reachable: `n`'s uses are all mov/add-copyable so its preferred class is GENERAL_REGS, and gcc pays copies to keep it in a hi register rather than restricting it to LO_REGS. Draft at work/sub_0801F234/sub_0801F234.c.

### Wave 86

WAVE 86 (W86-F, vocabulary-twin axis): twin sub_0801F34C (src/decomp/c_0801F34C.c, producer/consumer mirror) is a TRUE SHAPE TWIN and its lever (accumulate into an already-live DEAD PARAMETER) has NO HOST here: `sub_0801F234(int a)` has one parameter and `a` is live to the third statement from the end; `e`, `tile` and `n` are fresh locals by necessity (tile must be a local -- e->unk05 is incremented before the re-read; n is used twice). This is W81-D's wall on sibling sub_0801F19C stated from the other side ('the target must carry an ALREADY-LIVE range; freshness defeats the lever'). One probe: STATEMENT order of `tile` and `n` (n moved ahead of tile) -- REFUTED, byte-neutral, the allocation is identical (a->r6, e->r4, tile->r5, n->r8, r8 round trip and all); statement order joins declaration order (W77-K, four permutations) as byte-neutral for allocno numbering here -- do not spend a probe permuting independent statements to move a register. Next candidate named: remove one pseudo from the set live across the two calls, and the remaining candidate is `i` (live from sub_0801F3D4's return through &gUnknown_0200F920[i] into sub_0801F444(a, i); in the ROM it rides the r1 scratch for free over that span, per W77-K on the sibling). Configured, 124/120 (+4), 31.7%, unchanged; no try_match spent.

### Wave 89

WAVE 89 (W89-D): SCREENED OUT before any probe. Residual (1) is a register assignment and residual (2) parks a call result; the one genuine memory re-read (`e->unk05`) is ALREADY REPRODUCED and sits in settled_by_this_attempt, not in the residual -- so the re-read lever family has no target here. W77-K's stated reason for withholding the parking lever was re-checked and still holds. HANDOFF FOR THE NEXT ATTEMPT: neither build has a stack frame, so r7 is not reserved as a frame pointer in either, yet the draft pays four instructions for an r8 round trip (plus a fifth to move the `muls` product out of a lo scratch) with r7 SITTING UNUSED. The question is what makes r7 unavailable to that compilation, not why `n` prefers r8. Evidence: work/sub_0801F234/W89-notes.md.

### Wave 96

Base: sub_0801F234.c unchanged (30.65%, size+4). Probes (structural instruction diff against the ROM, 16 lines for the draft): `tile` as int/u32, `n` as int/u16 and folding the `i` local into `e - gUnknown_0200F920` (which is worse, 26). Every retyping was byte-identical to the draft. Un-binding does not have a host: `tile` is read before the calls and the slot it reads is rewritten (e->unk05 is incremented) before its second use; `n` is one multiply used twice and the ROM has a single `muls`; removing `i` adds a subtraction and a shift. Nothing found for r7 vs r8 (see the wave 89 note in the source).
Permuter (600 s, --current): 'improved' 30.65% -> 46.67% and size-exact, but the kept file is WRONG C (kept as sub_0801F234.w96-perm1-WRONG.c): it turns `n` (the product of two table bytes) into a copy of `e->unk05`, loses the multiply, and rewrites the final store as `unk08[n].unk00 = tile; unk08[n].unk00 += n`. The score rose because the changed statements happen to line up the register pool. Draft restored. Pre-registered un-binding: nothing to un-bind (see above); the delta -1 copy remained.

Proposed summary: does = queues one tile upload (CpuFastSet) for a picture slot and appends its tile range to the slot's list; status = 30.65%, 4 bytes long; left = ROM keeps the four values live across both calls in r4-r7 while the draft spills one to r8 (extra push/pop and a mov after the multiply) and does not park sub_0801F444's result; tried = retyping tile and n, folding i, statement order (wave 86), permuter (wrong C only).

</details>
