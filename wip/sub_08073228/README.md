# sub_08073228

0x08073228, 220 bytes, THUMB, parked.

Best score so far: 82.7%.

## What it does

Loads the sprite graphics for a text string into sprite VRAM. For each character it looks the code up in the glyph table gUnknown_08614024 and copies that glyph's 256 bytes to the character's slot in VRAM, recording an x position and a running total per character in the proc. At the end it stores the character count in the proc.

## How close it is

Compiles to the right size (220 bytes) with the original's stack frame; 76% of bytes match and every remaining difference is inside the character search loop (its shape and register choice). The draft is now ordinary defined C: the search index is read in its own statement before the copy call, so nothing is read and written without a sequence point.

## What is left

The original has a leftover copy of the table offset at the top of the search loop. Recomputing the offset each pass, so that the compiler's strength reduction leaves that copy behind, was tried and makes things much worse: it also removes the spilled values that give this function its stack frame, which is the part that already matches. The copy has to come from something that does not free up registers.

## Already tried

- for and while search loops: the compiler moves the test to the bottom and precomputes the stride; the original tests at the top, which only a goto loop gives (kept).
- Indexing the table as flat bytes, or swapping the compare's operands: the compare order is right but the stack frame shrinks by 4 bytes.
- Local aliases for the string and proc parameters: the string pointer stays in a register instead of being saved on the stack; casting the parameters at each use is kept.
- `u16 acc` with a cast at the read: the narrowing disappears; a signed `short acc` is right.
- An extra `inext = i + 1`, `x10 = i * 16` or `m = k`: no help; the compiler already does the first and merges the others away.
- Taking the string parameter's address, or static inline helpers, to push it onto the stack: folded away.
- Automatic permuter, four chained runs from the goto-loop draft: 21% to 76% at the right size (this draft), then no further gain.

## Files

- `sub_08073228.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

29.1% (wave 46, W46-H; was 12.7%)

### What still differs

Size-EXACT at 220 bytes and the residual is confined to the inner search loop. Byte-exact already: the whole prologue, the 0x20 frame, the ENTIRE stack-slot map ([sp,#0]=a1, [sp,#4]=a2, [sp,#8]=a4, [sp,#0xc]=acc, [sp,#0x10]=proc+0x2a, [sp,#0x14]=a3*0x20, [sp,#0x18]=proc+0x46, [sp,#0x1c]=i*2), every callee-saved assignment (r5=i, r6=i*0x100, r8=i*0x10, sb=acc<<16, sl=i+1, r4=j*8), the CpuFastSet call, both proc stores, the acc update and the whole epilogue. TWO residuals remain. (1) LOOP ROTATION: the ROM's inner search is UNROTATED -- exit test at the top, unconditional `b` at the bottom, with a dead `adds r0, r4, #0` ahead of the test -- and it strength-reduces j*8 to `adds r4, #8`. The candidate comes out ROTATED (peeled test, bottom-tested `bne`) and recomputes `lsls r4, r3, #3` each iteration. (2) The ROM's compare is `cmp r3, r7`, i.e. `c` on the LEFT, and the VRAM destination groups as (i*0x100 + 0x06010000) + a3*0x20 with the 0x06010000 pool word loaded INSIDE the loop; the candidate lets LICM hoist `0x06010000 + a3*0x20` together as one invariant. NEXT LEVER -- one question only: make agbcc leave the inner search loop UNROTATED. `while (tbl[j].unk00 != c) j++;`, `for (;;) { if (...) break; j++; }` and the for(;;) with a leading `k = j * 8;` all rotate. The ROM's dead `adds r0, r4, #0` ahead of the exit test is the tell that a source statement sits there. Fixing rotation is expected to fix the compare operand order and the j*8 strength reduction with it, because both moved together in every probe.

### Why it is close

Everything outside the inner search loop is byte-identical, including the frame size and every stack slot -- the two things the previous 12.7% attempt got wrong. The score is positional and understates it.

### Already ruled out

