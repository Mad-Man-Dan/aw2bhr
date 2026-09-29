# sub_0801ECE8

0x0801ECE8, 152 bytes, THUMB, parked.

Best score so far: 76.3% (best.c).

## What it does

Queues one sprite for drawing. It fills the next free record of gUnknown_0200ED20 (index gUnknown_03002510) with position a2/a3, the frame data a4, the 8-byte OAM template a5 with bits 12 and 13 of its first word cleared (the hide and flicker flags that sub_0801DCD4 tests), and a6. It then inserts the record into the sorted list with sub_0801A718 using a1 as the key; returns 1 if the list is full, otherwise bumps the record count and returns 0.

## How close it is

The recorded draft compiles to the right size (152 bytes) with 63 bytes different. A form found in wave 94 is far better understood, though 4 bytes short: it reproduces every constant the original uses, both stores of the 8-byte template, and the whole stack frame, and differs only in which of two values keeps the last spare register. It is kept beside the draft as w94b-simode-words.c.

## What is left

In the w94b-simode-words.c form, give the template's high word the last spare low register instead of the record array's base address. That swap is worth exactly the 4 bytes the form is short, because a base address in a high register cannot be advanced by an immediate and so costs two more instructions. Reading the two halves of the 64-bit argument as a register pair, rather than through its address, does produce the original's choice, so what is left is to do that without the shift that currently forces a larger stack frame.

## Already tried

- Both masks in one expression (`a5 & ~0x2000 & ~0x1000`): folded into a single mask, 12 bytes short.
- Masking the parameter a5 in place: gives it a stack slot and moves the masking to the top of the function.
- Masking the template as two 32-bit halves through pointer casts: the -1 comes out right, but the record's address is rebuilt three times; 20 bytes too long.
- A pointer local for the record: changes the address arithmetic away from the original's.
- Reading a6 between the two masks, splitting the masks into separate statements, or putting the constant first (`~0x2000 & a5`): no change, because the constant is loaded before the AND is built.
- Masking the low 32 bits only, and writing the record's two words through a file-local struct that spells the 64-bit member as two words: this removes the extra constant entirely and is 4 bytes short, not 20 bytes long as the earlier pointer-cast attempt was.
- Two plain int parameters instead of the 64-bit one: refuted by two already-matched callers, which pass a 64-bit value.

## Files

