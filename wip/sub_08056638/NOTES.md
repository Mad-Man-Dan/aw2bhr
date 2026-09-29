# sub_08056638 — 144 bytes, 95.8%, size-exact, NOT matched

Wave 58, W58-D. Re-read against the wave-52 park. **Nothing in the wave-52 note
below is wrong and none of it needs re-measuring.** What this wave adds is the
classification and the reason it is not worth another attempt.

## The residual is THREE instructions, and it is an LICM-hoist inversion

Instruction counts are identical. Diffing `target.s` against the draft's own
`sub_08056638.s`, all six differing bytes are these three plus the pool order:

| | ROM | candidate |
|---|---|---|
| loop head, 1st base | `mov r3, sb` | `ldr r3, .L21+4` |
| loop head, 2nd base | `mov r0, sb` | `ldr r0, .L21+4` |
| swap block base | `ldr r2, =gUnknown_0202980A` | `mov r2, r9` |

Both compilations hoist ONE of the two row symbols to a callee-saved hi register
for the whole function and rematerialise the other from the literal pool at its
uses. The ROM hoists `gUnknown_02029822` (the sort key, read every iteration in
the loop head) and rematerialises `gUnknown_0202980A`; the candidate does the
exact opposite. The pool words come out swapped because the hoisted one's `ldr`
is emitted first — that is a symptom of the same fact, not a second fact.

**This is the W58-B "HOLD/REMATERIALISE inversion" residual** (see
`docs/agbcc-codegen.md`), and specifically it is an LICM decision: which
invariant `reg = symbol_ref` is moved to the preheader. W58-A's chapter "LICM
hoists a non-const global load out of a loop that CONTAINS CALLS" puts that in
the unreachable family — instruction ORDER across a loop boundary, reachable by
neither a declaration nor the permuter.

## Ruled out THIS wave, in addition to wave 52's list

- **The member/struct spelling is settled and is NOT the lever.** Both rows are
  0x6c-stride rows of `gUnknown_02029808` through their own linker symbols, and
  `include/unknown-globals.h` (W48-G, W49-M) records the discriminator directly:
  the member form emits `=gUnknown_02029808` plus `adds rB, #0x1a`, an extra
  instruction this function does not have. The flat form both rows already use
  is the right one. Do not spend a probe re-deriving this.
- **The wave-39 LICM blocker does not apply.** The only documented way to stop
  LICM hoisting an invariant is a SECOND assignment to the pseudo
  (`n_times_set == 2`). Here the pseudo being hoisted is the bare `symbol_ref`
  itself, which is single-set by construction — no C spelling gives one symbol
  two different values inside the loop without changing the address arithmetic
  the ROM already matches.

## Do not run the permuter

Wave 52 ran four sessions, ~85k iterations, three starting points. It moves
registers; it cannot move an insn across the loop boundary.

## Wave 81 (W81-B)
Deferring the gUnknown_02029822[side][j] = b; store past both payload writes (semantically free -- different arrays) so the payload base pseudos are created first: 85.4% / 21 bytes, first difference +0x26. The write order inside the swap block is also load-bearing. Park stands as classified.
## W90-A (wave 90) -- still 95.8%, 6 bytes; the residual is now a NUMBER

Draft unchanged (`w90-start.c` == draft, re-verified by the permuter's
baseline each run).

**Mechanism, read straight off an RTL dump (`agbcc ... -dg`; recipe in
`work/sub_0807E980/NOTES.md`).** The `.greg` dump prints every allocno's
`refs` and `live_length`; global.c sorts by
`floor_log2(refs) * refs / live_length`. For the draft:

| pseudo | what | refs | live | priority | gets |
|---|---|---|---|---|---|
| 36 | side*0x6c | 7 | 42 | 0.333 | r8 |
| 109 | &gUnknown_0202980A | 8 | 80 | 0.300 | sb |
| 169 | i+1 | 4 | 35 | 0.229 | sl |
| 31 | &gUnknown_02029822 | 7 | 88 | 0.159 | nothing -> rematerialised from the pool |

REFS ARE LOOP-DEPTH WEIGHTED, AND A `do { } while (0)` COUNTS AS A LOOP LEVEL
in flow (its NOTE_INSN_LOOP_BEG survives to flow here): 109's 8 is
1 (set) + 3 (use at depth 3) + 4 (use inside the swap do/while). THAT is why the
do/while(0) is load-bearing -- it is worth exactly one ref, and one ref crosses
the floor_log2 step at 8. The ROM therefore needs refs(22) in {8, 9} and
refs(0A) <= 7 with every other allocno's order unchanged.

