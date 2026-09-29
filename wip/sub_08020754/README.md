# sub_08020754

0x08020754, 208 bytes, THUMB, parked.

Best score so far: 44.2% (best.c).

## What it does

Fills a map-sized byte buffer with 0 or 1 per tile: 0 where the terrain is wood (4) or reef (0x13) and the tile has no unit or a unit whose type is outside 0x10-0x14; 1 everywhere else.

## How close it is

The retained draft compiles 8 bytes too long and matches 38.46% of the bytes. Its branch layout still differs from the original; the earlier draft was 12 bytes too long.

## What is left

Find how the original wrote the if/else so that the compiler's common-subexpression pass reaches the '= 1' branch and not the '= 0' branch. The RTL dumps show it currently reaches '= 0' by skipping around the unit-type check.

## Already tried

- The map through the shared struct Map or gMap: identical output (the draft already used members).
- Negated tests, nested ifs, else-if chains, goto labels for either branch, continue in the '= 0' branch: no match; the closest is 8 bytes too long with the branches in the wrong order.
- Binding the tile index to a local: removes too much register pressure.
- A 40-minute permuter run (earlier wave): no match.

## Files

- `sub_08020754.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

220 bytes against 208 (+12), 27.9% identical, first difference at +0xc. Re-measured wave 58 (W58-B) and identical to wave 41's number to the byte.

### What still differs

WHICH ARM CSE RECOMPUTES THE INDEX ON, and nothing else. Everything outside the two store arms is byte-exact: prologue, both loop guards, the `sub sp,#4` spill of `y + 1`, the three hoisted bases (rowOffset[y] at +0x417A, cell at +0x1432, unit at +0x51A), the whole predicate, and both loop bottoms. The ROM recomputes `MAP->rowOffset[y] + x` from scratch -- reloading gUnknown_08499590 and re-adding 0x417A and y*2 -- in the `= 0` arm and reuses the loop-top value in the `= 1` arm. The candidate does the exact opposite. Two consequences, both mechanical: (1) the recomputing arm then sits AFTER the literal pool instead of before it, so a SECOND gUnknown_08499590 word AND a second 0x417a word are emitted at the tail, +8 bytes -- read it straight off try_match's relocation list, the ROM has one of each and the candidate has two; (2) the address and the value land in swapped hard registers in the two arms, which blocks the final jump pass from cross-jumping the common `strb r1,[r0]`, +2 bytes and +2 for alignment. 8 + 2 + 2 = the whole +12.

### Why it is close

gcc's cse_end_of_basic_block follows a conditional jump only when the target label has ONE use and is preceded by a barrier. Here the `= 1` label has TWO uses (`bne` off the 0x13 test and `bls` off the range test), so CSE cannot follow it, stays on the fall-through, and the `= 0` arm keeps the value. The ROM's CSE clearly DID follow into the `= 1` arm, which means that at CSE time its label had a single use -- i.e. the original source had two SEPARATE `= 1` blocks that only merged later. The type model is settled and shared with sub_080206E4 (matched): width/height u16 at +0/+2, the u8 unit-id plane at +0x51A, the u8 terrain plane at +0x1432, the u16 row-offset table at +0x417A, all indexed `rowOffset[y] + x`.

### Already ruled out