- PROTOTYPE. `void sub_08073228(const void *, void *, u16, ProcPtr)` is correct; two callers of sub_08073304 matched byte-for-byte this wave corroborate the chain. Do not re-derive it.
- `u16 acc` with an explicit (s16) cast at the read -- convert_to_integer distributes the narrowing over the `+` and deletes it. Not the spelling.
- gUnknown_08614028 as a second object -- it is this table's `unk04` member (include/unknown-globals.h:15243). The candidate's `.word gUnknown_08614024+4` against the ROM's `.word gUnknown_08614028+0` is the documented same-address false mismatch, NOT a real difference.
- The flat `*((u8 *)gUnknown_08614024 + k)` table spelling: it gets the compare operand order right but DROPS the frame back to 0x1c and puts a1 in sl. The struct-index spelling is required for the frame.
- Swapping the compare to `c == tbl[j].unk00` on its own: same effect -- correct `cmp` operand order but the frame collapses to 0x1c (216 bytes, 20.0%). The compare order and the frame are COUPLED; do not chase them separately.
- TOOLCHAIN. Not the axis: sub_08073714 and two block neighbours matched on the default toolchain this wave, and data/compiler-overrides.json has no entry anywhere in 0x08073.
- The wave-46 orchestrator's advice to 'aim for eight distinct stack-resident locals' -- REFUTED, see settled_by_this_attempt.

### Settled

- `short acc` -- a SIGNED short LOCAL -- is what produces the ROM's `lsls #16; asrs #16` on every read and `lsls #16; lsrs #16` on every write. agbcc's PROMOTE_MODE forces sub-word LOCALS (not just parameters) to zero-extended SImode, so a signed short local sign-extends at each read and truncates at each write. This closes the previous attempt's 'defect 2' outright, and it explains for free why the ROM computes acc<<16 at the TOP of the loop body and the `asrs` at the BOTTOM: the two halves straddle the CpuFastSet call because the value must be carried in a callee-saved register.
- Using the PARAMETERS DIRECTLY with casts at each use (`((const u8 *)a1)[i]`, `((struct Unk73228Proc *)a4)->unk58`) rather than through aliasing locals `str`/`proc` is what produces the ROM's 0x20 frame with a1, a2 and a4 spilled to [sp,#0]/[sp,#4]/[sp,#8] in PARAMETER ORDER. With the aliasing locals, a1 stays in sl and the frame is 0x1c. The casts are byte-neutral; the aliasing locals are not.
- Adding `k = j * 8;` as the first statement of a `for (;;)` inner loop, with k used after the loop for the `.unk04` access, is what puts a real insn ahead of the exit test and makes j*8 survive the loop in r4.
- The stack-slot map, the frame size and every callee-saved register assignment are now exactly the ROM's, so the remaining work is ONLY the inner loop's shape, not allocation.
- [sp,#0x10], [sp,#0x14], [sp,#0x18] and [sp,#0x1c] are LICM hoists and strength_reduce giv temporaries and must NOT be authored as source locals. The 'eight distinct stack-resident locals' reading of the slot map counted four compiler-generated temporaries as source variables; only a1, a2, a4 and acc are author-level.

### Why it is parked

2 try_match attempts spent; W46-H had a single round and three other functions.

### Wave 86

WAVE 86 (W86-C then W86-G): the pre-registered hypothesis 'the inner search is a LABEL/GOTO LOOP' is CONFIRMED (W46-B chapter): with `k = j * 8; p = tbl + k; _search: if (c == *p) goto _found; k += 8; p += 8; j++; goto _search;` the loop is UNROTATED (test at top, `b` at bottom), the compare is `cmp rC, r7` with c on the LEFT, j*8 is the ROM's `adds r4,#8`, and the 0x06010000 pool word stays inside the loop -- all three wave-46 facts moved together as that park predicted; every for/while spelling rotates because it emits NOTE_INSN_LOOP_BEG. Everything settled in wave 46 (struct-index table spelling, parameters used directly with casts, `short acc`) is retained. New state, configured: 216/220 (-4), 21.8% (positional; the divergence moved from inside the loop to +0xa), draft = the goto-loop form (wave-46 draft preserved as w86-start.c). W86-G answered the follow-up: the ROM carries BOTH strides as pointers (r2 = `adds r2,r4,r7` once before the label, then `adds r2,#8`; no per-iteration re-add), so the draft's three strides j/k/p are exactly the ROM's and 'index by k with no pointer local' is a different loop. Also refuted: `int inext = i + 1;` at the body top (size-neutral, marginally worse -- gcc already emits `adds r2,r5,#1 / mov r9,r2` from a plain i++, so the ROM's `adds r3,r5,#1 / mov sl,r3` is not a source construct); `m = k;` at the loop top (coalesces, deleted); moving `c = a1[i]` after the k/p init (reorders three insns only). THE 4 BYTES ARE NOT THE FRAME: `sub sp,#32` vs `#28` costs nothing and the a1 spill is size-neutral (three `ldr rX,[sp]` = three `mov rX,sl`); counted off the diff they are `mov r8,r7` (parking i*0x10 in a high register), `ldr r0,[sp,#28]` (reloading the spilled i*2 after the call) and the dead `adds r0,r4,#0`, less the draft's 2 bytes of pool alignment. So the residual is a SPILL-CHOICE TIE between i*2 and i*0x10 -- each computed once at the body top, each read once after the call in the SAME statement (`proc->unk2a[i] = i * 0x10 + 4`: the address needs i*2, the value needs i*0x10), created in the same order in both; one takes a high register, one the stack, and ROM and draft disagree which. Order-correct, slot-wrong = the permuter's own case, and NO permuter run has ever been taken against the goto-loop draft (the recorded negatives predate it) -- that is the named next step.

