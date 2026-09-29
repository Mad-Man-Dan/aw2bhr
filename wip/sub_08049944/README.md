# sub_08049944

0x08049944, 180 bytes, THUMB, parked.

Best score so far: 92.2%.

## What it does

Draws the number a1 at (a2, a3) with sub_08014B0C when a4 is non-zero, after calling sub_08012BC8 on a 6 by 2 area starting 5 columns to the left (probably clearing it); the zero and non-zero cases are separate calls. It also counts a1's decimal digits in a loop whose result is never used, then calls sub_08013AFC.

## How close it is

Compiles to the right size (180 bytes); 14 bytes differ (92.2% line up).

## What is left

The draft copies a1 inside the first `if` (`if ((new_var = a1) == 0)`), which gets the original's register assignment but costs one extra copy; to pay for it the draft uses a different scratch register for the two stack arguments, which is the other half of the difference. A spelling that gets the same register assignment without the copy would close it; nothing tried so far does.

## Already tried

- No copy at all (six variants: duplicated tails, both `if (a4)` polarities, a goto, 0 instead of a1, `a1 - a1`): a1 and a4 swap registers.
- The copy as its own statement before the `if`: 12 bytes too long, 5%.
- Changing local declaration order or statement order: byte-neutral.
- Writing the two draw calls differently from each other: byte-neutral.
- Reaching the text buffer through a pointer local: 4 bytes too long, 8.9%.
- The automatic permuter, four runs (about 30 minutes): found the copy, then plateaued.
- All seven compiler profiles: identical output.

## Files

- `sub_08049944.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

92.2% identical, SIZE-EXACT (180/180), 14 differing bytes, first difference at +0x22. RE-MEASURED wave 77 (W77-K).

### What still differs

*** THE a1/a4 HARD-REGISTER SWAP IS GONE -- THIS ENTRY'S PREVIOUS remaining_diff WAS STALE. *** In the wave-77 diff the candidate and the ROM agree on every parameter (a1->r6, a2->r7, a3->r8, a4->r5, a5->r4) and both emit `cmp r5,#0` for a4 and `cmp r6,#0` for a1. W73-B's live-range split fixed the swap outright. The remaining 14 bytes are TWO COMPENSATING ERRORS worth +2 and -2, which is why the size stays exact. (1) THE SPLIT COSTS ONE INSTRUCTION THE ROM DOES NOT PAY: the ROM has NO split, using r6 directly at all five of a1's uses (`cmp r6,#0`, `cmp r6,#10`, `cmp r6,r1`, `str r6,[sp,#4]`, `adds r3,r6,#0`), where the draft emits an extra `adds r3,r6,#0` after `bl sub_08012BC8` and tests `cmp r3,r1`. (2) THE ROM SPENDS r2 ON THE TWO STACK-SLOT CONSTANTS AND RELOADS a3: `movs r2,#2 / str / movs r2,#0 / str / mov r2,r8 / movs r3,#6` against the draft's `movs r3,#2 / str / movs r3,#0 / str / movs r3,#6`, which finds a3 still in r2 from the prologue's `mov r8,r2` and never pays the reload. These are ONE fact: r3 is unavailable as the stack-arg scratch precisely because the split parks new_var there. A spelling that reproduces the ROM's register assignment WITHOUT a split closes the function -- both errors vanish together.

### Why it is close

The priority model does not explain the ROM and the mismatch is not marginal. a1 has 6 refs to a4's 4, one of a1's refs is INSIDE the loop (global.c weights by freq, ~10x per loop level) and a1's live range is the longer -- every term of floor_log2(n_refs)*n_refs*freq/live_length puts a1 ABOVE a4, which is the candidate's order, not the ROM's. A plain REG_ALLOC_ORDER readout is inconsistent in both directions too: ascending would give r4 to the highest-priority allocno (a5, the lowest by refs), descending would give r7 to a2 over a1's loop-weighted refs. So find_reg is deciding on conflicts and copy preferences, not priority order. This is the SECOND independent failure of that model in this region -- sub_08045C18, two functions away, fails it the same way.

