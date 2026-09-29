# sub_08039588

0x08039588, 172 bytes, THUMB, parked.

Best score so far: 87.2%.

## What it does

Copies a text string's glyphs into sprite tile memory starting at 0x06016140, looking each character up in the table gUnknown_08090F30. The glyph data comes from a 0x3000-byte block returned by sub_08014E44; where that block's contents come from is unknown.

## How close it is

Compiles to the right size (172 bytes); 22 of 172 bytes differ, 87.2% line up.

## What is left

The original computes the VRAM destination once per character in the setup of the search loop, after that loop's first test, and shares one register between the slot counter and the destination; every position for that assignment has now been measured and none gives both. The only direction left is that the original wrote the destination as a different expression altogether.

## Already tried

- The destination written inline in the call: the compiler merges its two constants into one and the function is 8 bytes short (43%).
- Assigning the destination inside the `if`, at its use: same merge, 8 bytes short.
- Assigning it as the first statement of the search loop: still merged, and computed before the table address instead of after.
- Assigning it at the bottom of the search loop body: the compiler will not move it out of the loop, so it costs instructions on every pass.
- Assigning it both before the search loop and again at its bottom: both constants survive but neither assignment moves.
- Rewriting the search loop by hand as a do/while with a first test: much worse (an extra constant word and different registers).
- The automatic permuter for 5 minutes: every candidate drifted back toward the merged, 8-byte-short form.

## Files

