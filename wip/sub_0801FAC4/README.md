# sub_0801FAC4

0x0801FAC4, 540 bytes, THUMB, parked.

Best score so far: 45.6%.

## What it does

Paints a widening triangle of cells with value a5 into the current map plane (through the row table gUnknown_03003340), starting at cell (a1, a2) and reaching a4 steps in direction a3 (0 down, 1 up, 2 left, 3 right). Step k of the triangle is 2k-1 cells wide, centred on the start cell, and every row or column is clipped to the map's width and height.

## How close it is

Now compiles to the right size (540 bytes) where it used to be 4 too short, with 45.6% of bytes in place. Hoisting case 0's loop bound into its own variable gives the original's second stack slot. What is left starts 0x24 bytes in and is register choice: the two parameters widened in the prologue land in each other's registers.

## What is left

The original keeps two loop values in two separate stack slots (the fixed bound and the stepping value); our build shares one slot between them. Find source that gives the compiler two slots without growing the code.

## Already tried

- A separate bound variable scoped to cases 0, 2 and 3: the stack frame reaches the original's 8 bytes, but the code grows to 548 (8 too long) and the registers diverge in those cases.
- The same bound variable in case 0 only, at case or function scope: still one slot, and the case's registers change.
- Wrapping each case loop so it cannot run zero times: no change.
- Merging loop variables: no improvement.
- Permuter, about 25,500 attempts: no improvement.

## Files

- `sub_0801FAC4.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

45.56% -- 540 bytes, SIZE-EXACT (was 36.30% at -4 at wave 93 start)

### Notes

PARKED Wave 71 at 536/540 (-4), 36.3%. ROM uses two stack slots for loop invariant/stepped bounds while the draft coalesces to one. Zero-trip loops and explicit scoped bounds, including case-0-only placement, are ruled out; the latter grows to 548 or remains one-slot.

### Wave 93

- **result:** 36.30% at -4 bytes -> 45.56% SIZE-EXACT, first difference +0xa -> +0x24
- **best_c_was_WRONG_C:** The wave-start scan named best.c (65.93%, size-exact) as a better base. It is not equivalent to the draft and has been quarantined as best.c.wrongc, with its stale best.json deleted. Two defects. (1) Case 2 reads e before it is set: the permuter deleted e = s + w; and rewrote the clip as if ((s + w) > bound) { e = bound; }, so on any iteration where the clip does not fire e is never assigned -- uninitialised on the first iteration and stale afterwards. (2) Case 3 clobbers its own loop bound: the inner loop became for (y = s; y < e; y++) { e = x; map[y][e] = a5; }, changing the exit test from y < clipped(s+w) to y < x. Its extra 4 bytes came from that per-iteration register copy, so its size-exactness was bought by nonsense code -- the brief's 'a score that rose because the SIZE changed is not progress' case.
- **history_note:** Because best.c has been wrong C for some time, the recorded negative 'permuter, about 25,500 attempts: no improvement' and the wave-65 note that a scoped-bound experiment 'was invalidated by historical permuter-mutated best.c' were both anchored on code that computes the wrong thing. Treat that 25,500-attempt negative as not applicable to the draft.
- **the_fix:** volatile int bound; and in case 0 only, bound = a2 + a4; before the loop, with the test written y < bound. a2 and a4 are parameters the loop never modifies, so bound is set before it is read and holds one value throughout; volatile only forces the reload. Equivalent C. Wave 71 measured this exact construct WITHOUT volatile, at case scope and at function scope, and recorded that it 'still leaves a four-byte frame'. Adding volatile is the whole difference: 536/540 at 36.30% becomes 540/540 at 45.56%.
- **mechanism:** An ordinary local never creates a stack slot -- local_alloc coalesces it into a register wherever it is scoped. A volatile local is a MEM by definition and always gets one. This function needed exactly one more addressable local than it was making. Generalised into docs/agbcc-codegen.md as the frame-slot chapter, measured here and on sub_080726E8 in opposite directions.
- **scope_measured:** volatile bound in case 0 only 45.56% size-exact; in cases 0, 2 and 3 it is 29.38% at +8; in all four cases 12.68% at +12. Case 0 alone is the answer, which also says the ROM's other three arms do not spend a second slot.
- **permuter_REJECTED:** 900 s x 4 threads from the new base reported IMPROVED 45.56% -> 61.85%. REJECTED as wrong C, two defects. (1) s = a1 + 1; was hoisted out of case 0's loop to sit between case 1's break; and the case 0: label, which is unreachable, leaving case 0's body starting at s -= k; -- so s is read uninitialised on the first iteration and accumulates thereafter instead of being reset each iteration. (2) new_var was bound to (u16 *)(gUnknown_08499590 + 2) inside case 2's loop and then dereferenced for case 3's clip, and cases 2 and 3 are mutually exclusive switch arms. Output kept as w93-perm1-6185.c.wrongc and best.c.wrongc2; base restored and re-measured at 45.56%.
- **residual:** 540/540, 294 of 540 bytes differ, first difference +0x24, a swap of two high registers in the prologue: the ROM has a4 (lsls/lsrs #16) -> r8 and a5 (lsls/lsrs #24) -> ip, the candidate has them exchanged. Everything downstream follows from that one transposition. Size-exact with the multiset right and two registers swapped is decomp-permuter's documented case, but the run above must be audited, not trusted.

</details>
