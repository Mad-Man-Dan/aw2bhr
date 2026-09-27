# sub_0801ECE8

0x0801ECE8, 152 bytes, THUMB, parked.

Best score so far: 58.5%.

## What it does

Queues one sprite for drawing. It fills the next free record of gUnknown_0200ED20 (index gUnknown_03002510) with position a2/a3, the frame data a4, the 8-byte OAM template a5 with bits 12 and 13 of its first word cleared (the hide and flicker flags that sub_0801DCD4 tests), and a6. It then inserts the record into the sorted list with sub_0801A718 using a1 as the key; returns 1 if the list is full, otherwise bumps the record count and returns 0.

## How close it is

Compiles to the right size (152 bytes); 63 bytes differ. The cause is one constant: clearing bits in the 64-bit template leaves the compiler holding 0xFFFFFFFF for the upper word, and it reuses that for the `== -1` test where the original builds -1 with two instructions. That adds a literal-pool word and moves registers around it.

## What is left

Clear the two template bits without a 64-bit AND against a negative constant, while still computing the record's address only once. Every way of writing that 64-bit AND loads the 0xFFFFFFFF, so the fix has to avoid the 64-bit AND altogether.

## Already tried

- Both masks in one expression (`a5 & ~0x2000 & ~0x1000`): folded into a single mask, 12 bytes short.
- Masking the parameter a5 in place: gives it a stack slot and moves the masking to the top of the function.
- Masking the template as two 32-bit halves through pointer casts: the -1 comes out right, but the record's address is rebuilt three times; 20 bytes too long.
- A pointer local for the record: changes the address arithmetic away from the original's.
- Reading a6 between the two masks, splitting the masks into separate statements, or putting the constant first (`~0x2000 & a5`): no change, because the constant is loaded before the AND is built.

## Files

- `sub_0801ECE8.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

58.6% identical, SIZE-EXACT

### What still differs

ONE CONSTANT. The two DImode masks against negative `int` constants materialise 0xFFFFFFFF for the HIGH word. The AND itself is correctly optimised away -- the high word is stored unmasked, as in the ROM -- but the constant survives as a live pseudo and CSE then spends it on the `== -1` comparison, giving `ldr rN,.LC; adds rM,rN,#0; cmp r0,rM` where the ROM builds -1 the long way with `movs r1,#1; rsbs r1,r1,#0`. That is a fifth pool word the ROM does not have; it is size-neutral only because it saves the two-instruction build, and it drags the allocation with it (a5's halves land in r4/r5 where the ROM uses r5/r6, the index temp in r6 where the ROM uses r4).

### Why it is close

The wave-33-to-41 blocker is solved: the sixth parameter is `volatile` AND read into a local, which is what stops copy propagation deleting the local and gives the ROM's full-word `ldr [sp,#0x2c]` in assign_parms. In-place `v &= ~0x1000` rather than the comma form took it from +4 bytes to size-exact.

### Already ruled out

- `a5 & ~0x2000 & ~0x1000` in one expression -- folds to a single `& 0xFFFFCFFF` before any RTL exists, one pool word, -12 bytes.
- `a5 &= ~0x2000; a5 &= ~0x1000;` -- modifying the parameter gives it a stack home (`sub sp,#8` plus a str/ldr pair of both halves) and hoists the masking to the top.
- Two u32 pointer casts on the member -- DOES fix the constants completely and restores `movs #1; rsbs #0`, but each cast starts its own address expression, so the element address is rebuilt three times and gUnknown_03002510 is re-read: +20 bytes.
- A pointer local for the element -- changes the unk04 address arithmetic away from what both ROMs show.

### Settled

- Wave 58 CONFIRMS THE TYPE MODEL against the matched twin. src/decomp/c_0801E338.c is promoted with the identical signature `(int, int, int, int, long long a5, volatile int a6)` and `unk0c = a5`, and it builds -1 the ROM's way precisely because it does no masking and never creates the high-word constant. The residual is the two masks and nothing else.
- The twin does NOT discriminate one `long long` member from two `u32` members -- both emit two `str`s -- but the two masks do: as SImode statements `combine` merges them into one `& 0xFFFFCFFF`, and the ROM has two distinct pool constants. DImode is the only reading that survives, so the type model is not the way out.
- The whole residual disappears if the high-word constant is never created, and every spelling that avoids creating it also loses the single element-address expression. That is the shape of the remaining problem.

### Why it is parked

Wave 58 (W58-A), carried from waves 33/36/40/42. Constant placement: a DImode mask's high-word all-ones constant that CSE re-spends on the `== -1` compare. Type model confirmed against a matched twin, so allocation and typing are both excluded.

### Wave 81

WAVE 81 (D): interleaving t=a6 BETWEEN the two DImode masks refuted by probe. The masks do split into two separate ANDs, but the 0xFFFFFFFF high word is created BEFORE the first mask regardless (its ldr/adds pair sits above the ANDs) and the ==-1 compare still consumes it; three pool constants persist. High-word materialisation follows from ANY DImode AND against a negative int constant, not from statement adjacency or evaluation window.

### Wave 86

WAVE 86 (W86-E, vocabulary-twin axis): twin sub_0801E8D8 (src/decomp/c_0801E8D8.c, Jaccard 1.00) is a TRUE SHAPE TWIN; its only spelling difference (constant-first operand order `v = ~0x2000 & a5; v = ~0x1000 & v;`) is byte-neutral -- still `and r4,r4,r1`, still FIVE pool words against the ROM's FOUR. compile_probe only, no try_match, draft unchanged. MECHANISM NAMED AND CLOSED: agbcc's own .s comment says the -0x1 high word is `created by thumb_load_double_from_address` -- the DImode constant loader pulls BOTH words of the CONST_DOUBLE out of the pool into a register pair BEFORE any AND rtx exists; the high-half AND folds away but its operand register is already live, and cse merely re-spends it. So NO source-level respelling of a DImode AND against a negative int constant can avoid the high-word pool constant; the five spellings measured to date (one-expression fold and parameter self-assignment in wave 58, interleaving and statement split in W81-D, operand order in W86-E) all behave identically as predicted. The only escape is not having a DImode AND, and every spelling that removes it loses the single element-address expression (+20 bytes, wave 58). Treat as a closed kind-3 with a named mechanism, like kinds 4 and 5: stop respelling the mask.

### Wave 93

W93-E: not reached; still never permuted.

</details>