- `sub_08039588.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

87.2% identical, SIZE-EXACT at 172 bytes, 22 of 172 bytes differ (wave 73, W73-D). Was 43.0% at 164 bytes (-8).

### What still differs

22 bytes, TWO facts, and they are one cause. (a) POSITION: the ROM computes `dst` as an LICM HOIST in the inner loop's preheader -- AFTER the zero-trip guard (ldrb/cmp/beq) and AFTER the table-base hoist `ldr r3,[pc,#36]`. The draft computes it as a source statement in the outer loop body, so the three instructions (`lsls r0,rJ,#8`, `ldr r1,=0x6140`, `adds r4,r0,r1`) land BEFORE the guard. (b) SLOTS: the ROM shares r4 between `j` and `dst` -- `adds r6,r4,#1` saves j+1 before the reuse -- so `movs r4,#0`, `lsls r0,r4,#8` and `lsrs r4,r0,#16` all name r4; the draft has j in r3 and dst in r4, which also shifts both `ldr [pc,#N]` displacements. (b) FOLLOWS FROM (a): while `dst` is a source statement its live range starts before the guard and cannot overlap j's. IGNORE the branch lines in --diff -- `<_08039620>` against `<sub_08039588+0x98>` is the same encoding.

### Why it is close

THE CONSTANT MERGE, which was the whole park for four waves, IS SOLVED. cse merges `dst + 0x06010000` into `dst = (j << 8) + 0x6140` whenever both sit in the same extended basic block, collapsing the ROM's two pool words into one `=0x06016140` and costing 8 bytes. Assigning `dst` in the OUTER loop body, before the inner `for`, puts the definition behind the inner loop's back edge where cse cannot reach it, and both pool words survive: 164 bytes (-8) at 43.0% becomes size-exact at 87.2%. The open question has MOVED and is now a positioning question: what source form keeps the definition out of the use's extended basic block (which the pool words require) while still presenting it to loop.c as an invariant OF THE INNER LOOP (which the hoist position requires)? Those are the two ends of one span and every spelling measured so far sits at one end or the other.

### Already ruled out

- the expression inline, `(void *)(0x06010000 + j * 0x100 + 0x6140)`, and `(u8 *)0x06010000 + (j * 0x100 + 0x6140)`: fold/cse merge the two literals, 164 bytes
- `dst = ...` as the first statement of the inner loop body: still merged, and it also hoists BEFORE the table base, reversing the preheader
- `dst = ...` inside the `if`, i.e. at the use: merged. THIS WAS THE KEPT DRAFT FOR FOUR WAVES at 43.0% / -8 bytes
- hand-rotating the inner loop into `k = 0; if (tbl[k]) do {...} while;` so the assignment sits in the preheader: much worse -- it spills the table base to a -fforce-addr .rodata word and moves str into r9
- WAVE 73 (W73-G): DEFINING `dst` IN THE OUTER BODY *AND* REDUNDANTLY AT THE END OF THE INNER BODY -- the one spelling that looked like it reached both ends of the span. It reaches neither. The two pool words survive, but the inner def is NEITHER HOISTED NOR DELETED (it sits at the loop bottom costing two instructions an iteration) and the outer def still lands ahead of the zero-trip guard. The reason is a precondition of gcc's move_movables this project had not recorded: A MOVABLE'S REGISTER MUST NOT BE LIVE ON ENTRY TO THE LOOP. Defining dst anywhere outside the inner loop makes it live at entry and FORECLOSES THE HOIST BY CONSTRUCTION, so no outer position can ever work and the whole `where outside` axis is closed. What remains is to split the def and the use into different extended basic blocks with BOTH inside the loop, without adding control flow. See the new chapter in docs/agbcc-codegen.md.
- WAVE 73 (W73-D): decomp-permuter, 300 s / 4 threads, from the new 87.2% best.c. No match, and every candidate it scored regressed TOWARDS the merged -8 spelling (best 40.7% at 164 bytes) -- the tool confirming from the other side that the size-exact spelling is the right branch. Wave 57 had closed this axis BY ARGUMENT ('the residual is a missing INSTRUCTION, wave 37's one useless case for the permuter'); that was correct for the OLD draft and does not apply to this one, which is size-exact. Now measured rather than reasoned about.
- WAVE 79 (W79-E): `dst` defined ONLY at the BOTTOM of the inner loop body (after the `if`, so the def is outside the use's forward cse reach and its pseudo is created after the exit test's). This was the one point of the span wave 73 left untried -- W73-G kept the outer def as well, and blamed the failure on that. It fails on its own too, and the reason CLOSES THE SPAN BY PROOF rather than leaving it open: the def is NOT hoisted (it stays at the loop bottom as `lsl r0,r6,#8; ldr r1,=0x6140; add r1,r1,r0; mov r8,r1`, and dst is pushed out to r8). loop.c's `loop_reg_used_before_p` refuses to hoist a movable whose register is READ BEFORE THE SET INSIDE THE LOOP, exactly as it refuses one live on entry. So hoisting requires the def to DOMINATE the use, and the pool split requires the def to be OUTSIDE the use's extended basic block; inside a loop body with no join those two are mutually exclusive. THE `both inside the loop` AXIS IS NOW CLOSED AS WELL, and with W73-G's `outside the loop` result that is the whole span.

### Settled

- WAVE 43 (W43-I), still valid: THE DEF/USE SPLIT IS STRUCTURAL. Both `dst` and `dst + 0x06010000` are invariant in the inner loop, yet the ROM hoists only the first -- the second's destination is r1, set immediately before the `bl`, and loop.c will not move an invariant whose register is call-clobbered out past the call. `dst` lands in callee-saved r4 and hoists freely. A non-trapping invariant inside a conditional still hoists, so both may legitimately be written inside the `if`.
- WAVE 73: the merge is an EXTENDED-BASIC-BLOCK property of cse, not a tree-level fold. `dst` is a VAR_DECL, so `0x06010000 + dst` cannot be folded at tree level at all; what merges the literals is cse propagating the definition into the use, and it can only do that while the two are in one EBB. Putting the definition behind a back edge is what stops it. This is the general lever for a 'two pool words became one' residual.
- WAVE 73 PROCEDURAL: this entry previously read 'NOT CHARACTERISED', 'W59-B did not open this function' and 'NOTHING WAS RULED OUT IN WAVE 59'. All three were misleading -- work/sub_08039588/sub_08039588.c carried three waves of measured evidence (W43-I, W57-E) the whole time. A position-only entry written as though the function were unexamined cost this function two waves of being read as either exhausted or untouched, and it was neither. If an entry records position only, say where the evidence actually lives.

### Why it is parked

Wave 79 W79-E: THE SPAN IS CLOSED AT BOTH ENDS AND THERE IS NO POINT LEFT IN IT. The hoist needs the def to dominate the use (loop.c will not move a movable whose register is read before its set, nor one live on loop entry); the two-pool-word split needs the def to sit outside the use's extended basic block. The inner loop body contains no control-flow join, so no position satisfies both, and every position has now been measured: outside the loop (W73-G, foreclosed by liveness), top of the body (hoisted but merged, and ahead of the table base), inside the `if` at the use (merged), bottom of the body (W79-E, not hoisted at all). A SOURCE CONSTRUCT IS BEHIND THIS RESIDUAL -- the LICM hoist position of `dst` -- but it is not reachable from C without adding control flow the ROM does not have. Do not spend another wave on positioning. If this function is resumed, the only untried direction is that the ROM's `dst` is not this expression at all.

### Wave 93

WAVE 93 (W93-D): still 87.2%, size-exact, draft unchanged. HALF THE RESIDUAL IS SOLVED. Residual (a) had two parts, the hoist's POSITION and its ORDER; the order is now a solved, general lever. Reading the table entry into a local as the FIRST statement of the search loop body (c = gUnknown_08090F30[k]; then comparing against c) flips the preheader from `lsl` then `ldr` to `ldr` then `lsl`, which is the ROM's order, at no instruction cost -- the ROM loads tbl[k] into a register there anyway. General rule, now written into docs/agbcc-codegen.md: LICM emits its hoists in the order the invariants' first references appear in the loop body, so a leading reference decides the preheader order. WHAT IS LEFT is only the constant merge: the preheader gets `lsl r4, r4, #8` and the +0x6140 folds into the use as one `=0x6016140` pool word, 164 bytes (-8) at 43.0%. NEWLY REFUTED merge-blockers, all still producing the single 0x6016140 word: -fno-cse-follow-jumps (measured end to end, 43.0% / -8, unchanged); splitting the def into `dst = j * 0x100;` then `dst = dst + 0x6140;`; declaring dst a `u8 *` and adding the VRAM base as pointer arithmetic; and a volatile read of the table entry between the def and the use (which also reverses the hoist order again, so volatile does not split cse's REGISTER value numbering -- the wave-89 splitter is about memory). -fno-cse-follow-jumps cannot help in hindsight: gcc lays the if-body out as the FALL-THROUGH of the inverted compare, so def and use sit on one cse path with no jump followed. The impasse is now exact, and the two halves are mutually exclusive at every position in the body: the def must be INSIDE the inner loop and BEFORE any conditional branch or LICM will not hoist it (everything after the `if` is maybe_never, which is why the bottom-of-body spelling keeps both constants but never moves); and the def must be OUT of the use's fall-through path or cse reassociates 0x6140 with 0x06010000. Breaking it needs a JOIN between def and use inside the loop, and no C construct that survives the `jump` pass creates one here.

### Wave 96

Base: `sub_08039588.c` (87.2%, size-exact, first diff +0x17); confirmed the parked residual (ROM hoists `dst` after the
zero-trip guard and table base; draft computes it before the guard, j/dst in r3/r4 instead of the shared r4).
Pre-registration (same LICM first-use family as sub_08037A78) NOT confirmed: the def has to be inside the inner loop to
hoist, and every form that puts it there merges the constants (-8). Probed: fold-proof mask on j (`((u32)j<<16 &
0xffff0000)>>16`) in the def, in the use inline, and in the reordered-constant use: 40.7% / 43.0% (-8), the mask does not
split cse's merge of `0x06010000 + dst` because j is re-derived (not a held narrow operand) here; def in the `for`
condition as `k=0; a[k]!=0 && (dst=..,1)`: 6.4%; `(dst=..., a[k]!=0)`: 39.7% +12; def in the increment clause: 19.8%. No match.
Residual unchanged: the def cannot be both out of the use's EBB (pool words) and an inner-loop invariant (hoist).

### Wave 97

wave 97 (W97-U)
Base: `sub_08039588.c` (87.21%, size-exact, first diff +0x17), unchanged. Pre-registered hypothesis (a constant merged
across the loop is a lever-1/lever-5 case) NOT confirmed. Probed with spellings.py (all 43.02% -8 unless noted):
`dst` removed and the address written inline in the call as `(u8 *)(j*0x100 + 0x6140) + 0x06010000`,
`(u32)(...) + 0x06010000`, `0x06010000 + (u32)(...)`, `(j<<8)` form, `(u8*)0x06010000 + (...)`: all fold back to the one
`=0x6016140` word (cse folds `(x + C1) + C2` however the cast is placed). `dst` typed u32 / `(j<<8)`: same. `c = tbl[k]`
first then `dst = ..` in the inner loop: same. Copy-back step `nj = j + 1; ... j = nj;` (before dst / after dst / at the
top of the outer body): 35.2% / 36.4% / 24.4% at +4 (adds the copy the ROM has but moves the guard). `nj` before the inner
loop with the address inline: 51.7% -4 (best of the new probes, still short). A `vram = (u8*)0x06010000` local used as
`vram + dst`: 75.0% size-exact, first diff +0xC (the base gets held, worse than the draft).
Mechanism note: the ROM keeps `j+1` (r6) computed before the zero-trip guard and the `dst` sum after it, i.e. both are
loop.c hoists, and the two constants stay separate words. That needs the sum's def inside the loop AND out of the use's
cse path; no spelling tried does both.
Proposed summary tried: "inline / casted / u32 spellings of the VRAM address all fold to one constant word; copy-back
step for j moves the guard but not the hoist".

wave 97 (W97-AA)
Base unchanged (87.21%). Checked the twin lead: the ROM pool here has only three words (gUnknown_08090F30, 0x00006140, 0x06010000);
neither 0x6140 nor 0x06010000 is a neighbour symbol (no asm/ symbol at 0x0601xxxx, VRAM is not a linked object), so the sub_08073228
trick (name a second symbol at offset 0) has nothing to name. Re-read the diff: the whole residual is the ROM computing
`j*0x100 + 0x6140` AFTER the zero-trip guard and reusing j's register (r4) for it (j+1 kept in r6 across), while ours computes it before the
guard into r4 with j in r3. No new probes beyond W97-U's list.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.

</details>
