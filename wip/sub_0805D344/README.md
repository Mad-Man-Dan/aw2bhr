# sub_0805D344

0x0805D344, 244 bytes, THUMB, parked.

Best score so far: 80.7% (best.c).

## What it does

Sorts the zero-terminated list of unit ids in gUnknown_030045F0 that the list builders just filled. For each unit it stores a key, GetUnitMovementWithCoBonus(player gUnknown_030033EC, the unit's type), in gUnknown_030046E0, then bubble-sorts the two arrays together by that key: largest first when the argument is 0, smallest first otherwise.

## How close it is

The draft is now the plain form, 8 bytes short (236 against 244). The earlier right-size draft padded exactly those 8 bytes with an unused flag, so it was right-size by accident. What is left: the original keeps n in r8 for the whole function and the draft keeps it in a low register.

## What is left

Get n into a high register with every instruction around it unchanged. The recorded next step is a chain of permuter runs starting from best.c (currently the same as the draft); do not write the id list as a walking pointer, because in the ROM that pointer is one the compiler made itself.

## Already tried

- Reading the unit table pointer gUnknown_08499594 plainly, through a pointer cast, or as `(&g)[0]`: its address is reloaded on every pass. Only a volatile-qualified read makes the compiler load it once before the loop as the ROM does (kept).
- Binding the unit id to a local in the fill loop: moves one load to where the ROM has it but is worse overall (13.1%).
- Passing the second call argument inline instead of through a local: the loop body is one instruction longer (the local is kept).
- One 8-minute permuter run from the current draft: its best candidate scored 16.0%, worse than the draft.

## Files

- `sub_0805D344.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

13.5% identical, candidate 236 bytes (-8), first difference at +0xf. Re-measured by W59-E; unchanged since wave 57. NOTE work/sub_0805D344/best.json reads 13.9% for a DIFFERENT spelling in best.c -- gate on the draft's exit code, not on that number.

### What still differs

FOUR INSTRUCTIONS, all in the FILL loop, all one coupled decision. The ROM keeps the counter n in r8 (a HI register) and therefore pays 'mov r8,r0' for 'n = 0', 'movs r1,#1' / 'add r8,r1' for 'n++', 'mov r2,r8' before the guard's cmp and 'mov r0,r8' after it -- exactly the -8. The candidate keeps n in r5 and pays none of them. Freeing r5 is what lets the ROM LICM-hoist &gUnknown_08499594 into it ('ldr r5,=gUnknown_08499594' in the preheader, 'ldr r1,[r5]' in the body) where the candidate reloads that pool word inline every iteration. The hoist and n's register are the same fact; do not chase them separately.

### Why it is close

Everything from 'if (n > 1)' onward is byte-exact -- both loop guards, the stack spills, the ?: with its arms the right way round, the duplicated in-loop direction test, the inner descending loop, both six-access swaps including all four dead volatile loads, and the epilogue. The 'volatile u8 []' typing of gUnknown_030045F0 that this needs was made and re-verified in wave 57 against the twelve promoted builders (sub_0805CA60, sub_0805D2A0, sub_0805CDF0, sub_0805CE20 all still match after it).

### Already ruled out

- Wave 57: binding 'u8 id = gUnknown_030045F0[n];' in the loop body. It does move the ldrb ahead of the gUnknown_030033EC read as the ROM has it, but leaves both the pool ordering and n's register unchanged. Strictly worse at 13.1%.
- Wave 59 (W59-E): writing the id list as an explicit POINTER WALK ('volatile u8 *p = gUnknown_030045F0; while (*p != 0) { ...; p++; n++; }') to force the ROM's split between a pointer for gUnknown_030045F0 and an integer index for gUnknown_030046E0, and so push n out of the low registers. This is the obvious remaining idea and it is WRONG BY THE REPO'S OWN RULE, so it was not spent as an attempt: in the ROM's preheader the pointer's init 'adds r4,r1,#0' sits AFTER the LICM hoist 'ldr r5,=gUnknown_08499594', and per the wave-58 giv rule (the preheader is written by three passes in a fixed order: source, then LICM hoists, then strength_reduce's inits) anything after a hoisted invariant was written by the loop optimiser and must not be authored. The pointer is a giv. Authoring it would reproduce the wave-58 sub_0800CAA0 failure in reverse.
- Wave 59 (W59-E): what is left to explain is therefore an ASYMMETRY, not a source shape -- gcc gives gUnknown_030045F0 a pointer giv (r4, 'adds r4,#1') and gives gUnknown_030046E0 none (its base is rematerialised from the pool and added to n every iteration). Whatever makes the second array ineligible is the thing that frees r5 and pushes n to r8. Both arrays are volatile u8 and both are indexed by n; the visible difference is that gUnknown_030045F0 is also read by the loop's exit test.

### Wave 87

WAVE 87 (W87-A): 13.5% -> 16.4%, still 236/244 (-8), draft REPLACED (wave-57 draft in w87-start.c). The pre-registered giv-asymmetry probe was MOOT: the wave-57 draft ALREADY reproduces the ROM's split (pointer giv `add r4,#1` for gUnknown_030045F0, gUnknown_030046E0 rematerialised from the pool each iteration) and nobody had checked. Park claim REFUTED: the &gUnknown_08499594 hoist and n's register are TWO facts -- the hoist is now in the ROM's preheader position with n still in r5. What moved it: a VOLATILE-qualified read of the pointer global, `(*(struct Unk08499594 *volatile *)&g)[k].unk00` -- a non-volatile read of a pointer global leaves the address folded into the load (re-emitted every iteration, no invariant pseudo for LICM); a volatile read force_regs the address into its own pseudo, which LICM hoists (`ldr r5,=g` preheader, `ldr r1,[r5]` body, the ROM's form). Taking the address is not the lever; the qualifier is (doc chapter). Also required: bind the second call argument to a local before the call (`u8 x = ...; f(g, x)`) -- without it agbcc emits arg1 first and the body is 11 insns vs the ROM's 10. Header deliberately NOT retyped (20+ promoted users; the local cast gives the same code). Remaining -8 is ENTIRELY n's register (ROM r8, candidate r5): four 2-byte items. PERMUTER, first ever run here: `tools/permute.py --seconds 480 -j 4 --current` from the 16.4% draft ran to completion (exit 0) -- one candidate at permuter score 1400 re-checks at 16.0% (worse); a genuine negative for THIS draft, single run from one base (weak; wave 83 needed a chain of three). Next: chained runs from best.c; do NOT author the pointer walk (W59-E: a giv).

### Wave 93

WAVE 93 (W93-A): A FAITHFUL sub_0805D344 DOES NOT MATCH UNDER -fno-gcse, AND THE REASON IS STRUCTURAL, NOT A SPELLING. Measured: configured 16.39% size-8, first difference +0xf; --cflags-add=-fno-gcse 33.20% size-20, first difference +0xa. The score RISES and the function gets WORSE: at configured the whole sort half is byte-exact (the ROM's 12-byte frame, both compiler spills, the four dead volatile loads, the ip/sb/sl inner-loop addresses, the epilogue), and the ONLY remaining differences are n's register and the two stack slot numbers swapped. Under -fno-gcse that whole structure collapses: the frame drops to 4 with no spills at all, the inner swap loses its dead loads and walks two low-register pointers instead. The ROM's register pressure IS gcse's work here, so sub_0805D344 was built WITH gcse. CONSEQUENCE FOR sub_0805D438: since files are contiguous, the only file ending at sub_0805D438 that can carry -fno-gcse is sub_0805D438 ALONE -- flag_probe's 'sub_0805D338 .. sub_0805D438' is the maximal window, not the only reading, and sub_0805D344 sits between D338 and D438 so {D338, D438} is not a file. ALSO MEASURED, all at configured, all negatives: an explicit `m = n - 2;` local instead of writing `n - 2` in both loop headers is 17.21% size-8 but moves the first difference BACKWARDS to +0xa because it changes the frame -- the ROM's two stack slots are COMPILER spills of `n - 2` and `i + 1`, not source variables, so do not name either. Reusing `n` as the outer sort counter (with `m`) is 28.23% at size+4. THE PARK'S CLAIM THAT n's REGISTER IS THE ONLY DIFFERENCE IS WRONG: the fill loop also has three evaluation-order differences (the ROM loads the unit-table pointer AFTER the index arithmetic, adds it base-owns-destination, and loads the type byte AFTER arg0's pool load). All three are source-reachable and all three REGRESS -- binding a pointer to the type byte and dereferencing it at the call is 12.70%, computing `id * 12` into an int local and adding the volatile-read base to it is 13.11% (13.11% with sizeof). So the order differences are DOWNSTREAM of n's register, not independent facts. Draft unchanged at 16.39%; the wave-start copy is in sub_0805D344.w93-start.c.

### Wave 95

Base: draft (236, -8, 16.4%) kept as `sub_0805D344.w95-start.c`. best.c form (n reused as outer counter with `m = n - 2`) gave 28.2% at +4; the `for (n = 0; m >= n; ...)` spelling of it reached 33.9% at +4 but folds `m >= 0` into a branch the ROM does not have, so it was dropped. Copying n to a second variable by hand (`k = n; m = k - 2`) is byte-identical (copy propagates).
RESULT: SIZE-EXACT (244), 75.0%, first difference +0xf. Draft = `sub_0805D344.w95-perm3-start.c` = current `sub_0805D344.c`. Chained permuter: run 1 (600 s) 16.4 -> 72.1; run 2 72.1 -> 75.0; run 3 75.0 -> 80.7 was WRONG C (`n = n > 1; new_var = n;` clobbers the list length; kept as `.w95-WRONG-80.c`); run 4 75.0 -> 75.8 was `volatile unsigned a1` (parameter made volatile; kept as `.w95-volatile-a1-75_82.c`, not adopted).
What the two kept steps are (checked by reading, semantics identical to the start):
1. Lever 2 transfers: the address of the unit-table pointer is bound to a local once at the top (`new_var2 = &gUnknown_08499594;`) and the volatile-cast read goes through it. That took 16.4 -> 72.1 and made the size exact (the old draft was 8 short).
2. Lever 1 transfers in the form "copy the list length into a per-block variable for the sort" (`new_var3 = n; ... i <= new_var3 - 2 ... j = new_var3 - 2`): 72.1 -> 75.0.
Residual: n is still in a LOW register (r5) with a hoisted-address difference in the fill loop (`ldr r2,[r6]` before the index arithmetic; ROM loads the table pointer after). The ROM's n lives in r8. The 80.7% form got n into a high register only by destroying it, so a high register for n is reachable only if a second variable, not n, takes the flag / copy role.
Pool words: none new.
Proposed summary: status=size-exact, 75% identical, only n's register and the order of three loads in the fill loop differ; tried += "binding &gUnknown_08499594 to a local at the top with the volatile read through it: size-exact (kept)"; "per-block copy of n for the sort loops (kept)". Rename new_var2 -> unitTable, new_var3 -> count when promoting, re-checking bytes.

### Wave 96

Findings: (1) `best.c` (80.74%) is the known-wrong `n = n > 1; new_var = n;` form (byte-identical to `.w95-WRONG-80.c`); the drafts.py
"better" hint is a trap. (2) The 75.00% size-exact draft is size-exact only by accident: replacing its `new_var = n > 1; if (new_var)` with
plain `if (n > 1)` drops it to 236 bytes (-8, 18.4%). The 8 missing bytes are exactly n's four hi-register moves (+2 each), so n living in
r8 (ROM) is the whole real residual; the flag temp was filling the gap with junk (`movs r0,#0 / cmp / movs r0,#1 / cmp r0,#0 / beq`).
(3) Permuter run from the honest -8 form (900 s, 2 threads, `perm-w96-1.log`): 16.39 -> 70.90, size-exact, first +0xf. The kept change is
`long long new_var = n;` used as the index in the fill loop: valid C, same meaning, but it buys the 8 bytes with a 64-bit pair, not n in r8.
Saved as `sub_0805D344.w96-perm1-longlong.c`, NOT adopted. Draft `sub_0805D344.c` = the honest pure form (`.w96-perm1-start.c`, 236 bytes, -8).
Not reached: why r6/r7 are unavailable to n in the fill loop (ROM keeps ptr in r5, walker r4, n in r8). NEXT: `tools/rtldump.py` .greg conflicts for n.
Proposed summary: left: n in r5 where ROM keeps it in r8 (8 bytes); tried += flag-temp size padding is not real, long long index temp.

### Wave 97

wave 97 (W97-V)
Base: w87 draft (16.39% -8). levers 5a-55 = bind `n > 1` to a local (`big = n > 1; if (big)`); wrongc OK (400 seeds). try_match: 70.49% size-exact 244 B, first diff +0xf (unchanged: `n` in r5 not r8). The gain is SIZE only: the flag costs the 8 bytes that n-in-r5 saves (movs #1/cmp/beq), so it is padding, not the ROM's mechanism. Round-2 levers on the new base: nothing above 70.49%. `last = n - 2` bind changes frame (sub sp #8), `n >= 2` no change. Residual still n's register (r8). Kept the flag draft at sub_0805D344.c; pre-lever draft is sub_0805D344.w97v-start.c.

</details>