- `int i` bound at the top of the x loop: NO spill and gUnknown_08499594 hoisted, both wrong -- the bound local removes a whole register's worth of pressure.
- The condition negated with the arms swapped (`... != 4 && ... != 0x13 || ...`, `= 1` first): cross-jumps correctly but then recomputes in BOTH arms and spills 8 bytes.
- `int y, x;` versus `int x, y;` declaration order: byte-identical.
- NESTED ifs with the `= 1` store duplicated in the outer else -- the shape that SHOULD give the `= 1` label a single use at CSE time. Gets the cross-jumped `strb` AND dst in a low register, both right, but the two `= 1` blocks are merged by the FIRST jump pass, before CSE ever runs, so the label has two uses again and neither arm recomputes. 186 bytes, 22 short. This is the closest anything has come and it is why the residual is classified as basic-block layout rather than as a spelling.
- The two `= 1` arms as an `else if` chain: puts `= 1` first in the layout, and the ROM's `= 0` arm is first.
- `continue` in the `= 0` arm: gives that arm its own `strb` plus a branch to the loop increment, where the ROM shares one `strb` with the `= 1` arm.
- `(u32)(unk00 - 0x10) > 4` in place of the `(u8)` cast: drops the two shifts the ROM has. The `(u8)` spelling is settled.
- WAVE 61: permuter 2400s / 4 threads from the work draft (216 bytes, +8, 23.1%). The search DID improve on its seed, reaching 42.8% at the same +8 size, but that is still short of the 52.4% best.c on disk, so best.c was left alone and the work file restored. The +8 never closed, which points at a structural difference rather than register allocation.

### Settled

- THIS IS THE THIRD BASIC-BLOCK-LAYOUT PARK, after sub_080373F0 (which of N identical blocks survives cross-jumping) and this one (which arm CSE follows into). Both are decided by the jump/CSE pass interaction and neither is reachable from any statement-level spelling measured so far. The permuter cannot reach either -- it permutes register allocation and statement order, not which basic block a label's use count lets CSE walk into.
- The +12 here is a COINCIDENCE, not a family. Wave 58's brief batched it with sub_0800CAA0 and sub_080200EC on the theory that three drafts at exactly +12 shared a cause. They do not: sub_080200EC was missing a SOURCE induction variable and matched on one attempt once that was seen, sub_0800CAA0 has a strength_reduce giv the ROM does NOT have (opposite direction), and this one is a CSE/cross-jump decision. Do not batch on a byte delta.
- The wave-41 reading was confirmed instruction-for-instruction by an independent re-measurement in wave 58. Nothing in the diff is unexplained.

### Why it is parked

Parked wave 58 (W58-B) after wave 41 (W41-B) measured six source spellings and wave 58 re-measured and confirmed the diagnosis. The open question is NAMED and narrow: find a source shape that keeps the two `= 1` blocks distinct through gcc's FIRST jump pass, so that the `= 1` label still has a single use when cse_end_of_basic_block runs. Every shape tried so far is merged before CSE. Do NOT hand this back as a generic near-miss and do NOT re-run the six axes above; full analysis is in work/sub_08020754/sub_08020754.c.

### Wave 87

WAVE 87 (W87-C, unnamed-twin axis, J = 1.00 on the two map globals but no loops): twins c_08044854.c / c_08026100.c (byte-identical pair) and c_08025744.c all LACK the construct. sub_08026100/sub_08044854 have an if/else with a `= 1` arm but the index is computed ONCE before it and the arms store to a struct field (no duplicated block, no two-use label); sub_08025744 has no if/else over a map store. c_08026100.c's own recorded cost is a comma-chain pool-order anchor (`pp = &gUnknown_08499594`, embedded `t = y * 2`, `idx` before `(cells = p + 0x51A)[idx]`) -- an address-load-position technique that cannot address which block cse_end_of_basic_block may walk into. Clean NO, 0 probes. The two rule-outs close against each other: the nested-if near-miss is merged by the first jump pass before CSE, and binding the offset to a local relieves a register's worth of pressure; any future attempt must make the two `= 1` blocks differ WITHOUT a pressure-relieving local. Do not re-batch on vocabulary overlap. Configured, 220/208 (+12), 27.9%, unchanged.

### Wave 91

