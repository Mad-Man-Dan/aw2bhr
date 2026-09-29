# sub_08026290

0x08026290, 176 bytes, THUMB, parked.

Best score so far: 58.0%.

## What it does

Gives every army slot that is not yet set up a random value in gPlaySt.co[] (the field name suggests the commanding officer) that no other occupied slot already has. For each slot 1..n (n from sub_0802490C with the map id) whose aiControlled byte is 0, it sets that byte to 2, then draws values from sub_08026254 (a pick from a 0xFF-terminated list) until none of the other occupied slots has the same co[] value, and stores it.

## How close it is

Compiles to the right size (176 bytes) with 58.0% of bytes identical. What is left: the original reloads the plain gPlaySt pool word inside the outer loop where the draft hoists it (the loop pass hoists it because the draft's loop is small enough).

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

### Wave 97

wave 97 (W97-L)
Base: previous draft (28.8%, +8, `sub sp,#4` spill of i+1) kept as sub_08026290.w97L-start.c. New source
(sub_08026290.c, = vc.c): bind the address of the current army's CO byte before the retry loop
(`ci = &gPlaySt.co[i]; ... *ci = v;`), inner loop still on bare `gPlaySt.aiControlled[j]` / `gPlaySt.co[j]`.
Result: size-exact (176), no frame, push list identical, prologue and outer-loop shape now the ROM's, but the score
FALLS to 19.3% because the pool words differ (`&gPlaySt.co[i]` folds to a `gPlaySt+0x3d` literal, `ldr r0,=0x3d`, where
the ROM does `adds r6,r2,#0; adds r6,#0x3d; adds r7,r5,r6` from a bare word; and n lands in r7 not r8).
Negatives (measured with spellings.py): binding `ai` at the top of the outer body and using it in the inner loop
(-4, frame 8), binding `co = gPlaySt.co; ci = co + i` (size-exact, frame 8), both bound (-20), binding ai and co
inside the if AFTER the store (-8, frame 4), `ai`+`co`+`ci` all bound in the first spelling (-20).
Next: ROM's r6 = base+0x3d is a real pointer variable (`co`) alive across the inner loop and r7 = &co[i]; a
spelling that keeps `co` a pointer WITHOUT the frame is still needed (the frame comes from i+1 being spilled once
`co` and `ci` are both live).
Proposed summary tried: + "binding &co[i] before the retry loop removes the frame and the +8 but folds the address
into a gPlaySt+0x3d literal".

wave 97 (W97-V)
Base: levers 5d-73_1cp-57 (`s8 lv0 = i` copy for the aiControlled store, a do{}while(0) around the retry body); wrongc OK (warning is only the `while (0)` literal). 19.32% -> 46.59% size-exact. Permuter run 1 -> 57.95% (`ci = &gPlaySt.co[i]` moved into the retry loop body after the call; value-equal), run 2 nothing. Residual: ROM reloads the gPlaySt pool word inside the outer loop (plain literal) after the early `.rodata` force-addr word (the W95-B split construct); ours hoists both base+56 and base+61. Negative: binding `ai = gPlaySt.aiControlled` at loop top and `co` after the test (the ROM's apparent shape: aiBase in sl, coBase in r6) -> 156-160 bytes (-16..-20), 6-7%: agbcc folds them and drops the held registers.

wave 97 (W97-Z)
Base unchanged (`sub_08026290.c`, 57.95% size-exact; old draft saved as `sub_08026290.w97z-start.c`). Scratch probes only:
`w97z.c` (pointer binds `g`, `ai`, `co`: 160 bytes, 6-7%), `w97z2.c` (inner-loop temps: 31.8% -4 / 52.3% / 20.2% +12),
`w97z3.c` (alias `gUnknown_03003FC0` at entry / outer head / inner loop, five assignments: 184-192 bytes, 12-37%).
All negative. Mechanism found for the ROM's split (see the last chapter of docs/agbcc-codegen.md): PRE's copy `N = P`
gives the entry word its second use; loop.c pass 2 (26-insn limit) decides whether the `mem/u N` load leaves the inner loop;
N then loses allocation and the surviving use becomes a plain literal. The draft differs in that N wins a register (`sl`)
and the entry base is reused for the hoisted `+0x38`. Untried: enlarge the inner loop past 26 insns at loop time with insns
that vanish later while lowering N's priority. Proposed `tried` addition: "aliasing the global's second name at the entry,
outer loop or inner loop, and pointer binds, do not reproduce the pool split".
Follow-up (W97-Z): `-dL` on the draft: outer loop pass 1 (74 insns) hoists insns 41/44 (the `mem/u N` load and its `+0x38`, `savings 2, life 15`) to the preheader; the ROM re-derives both every outer iteration. Reading: loop.c moves them because `threshold*savings*life >= insns` (26*2*15). To keep them in the loop the pseudo's life must be ~1 insn (26*2*1 = 52 < 74). Not reached from source yet.

</details>
