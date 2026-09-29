# sub_08026290

0x08026290, 176 bytes, THUMB, parked.

Best score so far: 28.8%, +8 bytes (best.c).

## What it does

Gives every army slot that is not yet set up a random value in gPlaySt.co[] (the field name suggests the commanding officer) that no other occupied slot already has. For each slot 1..n (n from sub_0802490C with the map id) whose aiControlled byte is 0, it sets that byte to 2, then draws values from sub_08026254 (a pick from a 0xFF-terminated list) until none of the other occupied slots has the same co[] value, and stores it.

## How close it is

Compiles 8 bytes too long (184 against 176). 30.1% of bytes are identical, which means little because the extra bytes shift everything after them. Both loops, the retry loop, the types and the test order are the original's.

## What is left

The original reaches gPlaySt through a compiler-made address word once, before the loops, and reloads the plain address inside them; our build keeps the address in a register the whole time, which pushes another value into a 4-byte stack frame the original does not have. The best lead is that one more value may be alive across the loops in the original source; look for it in the original's instructions.

## Already tried

- Ten ways of writing the gPlaySt references (pointer locals inside or outside the loops, per-element pointers, a pointer global, pointer arithmetic on the bare symbol, a one-element struct array): none stops the address being kept in a register, and some move the address word onto the wrong reads.
- Pointer locals for the two arrays, set inside the outer loop: the closest yet (both address words come out plain), but the compiler moves both out of the outer loop.
- Writing the outer test as `continue` or as a nested `if`: identical output.
- Writing the inner loop with a hand-made jump to its test: 32 bytes too short, because the compiler no longer makes its automatic copy of the first inner iteration, which the original has.
- Declaring gUnknown_08090A60 (the compiler's address word) as a real pointer global: matches the first three instructions but is almost certainly the wrong model and fixes nothing else.

## Files

- `sub_08026290.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

30.1% identical, candidate 184 bytes (+8), first difference at +0xa. Re-measured wave 59 (W59-B) by exit status; unchanged from waves 41/45/58.

### What still differs

THE HOLD/REMATERIALISE INVERSION, third example. The ROM's pool carries TWO WORDS FOR ONE OBJECT: a .rodata force-addr word (gUnknown_08090A60, dereferenced, used ONCE for the pre-loop unk02 read) and a PLAIN gUnknown_03003FC0 symbol address loaded fresh at each of the two in-loop sites. The candidate emits only the force-addr word and holds it in a register for the whole function. The held base costs a register, so &unk38[0] is hoisted out of the OUTER loop instead of recomputed in it, &unk3d[i] is recomputed at the store instead of CSE'd off &unk3d[0], and i + 1 spills to a 4-byte frame the ROM does not have (sub sp,#4 / str r6,[sp] against the ROM's mov sb,r4).

### Why it is close

Both loops, the do/while retry, the guard order, all four u8 narrowings and the signed j < n + 1 bound are the ROM's. The types are settled: n, i, j, v are u8; the inner bound is n + 1 as its own int compared bge (SIGNED); the retry test is j != n + 1.

### Already ruled out

- Wave 41: j == i vs i == j (settled, it is i == j); declaring gUnknown_08090A60 as a struct pointer; u8 *flags/*vals bound inside the outer loop (kills the force-addr word entirely, both words come out plain, but LICM then hoists both bases and folds vals = flags + 5 -- still the closest anything has come to the ROM's pool split); u8 *flag = &unk38[i] / u8 *slot = &unk3d[i] (emits BOTH pool words in the ROM's order and drops the frame, but still hoists the two bases); the same through a pointer global (agbcc starts emitting gUnknown_03003FC0+0x38 and +0x3d as their own pool words -- worst tried); struct Unk03003FC0 *p = &gUnknown_03003FC0 for the body (p held in one register for the whole function, the opposite of the ROM).
- Wave 45: the outer test as a `continue` vs a nested `if` -- byte-identical, so the outer-loop statement shape is not the axis.
- Wave 59 (W59-B): THE INNER LOOP IN ENTRY-`goto` FORM -- `j = 1; goto _jtest; for (;;) { j++; _jtest: if (j >= n + 1) goto _jdone; ...guards as continues... }`. This is the exact shape that MATCHED sub_08035080 this wave, and here it is 144 bytes, -32, 5.1%. It reproduces the general copy's layout instruction for instruction but destroys the ROM's duplicated first iteration (see settled_by_this_attempt), and it also loses the force-addr word entirely and hoists BOTH bases out of the outer loop, so it is not a partial win either. Three inner-loop spellings are now measured -- continue-chain, nested-if, entry-goto -- the first two identical and the third 32 bytes short. Do not spend another wave on the inner loop's statement form.

### Settled

- Wave 59 (W59-B): THE ROM'S BLOCK AT _080262D2 IS A DUPLICATED FIRST INNER ITERATION, not a second source statement. It is a copy of the loop body specialised to j == 1 -- `cmp r5,#1` where the general copy has `cmp r5,r1`, and the literal offsets +0x39 / +0x3e (unk38[1], unk3d[1]) where the general copy indexes by r1 -- ending in `b _0802631A`, which cross-jumps into the general copy's shared tail. That is gcc's duplicate_loop_exit_test firing on a loop whose entry is a jump to a top exit test: it copies the test and what follows into the preheader and drops the entry jump. So 32 of this function's 176 bytes are a compiler transform, and any spelling that removes the entry jump removes them.
- Wave 59 (W59-B): THE ENTRY-`goto` SHAPE AND duplicate_loop_exit_test ARE MUTUALLY EXCLUSIVE. Writing the entry jump by hand leaves loop_start immediately before the INCREMENT rather than before the exit test, so the transform no longer applies. Both endings come from the same optimiser path; read the ROM's loop entry to tell which one you need -- a `b` means use the trick, a duplicated first iteration means do not. Written up in docs/agbcc-codegen.md.

### Notes

Wave 59 (W59-B). THIRD EXAMPLE OF A KNOWN-UNREACHABLE CLASS, with sub_0800CAA0 (+12) and sub_08028D28 (+4). W59-A isolated sub_0800CAA0's residual to exactly one live value and ruled out four address spellings across four waves; its conclusion is that the remaining axis is loop.c's move_movables cost test and is not addressable from C. Treat this as a park with evidence, not as a function to attempt. One caveat carried from W59-B's own result: W59-A recorded the component-ref rule as 'true in straight-line code, false in a loop', and that qualifier is wrong -- the discriminator is the loop's ROTATION, not its existence (sub_08035080 is a member reference inside a loop that matched byte-for-byte with neither the fold nor the hoist). That correction does not change the conclusion here, but it does mean a future measurement must record which layout it was taken in.

### Wave 87

WAVE 87 (W87-A): pre-registered W86-F bare-symbol pointer arithmetic in the loop body (`*((u8 *)&gUnknown_03003FC0 + (i + 0x38))` at all four sites) REFUTED -- it INVERTS the ROM's split: the pre-loop unk02 read becomes a direct pool load and the loop sites acquire `.rodata` force-addr words (`.LC2: .word g+0x3d`) with an extra indirection per iteration. New -fforce-addr fact: the force-addr word FOLLOWS THE POINTER-ARITHMETIC REFERENCE, not the member reference. That form preserves the duplicated first inner iteration (wave-59 settled) but drops to two hi registers (`push {r6,r7}` vs the ROM's and baseline's three) -- loses a live value instead of gaining one. Second form (W86-C array tell, since the ROM's `ldr r2,=g` + runtime `adds r3,#0x38` matches that chapter's exemplar): `extern struct Unk03003FC0 g[]; g[0].unk38[i]` -- BYTE-IDENTICAL to the baseline (word still emitted, base still held in r9, `sl = 0x38 + r9` still hoisted out of the outer loop, frame still present). Reference-form axis now has TEN measurements (wave 41 six, wave 59 one, wave 87 three) and no movement: STOP RESPELLING THE REFERENCE. The one pointer this wave: sub_0800CAA0's accident (same hold/rematerialise class) flipped when ONE MORE VALUE was live across the loop -- the axis is move_movables' pressure test driven by live-value count. Configured, 184/176 (+8), 30.1%, unchanged, 0 try_match.

</details>
