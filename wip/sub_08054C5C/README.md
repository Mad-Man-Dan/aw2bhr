# sub_08054C5C

0x08054C5C, 560 bytes, THUMB, parked.

Best score so far: 93.4%.

## What it does

Runs the setup chain for both sides of the two-side subsystem: it gathers each side's unit data, loads two sprite palettes, and calls a fixed sequence of setup functions for the current side and then, usually, for the other side.

## How close it is

Compiles to the right size (560 bytes); 37 of 560 bytes differ (93.4% identical), all in the setup loop at the top and all register choices.

## What is left

Get the setup loop's counter into the original's register from the start without the `t = i` copy (removing the copy makes the function 8 bytes too long), and get gUnknown_085D6A48, not gUnknown_085D6A52, held in a register across the loop. No source change found yet reaches either.

## Already tried

- Dropping the `t = i` copy: the counter and one stack address come out as in the original, but the function is 8 bytes too long later on.
- Reusing `u` as the setup counter: 8 bytes too long, 14.8% identical.
- A `do { } while (0)` around only the `a[t]` statement, to change which table address is created first: 4 bytes too long, 22.1%.
- An empty `gUnknown_08551E64[0][0] += 0` between the two table reads, to stop the compiler moving them out of the loop: 12 bytes too long.
- Block-scoped setup and result locals, and row or base pointer locals: worse frame and registers.
- The alternative compiler settings: identical, the wrong size, or 31.4% identical.
- Three permuter runs (about 42,000 tries, two undirected, one aimed at the declarations and the setup loop): only the `i = d[side ^ 1]` argument reuse helped (92.5% to 93.4%, kept); a 93.2% result from an earlier run changed the loop's behaviour and was rejected.

## Files

- `sub_08054C5C.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 80 (W80-C), unchanged at exact size 560/560, 92.5% (configured), 42 differing bytes, first difference +0x13. Wave 80 measured the wave-78 `t = i` copy: it is a SIZE COSTUME -- without it the candidate is 568/560 (+8) but gets i into r6 and computes `&b[1]` from sp as the ROM does (the copy form derives it as `adds r0, #2` off `&b[0]`, which is what pays for the copy). The `((0, gUnknown_085D6A48))` comma anchor is byte-neutral (identical output with and without). True residual: the priority order of the `&d[0]` pseudo vs gUnknown_03004580 (ROM r8/r7, no-copy form r7/r8) and which of gUnknown_085D6A48 / _085D6A52 keeps r9 across the loop (ROM: A48; every draft form: A52) -- an allocno ordering with no construct found. Earlier: scoped setup/result locals and row/base bindings worsen the exact frame and allocation; the permuter's 93.2% assignment-in-condition candidate changes loop semantics and is rejected. ||| W83-D: THE WAVE-16 ZERO-TRIP DO/WHILE PROMOTION IS NEGATIVE HERE. Wrapping a zero-trip loop around ONLY the a[t] statement (to push gUnknown_085D6A48's pseudo past gUnknown_085D6A52's into sb as the ROM has it) scores 22.1% at 564/560 (+4), first difference +0xc IN THE PREHEADER: the promotion takes the whole a[t] address chain with it, including gUnknown_03004582's inner reference, and disturbs the stack-base spills. It promotes more than its target -- unlike c_0804D290 where the anchored expression owned one base only. Wave-80 fixpoint restored after the probe. SEVEN-PROFILE SWEEP: default == configured 42/560 (92.5%); no-force 548, o1 552, o1-no-force 548, old-agbcc-no-force 544 (all size-mismatched); old-agbcc 31.4%. No toolchain lever.

WAVE90: WAVE 90 (W90-A): 92.5% -> 93.4% (37 of 560, size-exact, first difference +0x13): permuter undirected run found `t = sub_08055058(a[side], b[side], c[side], i = d[side ^ 1]);` (the dead loop counter reused as an argument temp -- semantics-preserving; draft = w90-perm1-934.c). Chained undirected run 16,819 it: best permuter score 2300 is 69.3% by bytes (rejected). Run 3 DIRECTED (w90-directed.perm.txt: PERM_LINESWAP over the declarations + PERM_RANDOMIZE over the setup loop) 13,564 it from the 93.4% base: nothing better than 2420. Residual unchanged from W80: counter born in r3 and copied to r7 (the wave-78 `t = i` size costume; ROM r6), the sp+10 spill derived `adds r0,#2` off sp+8 instead of from sp, table base r6/sb=gUnknown_085D6A52 vs ROM r7/sb=gUnknown_085D6A48. Notes: work/sub_08054C5C/NOTES.md.

### Wave 97

wave 97 (W97-G)
Base: `sub_08054C5C.c` (== w90-perm1-934.c), 93.39% size-exact, first diff +0x13. Unchanged.

Pre-registered hypothesis (a shared side/other-side index recomputed by the ROM) did NOT hold: the whole residual is
in the setup loop preheader (nothing after +0x13 differs except register names from the r6/r7 swap), not in the
call groups.

Probes (trymatch):
- Drop the `t = i` copy (index `i` directly) while KEEPING `i = d[side ^ 1]` as the 4th argument: 560/560
  size-exact but 75.5%. Preheader gets `movs rN,#0` for the counter in r7 (ROM r6), keeps `adds r0,#2` for sp+10,
  and the tail argument costs `ldrh r7; adds r3,r7,#0` (ROM loads straight into r3). So the `i =` reuse is what
  makes the no-copy form size-exact, but it adds a copy the ROM does not have.
- No-copy without `i =`: 568/560 (+8), 16%. With `u = d[side^1]` instead of `i =`: 568, 16.4%. Only reuse of the
  dead loop counter keeps the size; a fresh pseudo does not.
Conclusion: unchanged allocno-order residual (counter reg, sp+10 derivation, A48/A52 in r9). No new lever.

Proposed summary: does = builds per-side setup tables for two sides, then runs the setup chain for the active side
and the other side if they differ. status = 93.4% size-exact, all differences in the loop preheader register
assignment. left = counter in r7 not r6, sp+10 derived off sp+8, wrong table base in r9. tried = see above plus
waves 78-90.

wave 97 (W97-AA)
Base unchanged (93.39%; draft snapshot `sub_08054C5C.w97aa-start.c`). Tried this wave's copy-back step in place of the `t = i` costume:
- `for (i = 0; i < 2; ) { ...use i...; nx = i + 1; i = nx; }` with the `i = d[side ^ 1]` tail reuse: 560 size-exact but 69.29%
  (first +0x13). The counter is a plain u16 with no `t = i` copy, and the setup loop is now the ROM's shape, but the counter lands in r7
  and the table base in r6 (ROM r6 / r7), `&b[1]` is still derived `adds r0,#2` from `&b[0]`, and the tail loads `ldrh r7; adds r3,r7,#0`.
  So the copy-back step gives the size the costume gave, without the costume, and leaves exactly the counter/base allocno swap.
- The same with `t = i` kept (copy-back + costume): 93.39%, byte-identical to the draft. Step at the top (`i = nx` in the for clause): 564 bytes.
- Replacing `i = d[side^1]` in the tail with `nx = ...`, `(t = ...)` or the bare read: 572 / 572 / 568 bytes, 15%: only reuse of the loop
  counter keeps the size, as before.
Proposed left: counter r7/base r6 swap in the setup preheader (allocno order), sp+10 derivation, tail copy.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.

</details>
