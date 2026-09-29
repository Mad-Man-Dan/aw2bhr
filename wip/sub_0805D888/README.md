# sub_0805D888

0x0805D888, 508 bytes, THUMB, parked.

Best score so far: 52.2% (best.c).

## What it does

AI: chooses where the active unit moves. If the unit can use its own tile (sub_0804236C), it calls sub_0805D648 there first. Then it scans every reachable free tile that sub_0804236C accepts. It scores terrain type 8 at +8, terrain in the gUnknown_085767F2 table at +4 and any other tile at +0, and keeps the best. If a helper unit exists (sub_0805C290) and a free tile of terrain type 0x11 turns up, it stops at once and calls sub_0805D648 with mode 0x14 and the helper's position. Otherwise it calls sub_0805D648 with the best tile.

## How close it is

Compiles to the right size but only 52% identical, and the draft needs an empty asm statement to get that far. Without the asm statement and with the compiler's gcse pass turned off (-fno-gcse), every byte matches except 2.

## What is left

Either establish that this part of the game was built without gcse, or find source that stops the 'found' call after the loop from sharing work with the call inside the loop. Under -fno-gcse the 2 remaining bytes are the first test of the y loop: the original compares against bestY (known to be 0 there), the draft against y.

## Already tried

- Loop counters as s16, cast spellings at the calls, copies of x and y before the found call: all still share the work.
- A 'found' flag with break instead of goto: loop body right, but 8 bytes too long from leftover flag tests.
- Calling the found case inside the loop: fixes the y-loop test but brings back the extra loop variable (4 bytes too long).
- Other compiler profiles (-O1, no force-addr, the older compiler): no match.

## Files

- `sub_0805D888.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

52.2% identical, 508 against 508, size-exact (wave 77 W77-C draft with empty-asm barriers at the body call).

### What still differs

ROM frame 20 / draft 16. The ROM spills bestX to sp+4 and spends sl on the 0x1f mask hoisted into the INNER loop preheader (one pseudo; all three `ands` read it via `mov r0, sl`); the draft keeps bestX in sl and rematerialises `movs r0, #31` per site. The ROM's y-loop entry guard compares bestY's spill slot (`ldr r3, [sp, #8]; cmp r3, r0`) -- cse replacing y (known 0) with its 0-class representative.

### Already ruled out

- s16 x, y loop counters (W49); five cast spellings at the body call (W66); separate s16 copies (W62 +44; re-probed W80: s16 copies still emit the ashift/ashiftrt pair -- extendhisi2 expands as two shift insns, so loop.c still sees `(ashift biv 16)` and reduces it)
- explicit fixed mask/row-offset registers (W76, 544)
- shared update block `v = ...; if (v > best) goto update;` -- WRONG SHAPE: the three compares merge into one block the ROM does not have; the three inline compares are settled source (W80-E)
- inline found block / copies at the goto site / fx-fy at the label: all still CSE-merge the exit conversion with the body's (W80-E)
- old-agbcc and o1 profiles (W62)

### Why it is parked

One construct left to find: a spelling of the found: exit call whose (s16)x / (s16)y cannot be CSE-merged with the body call's conversions while still reading the raw loop counters (the ROM recomputes the shifts from r4/r6 at the exit). Everything else -- frame, guard operand, mask-vs-bestX allocation -- follows mechanically once the two givs are gone. The barrier draft stays the byte-closest candidate (52.2%, size-exact) but blocks the permuter.

### Park kind

ORDINARY NEAR-MISS -- size-exact, one register-pressure residual, mechanised block by block in wave 80 (W80-E).

### Wave 80 finding

THE BARRIER IS MASKING A CORRECT LAYOUT (W80-E, all by compile_probe). Deleting the asm barrier and calling sub_0804236C(x, y) directly reproduces every structural feature of the ROM at once -- frame slot order (bestX sp+4, bestY sp+8, flag sp+12, e sp+16), the bestY-slot guard compare, the update blocks -- EXCEPT that loop.c strength-reduces (x << 16) (into sl) and (y << 16) (spilled), adding two frame slots (28) and displacing the mask. The givs survive because the found: block's (s16) conversions are CSE-merged with the body call's; the `mov ip, sl` / `mov r9, rN` pair after the call in the barrier-free build is loop.c's not-replaceable copy for exactly that merged use, and the exit block then reads the copies. The ROM's found block instead recomputes `lsls rN, #16; asrs` from raw r4/r6, so the original source keeps the exit conversion OUT of cse's reach from the body. No spelling probed does that: int fx/fy at the label, fx/fy copies at the goto site, the found call written inline in the loop, and s16 sx/sy copies at the body call ALL still merge (four compile_probes, wave 80).

### Wave 86

WAVE 86 (W86-D, constant-twin axis): twin c_0805E440.c (three shared callees) supplies NO new construct -- it does the OPPOSITE of what this park needs: it calls sub_0804236C(x, y) and sub_0805D648(x, y, 10, 0, 0) from inside one loop with plain int counters and its header states the x<<16 in r8 IS strength_reduce's giv for the s16 conversion, i.e. it wants the CSE-merged givs this function must avoid. No probe spent. Configured, 508/508, 52.2%, unchanged.

### Wave 91

W91-B. Member form: the draft already uses struct Map members, so it is a NEGATIVE by construction. NEW, and it changes the park reason: the found block's conversions are merged by GCSE (PRE), not by CSE. Under a temporary -O2 -fno-gcse profile the barrier-free draft (work/sub_0805D888/w91-nogcse.c) is SIZE-EXACT at 64.76%, with no giv, the mask in sl and the ROM frame. The ONE residual is 2 bytes at the y-loop guard: the ROM compares bestY's slot (ldr r3,[sp,#8]) where the candidate compares y. The -da trace puts it in cse2's class-head choice (make_regs_eqv): y's REGNO_LAST_UID is the found block at the textual end, which is later than bestY's last use. cse1 forwards goto-site fx/fy copies, so they do not help. The inline found call fixes the guard but brings back the y<<16 giv (+4). Under the configured flags, a found flag (fnd=1; break; if (fnd) break;) removes the giv (first diff +0xa -> +0x33) but costs +8 in dead flag tests (w91-flag.c). The barrier draft stays the configured best. Settle -fno-gcse together with sub_0805D438.

### Wave 96

Base: unchanged draft (52.17% size+0, first +0xa; still has the empty asm barrier).
Lead's un-binding / barrier-deletion probe measured in a one-unit harness at configured:
- barrier deleted: 542 bytes (+36 incl. pad), `sub sp,#28` (ROM #20, draft #16) -- the barrier is not masking a correct layout; deleting it makes the frame wrong in the other direction.
- barrier deleted AND sx/sy un-bound (call with x,y directly): identical to the previous line (cse folds the copies).
- same, plus the found block calling with x,y directly: identical.
Under `-fno-gcse` (a diagnostic profile only) the barrier-free source is size-exact 508 at 64.76%, first diff +0x7a (the y-loop guard);
`for (y = bestY; ...)` is byte-identical to `for (y = 0; ...)` there (bestY is the constant 0 and cse folds the copy).
So the earlier wave-91 reading stands: the barrier is a stand-in for the gcse/PRE merge, and the last 2 bytes are which register class head cse2 picks for y.
Proposed summary: left: y-loop guard compares y's register where the ROM compares bestY's spill slot; frame is 16 vs 20 with the barrier. tried += barrier removal (frame 28), un-binding sx/sy, y seeded from bestY (all folded).

</details>