WAVE 91 (W91-A), pre-registered member-form test: NEGATIVE, BYTE-IDENTICAL. This draft was already a member form through its local struct Unk20754Map; respelled through the shared struct Map (`((struct Map *)gUnknown_08499590)->terrain[->rowOffset[y] + x]`, `->unitUnk[...]`) and through gMap, _cand.bin is cmp-identical to the wave-90 draft (220/208, +12, 27.9%). NEW MECHANISM READ OFF `rtldump.py --flags="-da -dp"` (dump.cse): the candidate's `= 0` arm reuses the index (reg 47) because CSE reaches that arm by -fcse-skip-blocks -- the `unit == 0` jump branches AROUND the unit-type block (no other entry) to a single-use label -- not by fall-through as this entry's why_it_is_close says; the `= 1` label has two uses, so it starts a fresh block and recomputes with a reload of gUnknown_08499590. The ROM is the mirror, so its source must at CSE time deny the skip-blocks path into `= 0` (a second use of that label, or a second entry into the type block) AND give `= 1` a single-use label behind a barrier. Measured, all byte-identical to the draft: negated tests each `goto one;` then `= 0; continue; one: = 1;`; nested ifs with a shared `zero:` label inside the inner if; `... && !(unit != 0 && (u8)(type - 0x10) <= 4)`. `if (cond) goto zero; = 1; continue; zero: = 0;` is +8/45.7% but puts `= 1` first, recomputes there, and grows the frame to sub sp,#8. Evidence: work/sub_08020754/NOTES.md.

### Wave 93

WAVE 93 (W93-C). UNCHANGED at 220/208 (+12), 26.36%, first difference +0xc. The recommended base is a shape this function's own notes had already measured and rejected.
BASE REJECTED: `best.c` (43.98%, size+8) is, statement for statement, the `k3` variant recorded in work/sub_08020754/NOTES.md -- `if (cond) goto zero; = 1; continue; zero: = 0;` -- which that file describes as '+8, 45.7%, but it puts the `= 1` arm first, recomputes there, and grows the frame to `sub sp,#8`. Wrong direction.' Re-measured this wave: 43.98%, size+8, FIRST DIFFERENCE +0xa against the draft's +0xc, so it diverges EARLIER. It is correct C going the wrong way, and its higher percentage is entirely the 4 bytes it gives back on a draft that is already OVER size. Renamed `best.c.wrongc`.
METHOD NOTE: on a draft that is over size, a candidate closer to the target size scores higher whatever it does to the code. This entry and sub_080303C8's are the same trap in opposite directions (over size and under size); in both, the first-difference offset gave the right answer and the percentage gave the wrong one.
RESIDUAL unchanged: which arm CSE recomputes `rowOffset[y] + x` on.

### Wave 97

wave 97 (W97-L)
Base unchanged (26.36%, +12). Lever 1 (respell ONE identical expression) applied to the "= 0" arm's index, six
spellings (spellings.py): `dst[x + row]`, `*(dst + x + row)`, `(u16)(row + x)`, `row - ~x + 1` all 26.36% +12 (the
compiler canonicalises them back to the same expression, no new value number); `*(volatile u16 *)&row[y]` 24.54% +8;
`*(volatile u32 *)&gUnknown_08499590` as the map pointer in that arm 28.77% +4 but it makes BOTH arms recompute
(reload of the map pointer, row table and an extra literal word) -- wrong direction (ROM: "= 0" arm recomputes,
"= 1" arm reuses the loop-top sum). Mechanism unchanged from the wave-41/87 reading: the ROM's "= 0" block begins
at a label (the `unit == 0` branch target), so cse starts a fresh block there and re-derives the address; the
"= 1" arm stays inside the condition's block. A value-numbering respelling cannot create that; only a block
structure change can, and the jump-pass merge of duplicated `= 1` stores blocks the nested form.
Proposed summary tried: + "respelling the 0-arm index (4 pure commutations, a volatile row read, a volatile map
pointer read) does not split it".

wave 97 (W97-V)
Base: levers 4a-31 (branch polarity: the `dst = 0` arm is the else of the negated condition). wrongc OK. 26.36% +12 -> 43.98% +8. Permuter run 1 only 44.23% via a macro-expanded `new_var = rowOffset` (rejected: unreadable, +0.25%); draft left at the 43.98% levers form. Residual: +8 bytes. ROM keeps dst in r5 and the gMap pointer in sl (one `mov sl, r0`) with only one stack slot; ours keeps dst in sl and spills the map pointer to [sp,#0], so one more live value than the ROM.

</details>