### Wave 87

WAVE 87 (W87-B): 21.8%/216 (-4) -> 76.4% SIZE-EXACT 220/220 (draft; best.c 83.2%), first difference +0x19; draft REPLACED (goto-loop wave-86 draft in w87-start.c / _w87_gotoloop.c; the tidy 76.4% splice in _w87_76pct.c). THE RESIDUAL WAS NOT THE i*2 / i*0x10 SPILL TIE: read off target.s, the ROM SPILLS the PARAMETER a1 to [sp,#0] and reloads it at both uses while the candidate kept a1 in sl for the whole function -- that was the entire -4 and the +0xa first difference (`sub sp,#0x20` vs `#0x1c`). Both use exactly three hi registers (ROM r8=i*0x10, sb=acc<<16, sl=i+1; candidate r8=acc<<16, r9=i+1, sl=a1, i*0x10 on the stack): a THREE-WAY contest for the third hi register between a1 and i*0x10, and the loser's slot shifts the whole stack map. W86-C second delay lever (`t = i * 0x10;`) REFUTED: it MOVES the computation to the top of the body and does not touch the a1 decision. `*&a1`, `*(const void *const *)&a1` inline and a dead `ap = &a1;` all fold away (the address must be READ THROUGH); a static-inline helper around `i * 0x10 + 4` or around the whole indexed store leaves a1 in sl (that lever re-cuts live ranges among locals/temporaries, it does not dislodge a PARAMETER). Four chained permuter links from the goto-loop draft (first ever on it): 21.4 -> 83.2, fourth link flat -- the frame, the parameter spill order and the size are now the ROM's and what is left is 52 bytes INSIDE the loop (loop shape and in-loop allocation). remaining_diff/axes_ruled_out above are superseded. Next is a CONSTRUCT, not permuter time: the ROM's dead `adds r0,r4,#0` at the top of the search loop -- wave 46 produced exactly it with `k = j * 8;` as the first statement of a `for(;;)` body, the wave-86 goto rewrite dropped it, the permuter's `j = j * 8; k = j;` echoes it. Untested: a goto loop that RE-DERIVES k at the top of each pass rather than only stepping it, tried on top of the 76.4% draft (whose old negatives are void).

### Wave 93

WAVE 93 (W93-D): 76.36%, size-exact, and THE DRAFT IS NOW DEFINED C at identical bytes. It was raw permuter output that read j in the first CpuFastSet argument while assigning it in the second -- unsequenced, undefined. Fixed at zero byte cost by evaluating the first argument into its own local in the preceding statement: glyph = ((const u8 *) a2) + (j * 0x100); then CpuFastSet(glyph, (void *) ((j = (i * 0x100) + 0x06010000) + (a3 * 0x20)), 0x40);. j is now read in one statement and written in the next, so nothing is unsequenced. The write INTO j is load-bearing and must stay: a fresh local instead of j costs 9.5 points (66.8%), two plain statements cost 6.4 (70.0%), and the same two joined by a comma operator also 70.0% -- all still size-exact. THE ENTRY'S NEXT LEVER IS MEASURED AND WRONG: recomputing k = j * 8 at the top of the goto loop, so that strength reduction leaves the ROM's dead `adds r0, r4, #0`, gives -40 bytes / 10.0% (recompute k and the table pointer), -4 bytes / 30.5% (recompute k only, pointer still stepped), -12 bytes / 30.0% (the same plus the character read in a local) and -12 bytes / 15.5% (a clean rewrite around the idea). Recomputing lets the compiler strength-reduce and then drop the spills that give this function its 0x20 frame and its whole stack-slot map -- which is the part of the draft that is already byte-exact. The dead copy has to come from something that does not relieve register pressure.

### Wave 97

wave 97
Base: the wave-93 draft (76.36%). Hand probes: `c = a1[i]` bound before `j = j*8`, after it, and re-bound inside the search loop:
before/after -> 216 B (-4), 25% / 23.6% (frame 0x1c); re-bound inside the loop -> folded back to the plain draft (76.36%).
Permuter (2 runs of 900 s, `--current`): run 1 76.36 -> **82.27%, size-exact 220**, first diff +0x36; run 2 no improvement.
What run 1 changed (read, semantically identical): the search stride is held in a local (`new_var = 8;` `k += new_var; p += new_var;`),
the table base is bound to `new_var2 = gUnknown_08614024` after the loop entry and used for the `+4` advance read, and the VRAM base is
written `0x06010000 + (i * 0x100)`. Names should become `stride` / `tbl` when someone promotes it. The draft file is the 82.27% form.

wave 97 (W97-U)
Base: the 82.27% permuter form. Found one real lever: the glyph-advance read is `*(int *)(gUnknown_08614028 + k)` in the
ROM -- a SECOND pool word for tbl+4 with offset 0 and the operand order `k + base` (`adds r0,r4,r3; ldr r1,[r0]`), not
`(tbl + k) + 4` (`ldr r1,[r0,#4]`). Declared `extern const u8 gUnknown_08614028[];` above the function (same address as
tbl+4, defined in data/data-08581E70.s) and bound `new_var2 = (const struct Unk08614024 *)gUnknown_08614028;` then
`*(int *)(((u8 *)new_var2) + k)`. That alone flipped i/k registers (76.8%); declaring `int k; int i; int j;` in that
order (k before i) restores them (80.0%). One 600 s permuter run from that: 80.00 -> 82.73% (size-exact, first diff
+0x36): it copies `a4` into a local (`new_var3 = (struct Unk73228Proc *)a4;`, used for the unk2a store) and wraps
`i = 0; acc = 4;` in `if (1)`; read: valid, wrongc "same behaviour on 400 seeds". Stack-slot order of the three hoisted
temps now matches the ROM.
Residual: the ROM loads the character once at the top of the body (`ldr r2,[sp]; adds r0,r2,r5; ... ldrb r3,[r0]`)
BEFORE `k`/`p` are set up, and compares that register; the draft sets `k`/`p` first and loads the char after. Also two
commuted adds (`adds r1,r3,r1` glyph destination; `adds r0,r4,r3` advance read). Tried: `(a3*0x20) + (j = ...)` 70.5%,
`k + (u8*)new_var2` byte-neutral, `c = a1[i]` bound at the loop top: 21.4% -4 (frame 0x1c).
Proposed summary: left = "loop-top ordering of the character load vs the k/p setup, two operand orders".

</details>