- `sub_0801ECE8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
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

### Wave 94

W94-B: THE CONSTANT IS SOLVED; the residual is now one register. The new form work/sub_0801ECE8/w94b-simode-words.c measures 148 bytes (-4) and 30.92% -- positional: it is two instructions short in one place -- and it reproduces the ROM's four pool words (no 0xFFFFFFFF), its [r0,#0xc]/[r0,#0x10] store pair, its prologue, its 4-byte frame and its long-form -1, with correct semantics and no change to any shared header. Three parts: (1) mask the LOW WORD in SImode, via ((int *)&a5)[0]. The ROM cannot come from a DImode AND, because thumb_load_double_from_address pulls BOTH words of a CONST_DOUBLE from the pool, so two DImode masks would leave FOUR mask words where the ROM has two. (2) Write the 0x0c and 0x10 words through a FILE-LOCAL struct that spells the 64-bit member as two u32, casting the array base: ((struct W *)gUnknown_0200ED20)[gUnknown_03002510].unk0c = lo. This CSEs with every other member access, where the &member casts the park rejected each rebuilt the index and re-read gUnknown_03002510. struct Unk0200ED20 is NOT changed, so c_0801E338.c is untouched. (3) Bind lo then hi at the top, which gives the ROM's two ldr order, its frame and its three saved high registers. RESIDUAL: five values want the four low callee-saved registers, and the ROM gives the last one to the template's high word while this form gives it to the record array's base (ROM r5=lo, r6=hi, base->r8; ours r5=base, r6=lo, hi->sl). The ROM's choice is the dearer one and is exactly the -4: a base address in a high register cannot be advanced by an immediate, so the ROM pays one extra insn to load it and one more to add 4. Allocno priority floor_log2(refs)*refs/live_length predicts OUR order (base 2*4/8=1.0 against hi 1*2/25=0.08), not the ROM's. MEASURED INERT, three variants in one probe, all the same length: reading hi before lo; masking into a separate variable (copy propagation folds it back, so it does not produce the ROM's `adds r5,r1,#0`); moving the unk10 store after the unk0a store. NEXT LEAD, and it is a positive: `lo = a5; hi = a5 >> 32;` -- no &a5, so the parameter stays a DImode register pair -- DOES produce the ROM's assignment, base in r8 and hi in a low register. It costs an 8-byte frame and spills a2, so it is worse overall, but it proves the ROM's assignment is reachable; get the halves as a register pair without the shift and this closes. ALSO REFUTED THIS WAVE: parameters 5 and 6 are NOT two ints -- the matched callers c_0801ED80.c (a long long straddling r3 and the first stack slot, with agbcc's own thumb_load_double_from_address annotation) and sub_0801EDF8 (one movs r3,#0 covers both halves of a 64-bit zero, measured 4 bytes better than two int zeros) both prove the 64-bit argument. And the ROM's two unfolded ANDs are NOT a DImode tell: agbcc leaves two statement-level SImode ANDs unfolded too. PERMUTER, 900 s --current: reported 58.55% -> 89.47% size-exact, and the form is WRONG C (w94b-perm1-hi-zeroed.c.wrongc). It wrote `unsigned int new_var = 0x1000; v &= ~new_var;`; `~` on an unsigned int ZERO-extends to 64 bits, so the template's high word is cleared where the ROM stores it unchanged. It scored because clearing the high word makes the dead 0xFFFFFFFF visibly dead -- the same shape the correct form reaches honestly. Draft left at the 58.55% size-exact form; w94b-simode-words.c is the real base for the next attempt, and drafts.py bases will NOT name it, because it scores lower.

### Wave 97

wave 97
Base: `w94b-simode-words.c` (the honest 32-bit-halves draft, 30.92% because it is 4 bytes short; `sub_0801ECE8.c` is left as the size-exact DImode draft, 58.55%, saved as `sub_0801ECE8.w97-start.c`). `best.c` is the old DImode family and the two `.wrongc` files stay wrong.

Why the 32-bit-halves form is the right family, measured from the disassembly: the ROM masks only the LOW word (`0xFFFFDFFF`, `0xFFFFEFFF` are its only two pool words besides the two addresses) and stores the HIGH word unchanged (`str r6,[r0,#0x10]`). The DImode draft materialises the mask's all-ones upper half as a register and then reuses it for the `== -1` compare (`cmp r0,r6`), which is why it has five pool words to the ROM's four.

Residual of the halves form (4 bytes short): the ROM keeps low half / high half in r5 / r6 and the record-array base in r8; ours puts the base in r5 and the high half in sl. Reading `-da .greg`: the base pseudo (3 refs over 24 insns) outranks the two half pseudos (2 refs over 19 insns each), so it gets the low register. The ROM ranks them the other way round. No respelling of the read order (six orders), of the mask (`lo &= a; lo &= b`, one expression, a `u32` copy) or of the store order changed one byte; reading and masking `a5` in place through `((int *)&a5)[k]` forces the argument to memory and is far worse (6 of 76 halfwords).

Permuter run 1 (900 s x 2) 30.92 -> 76.32%, size-exact, REJECTED: it duplicates `unk02 = a3;` after the `unk04` store. The duplicated store survives to the object as a second `strh r2,[r0,#2]`, i.e. the extra 4 bytes are padding, not the ROM's missing `movs r1,#4`. Kept as `sub_0801ECE8.w97-perm1-out-PADDED.c`. Its useful signal: adding a reference that costs a store changes which pseudos get the low registers, so the real lever is a way to add one use of the base pseudo without emitting code.

Proposed summary: does = as before; status = "152 bytes, 4 bytes short, honest 32-bit halves"; left = "the original keeps the base address in a high register and both template halves in low ones; ours does the opposite, which costs the ROM's `movs r1,#4` / `add r8,r1` pair"; tried = the orders above, in-place `&a5` access, and the padded permuter form.

</details>
