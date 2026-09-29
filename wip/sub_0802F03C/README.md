# sub_0802F03C

0x0802F03C, 512 bytes, THUMB, parked.

Best score so far: 93.2%.

## What it does

Resets the link-cable communication state. It clears the counters and flags in the link record gUnknown_0849B018, sets the per-player tables in gUnknown_0849B01C to 0xFFFF, zeroes several 4- and 128-entry tables, clears the 48 packet buffers (0x88 bytes each), and empties the send ring (512 entries) and the four players' receive rings (1024 entries each) along with their cursors.

## How close it is

Compiles to the right size (512 bytes); 35 bytes differ (93.2% identical). No instruction is missing, extra or out of order: the two loop counters i and j sit in each other's registers throughout, plus a few temporary-register choices that follow from that.

## What is left

Make the compiler give i and j the opposite registers. Its choice is a tie between the two counters and no known source change breaks it, so this needs a new idea rather than more rewording.

## Already tried

- Changing the declaration order, scope and which loops share a counter: the swap stays.
- Making either counter unsigned: the counting-down loops change shape and the function shrinks to 324 bytes.
- A separate counter just for the 16-buffer loop: that loop and the one before it both change form and the function grows to 516 bytes.
- Pinning the counters to fixed registers: adds entry checks and is worse.
- Chained permuter runs: improved 46 to 35 differing bytes, then stalled. The odd `new_var`, `new_var2` and empty do/while in the draft come from these runs.
- Every other compiler setting: wrong size (496 to 516 bytes).

## Files

- `sub_0802F03C.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

WAVE 79 (W79-E) RE-MEASURED BY EXIT CODE: exact 512/512, 93.2%, 35 differing bytes, first difference +0x57. Unchanged from wave 74 -- the number the coordinator quoted is current.

### What still differs

Re-read instruction by instruction in wave 79 with the branch-label noise filtered out. It is exactly what wave 74 said and nothing else: a pure r4/r5 EXCHANGE running the whole length of the function (`movs r4,#0` vs `movs r5,#0`, `movs r5,#63` vs `movs r4,#63`, every `adds rN,#1`/`cmp rN,#3`/`cmp rN,#127`/`subs rN,#1` pair, and the two `ldr rN,[pc,#N]` bases that follow them), plus three reload scratch picks that ride on it: `ldr r7`/`ldr r6` against `ldr r6`/`ldr r3`, and the loop-invariant `adds r3, r5, #1` (the ROM's saved i+1) landing in r0 and then r1 in the candidate. NO INSTRUCTION IS MISSING, ADDED, OR OUT OF ORDER.

### Why it is close

All ten loops and total size match; Wave 74 reproduced the strongest valid candidate.

### Already ruled out

- Declaration, scope, signedness and loop-counter ownership probes did not close the swap.
- Clean chained --current permuter searches were drained with no match.
- WAVE 79 (W79-E): NO SOURCE CONSTRUCT SITS BEHIND THIS RESIDUAL. Re-classified from the diff under the wave-77 discriminator -- there is nothing whose respelling or deletion would change the SET OF PSEUDOS the allocator is handed. Every differing byte is a register number field of one allocno-priority tie between the two loop counters, plus scratch picks downstream of it. This is the 'exact size plus a one-register rotation is an allocno-priority tie' class. No probes were spent, by design; the classification is the result.
- W83-D: diff CONFIRMED CURRENT BY EXIT CODE (93.2%, 35 of 512, first +0x57) and SEVEN-PROFILE SWEEP completed: every temporary profile is size-mismatched (no-force 516, o1 500, o1-no-force 496, old-agbcc 516, old-agbcc-no-force 516 vs 512) and none better. configured remains canonical. No probes spent on source spellings, per the W79-E NO-CONSTRUCT directive.

### Settled

- The Wave-72 fourth-loop counter fix is load-bearing; do not rework loop structure.

### Why it is parked

Wave 74 W74-C, re-confirmed wave 79 W79-E. Resume only with a new register-priority lever. Classified NO-CONSTRUCT: allocno tie plus reload scratch picks. Do not give this function a spelling budget again until such a lever exists.

### Wave 96

Base: existing draft (93.16%, size-exact), unchanged; copy `sub_0802F03C.w96-start.c`. The whole residual is one register swap in the
first `for (i < 4)` loop: the ROM has `i` in r4 and the inner `j` in r5, the draft the reverse (35 differing bytes, all register names;
the later loops then choose r4/r5/r3/r6 differently for the same reason).
Negatives, with mechanism:
- Declaring `int j, i;` instead of `int i, j;`: byte-identical (allocation does not follow declaration order here).
- Wrapping the whole outer loop in `do { } while (0)` (raise priority of the outer counter): 27.9% and +8 bytes; it changes which loops
  keep their counters in registers, not the i/j swap.
- Giving the first loop its own counters (`x`, `y`) and leaving the later loops on `i`, `j`: 21.3% and +4; the wave-56 rule (a counter
  shared with a later loop outranks a fresh one) is visible here, since separating them makes both worse.
- Permuter, one 600 s run from the draft: NO-IMPROVEMENT.
Proposed summary: does = resets the link record, receive rings and send ring; status = 93.2%, size-exact; left = i/j register swap in
the first loop (ROM i=r4, j=r5); tried = declaration order, do-while wrap, separate counters, one permuter run.

### Wave 97

wave 97 (W97-S)
Draft unchanged (93.16%). -da .greg: the two counters are pseudos 24 (refs 58, live 270, priority ~1.07) and 25 (refs 44, live 88, ~2.5); the higher-priority one (j, 25) takes r4 first, the ROM wants i first. Tried lowering j's priority by moving later loops to a fresh `k` (each alone and in pairs: loop 3, 4, 5, 3+4, 3+5, 3+4+5): 90.8-92.6% size-exact or 71.7% at -4 when loop 5 (the 0x3ff loop) is moved; none flips the first loop. `unk08[j][i]` for the [64] store (uses j==64 after the loop): +12, 28%.

</details>