### Already ruled out

- WAVE 77 (W77-K): SPLITTING AT THE CALLS INSTEAD OF AT THE LOOP IS CATASTROPHIC. `new_var = a1;` as its own leading statement, `a1` in the loop test and `new_var` as the sub_08014B0C argument gives 192 bytes (+12), 5.0% identical, first difference at +0x2: it forces a FIFTH callee-saved register (r9 for a5), an extra `mov r3,r9 / str r3,[sp,#0]` at both call sites and an `adds r4,r6,#0` in the prologue. The split must be the ASSIGNMENT INSIDE THE `if` CONDITION; a leading copy statement is a different and much worse construct. This sharpens W73-B's finding, which recorded the win without recording that its placement is load-bearing.
- WAVE 77 (W77-K): LOCAL DECLARATION ORDER IS BYTE-NEUTRAL (measured on sub_0801F234 and sub_08052BBC the same wave). Not a lever here or anywhere.
- decomp-permuter CONFIRMED PLATEAUED at 92.2% for the third time: wave 73 (W73-F) ran a further 600 s from this best.c and it reported 'no candidate scored better than the starting point' -- the internal base score 225 was never beaten. Four runs now (300 s, 300 s, 600 s, 600 s). This one is converged and further permuter time on it is waste.
- The two sub_08014B0C call sites as DIFFERENT source. The ROM's first arm passes 0 in r3 and a1 at [sp,#4], the second passes a1 in r3 and 0 at [sp,#4]; writing that asymmetry is BYTE-NEUTRAL, because cse knows a1 == 0 on the first arm (the branch just proved it) and substitutes the register for the immediate freely. Both spellings compile to the identical 180 bytes, so the swap cannot be evidenced in either direction.
- All six wave-43 spellings of the `a2 - 5` argument -- see settled_by_this_attempt; they are the reassociated form and are now understood, not open.
- decomp-permuter: 180 bytes is below the 256-byte floor, and wave 17 measured that a permuter run returns nothing on a pure allocation residual whose instruction order is already correct, which is exactly this one.
- decomp-permuter is NOT ruled out here, and the previous entry's reasoning was wrong. It claimed the 180-byte size is 'below the 256-byte floor' and cited wave 17. Wave 73 ran it three times (300 s from the draft, then 300 s and 600 s from best.c): run 1 found the live-range split worth 40 points. The 256-byte figure is a 'run it before hand-rewriting' threshold, not a floor below which the permuter does nothing.
- Toolchain axis: all SEVEN compiler profiles produce BYTE-IDENTICAL output on the old draft -- configured, default, no-force, o1, o1-no-force, old-agbcc and old-agbcc-no-force all give exactly 70.0%, 54 of 180, size-exact. This function's register assignment is invariant to every flag the harness can set, which is the strongest such result in the batch.
- decomp-permuter has now PLATEAUED at 92.2%: two further runs (300 s and 600 s) starting from this best.c found no candidate better than the starting point.
- W73-E's row-3 pointer-global spelling (`u16 **t = &gUnknown_0849957C;` passing `*t` at the three call sites): NEGATIVE and much worse -- 184 bytes (+4), 8.9% identical, first difference at +0x2. Consistent with W73-E's own diagnostic, which indicates row 3 only when the ROM loads the pool word EARLIER than the candidate; both of this function's pool words already match the ROM.
- WAVE 80 (W80-D): THE W80-C REF-COUNT LEVER DOES NOT REACH IT. Six no-split spellings measured by compile_probe, every one keeping a1 in r5 and a4 in r6 (the swap): the plain draft without the split; `sub_08013AFC(); return;` written out in the a1 == 0 arm (a duplicated tail for cross-jumping to fold); the tail written under both polarities of `if (a4)`; `goto done` past the second call; `sub_08014B0C(a2, a3, g, 0, a5, 0)` in the a1 == 0 arm, which removes one of a1's refs outright (cse still emits `str r5,[sp,#4]` from the register it knows is zero, so the bytes are neutral); and `a1 - a1` for the sixth argument. No written-out tail in this function references a1 or a4, so there is no ref count for the lever to change. Bare allocno tie, stopped at two attempts per the wave-80 rule; the split draft stays as the closer measurement.