**Exhaustive search of the obvious lever (1,024 variants):** every combination
of wrapping / not wrapping each of the ten inner-loop statements in its own
do/while(0) was compiled with `-dg` (scratch `e638/combo`), and the same space
was run through the permuter as a pure PERM_GENERAL enumeration
(`w90-dowhile.perm.txt`, 1,024 iterations, best = the draft, 95.8%).
320 combinations DO put gUnknown_02029822 in sb and rematerialise
gUnknown_0202980A exactly as the ROM does -- and in every one of them
side*0x6c (36) goes to ip instead of r8, because the only statements that
reference 22's pseudo also reference 36, so any depth bump that lifts 22 to 8
refs lifts 36 to 8 or more, moving it up to or past the j+1 copy (170, 6/21 =
0.571), which then no longer takes ip first (one-wrap variant, `a = ...` wrapped
and the swap do/while removed: 36 is 8/42 = 0.571 exactly and wins the tie on
the lower allocno number). So the next lever has to raise 22's refs WITHOUT
touching side*0x6c's -- or raise the j+1 copy's (170) with it. Not a
statement-wrap question any more.

Permuter, fixed scorer: perm-w90-1 undirected 900 s (11,720 it) and
perm-w90-2 directed 900 s (10,674 it; `w90-directed.perm.txt`: LINESWAP of the
locals, RANDOMIZE of the body, five PERM_GENERAL swap-block shapes) -- nothing
better than the base (720).

## Wave 92 (W92-C)

Unchanged: 144/144, 6 bytes differ, first difference at +0x28. No new axis was
measured. The arithmetic recorded in wave 90 still bounds the problem: the
compiler ranks an address by (a step function of its use count) times the use
count, divided by how long the value stays alive. The keys array needs 8 or 9
weighted uses and the payload array at most 7, with `side * 0x6c` keeping its
register. Raising the keys array's count by a use with a constant subscript
would not touch `side * 0x6c`, but no such use exists in the code, and a dead
one is deleted before the counts are taken.

Two arithmetic routes nobody has measured, recorded here so they are not
re-derived: shortening the keys address's lifetime below about half its
current value, or roughly doubling the payload address's lifetime, each
reverses the ranking without changing either use count. Both need a live
reference in a place the sort does not have one.

### The two arrays are rows of one record — measured, and the flat spelling wins

Both arrays this function walks are rows of `struct Unk02029808` (stride 0x6c):
the payload is the `unk02` row and the keys are the `unk1a` row, so
`gUnknown_0202980A[side][j]` and `gUnknown_02029808[side].unk02[j]` are the same
halfword, as are `gUnknown_02029822[side][j]` and
`gUnknown_02029808[side].unk1a[j]`. A matched function nearby, `sub_0805653C`,
writes that memory both ways, and the note on `gUnknown_020297CC` in
`include/unknown-globals.h` records a matched function that deliberately mixes
the two forms for two rows of one record.

Measured here, all three combinations:

| keys | payload | result |
| --- | --- | --- |
| row symbol | record member | 144 bytes, 7 differ (95.1%) |
| record member | row symbol | 144 bytes, 7 differ (95.1%) |
| record member | record member | 148 bytes (+4), 132 differ (10.8%) |

So the mixed form costs one byte more than the draft and the doubled member
form is far worse — with both rows reached through one record the compiler
shares a single base address, and the original plainly keeps two. The draft's
row-symbol spelling for both arrays is right, and this axis is closed.

## wave 96
Base: draft unchanged (95.83%, size+0, first diff +0x28). Pre-registered hypothesis (bind the keys table address for a subset of reads) tested three ways, bound as `u16 (*k)[54] = gUnknown_02029822;`: loads only 18.8% (size+0, diff at +0x2), all reads 15.3% (-12), stores only 10.1% (+24). Mechanism: the bind makes agbcc fold `side*0x6c` into the base (one shared row pointer), which the ROM plainly does not have (it keeps side*0x6c in r8 and the symbol separately). The wave-90 arithmetic still stands: the keys address needs 8-9 weighted refs with side*0x6c unchanged; a bind adds a ref but also restructures the address arithmetic. Not re-run through the permuter (waves 52/90 did ~110k iterations).
Proposed status: unchanged; left = keys-vs-payload row symbol takes the callee-saved register (3 halfwords); tried adds "row-pointer bind of the keys table, three subsets, all restructure the address arithmetic".

## wave 97 (W97-S)
Draft unchanged (95.83%). Tried by spellings.py: one shared `n = j + 1` temp for every j+1 subscript in the keys array: 19.4%, -4 (frame differs: drops a callee-saved register); `-~j` for the payload's j+1: 31.8%, +4. Both restructure the address arithmetic. No movement on the sb-vs-rematerialised pair.

## wave 97 (W97-PG)
Permuter chain: 1 link, 95.83% -> 95.83%, NO-IMPROVEMENT.