### Settled

THE ARGUMENT IS `(u16)a2 - 5`, WITH THE CAST ON THE OPERAND -- worth 8 bytes and the whole size error, and it REFUTES the docs chapter '(u16)(narrow - K): SIX spellings all reassociate, and only a basic-block boundary blocks it' (wave 43, W43-L), which declared this unreachable from the source. a2 is live across basic blocks, so the prologue has already materialised it zero-extended in r7; `(u16)a2` therefore costs nothing and the subtraction is a plain SImode `subs r1,r7,#5` with no shift for combine to distribute into. `(u16)(a2 - 5)` -- the cast on the RESULT -- is the reassociated form and is NOT the same code. a2 stays s16 and still sign-extends at its two sub_08014B0C uses. Wave 43's six spellings were measured in a standalone compile_probe, where the parameter is used once, in one block, and is NOT pre-narrowed; every spelling collapses there. Re-measured in wave 59: the probe reproduces wave 43 exactly, so the measurement was sound and the inference from it was not. This is the wave-15 probe blind spot on the parameter side. The doc chapter now carries the refutation.

### Why it is parked

Residual kind 3 (allocation). NOT the a1/a4 swap any more -- see remaining_diff. Two compensating errors that are one fact: the live-range split buys the ROM's register assignment and pays for it with a copy parked in r3, which then costs the ROM's r2 stack-arg scratch and its `mov r2,r8` reload.

### Improved by this attempt

SPLITTING THE PARAMETER'S LIVE RANGE, found by decomp-permuter run 1 and semantics-checked afterwards. `if ((new_var = a1) == 0)` with the loop testing `new_var >= n` instead of `a1 >= n`, `u16 new_var` declared FIRST of the three locals. new_var is a pure copy of a1 that is never reassigned, so this is semantically identical to the old draft -- it only splits a1's live range so the calls read a1 and the loop reads new_var. Worth 40 points. This is the brief's 'binding locals are punctuation' lever applied to a PARAMETER, which the docs had only ever stated for address-arithmetic locals.

### Wave 89

WAVE 89 (W89-D): SCREENED OUT before any probe, and the wave-89 routing premise for it was wrong. The entry's 'the ROM reloads a3' is `mov r2,r8`, a HI-TO-LO REGISTER MOVE, not a memory re-read -- the ROM rebuilds r2 only because it picked r2 as the scratch for the two stack-slot constants and clobbered the prologue's copy. No memory is involved, so no re-read lever (volatile, static-inline helper) can act here. Separately, W86's bound DISQUALIFIES the live-range split this entry's remaining_diff is built around: a1 does NOT die at `(new_var = a1)` -- it is used at both later call sites -- so W73-B's split adds a FOURTH allocno rather than re-cutting a live range, and the copy it costs is not coalescable. Re-measured 92.2% size-exact by exit code. Evidence: work/sub_08049944/W89-notes.md.

### Wave 97

wave 97
Base: sub_08049944.c (== best.c, 92.22%, size-exact, first diff +0x22), unchanged.
Re-measured; the parked description holds (14 bytes: extra `adds r3,r6,#0` from the a1 split, and r3 vs r2 as the stack-constant scratch). Probes: (a) no split (plain `a1`) -> a1/a4 swap returns (a1 in r5, a4 in r6), i.e. the split is what fixes the swap; (b) same with `0` as the sixth arg of the zero arm -> identical swap. Two probes, nothing new; stopped per budget. Proposed left: "a1/a4 register pair only fixable with a split that costs one copy; ROM has none."

wave 97 (W97-AA)
Base unchanged (92.22%). Four spellings of which name (a1 / new_var) each of the four later reads uses, via spellings.py: any
read of `new_var` at a sub_08014B0C site (either arm) or `a1` in the loop test gives 192 bytes (+12, frame gains r6), because the
split half then spans the calls; only the current form (split read in the digit loop alone) is 180. No new axis.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.

</details>
