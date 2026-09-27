# sub_0808AAF4

0x0808AAF4, 152 bytes, THUMB, parked.

Best score so far: 94.1% (best.c).

## What it does

Reads the save flash chip's ID. It sends the flash chip's enter-ID-mode command sequence, waits, reads the device and maker codes through a tiny read routine it copied onto the stack, sends the exit sequence, waits again, and returns (device << 8) | maker.

## How close it is

Measured with the settings its flash-code neighbours use (-O1, force-addr off; no override entry yet): 148 bytes against 152 (4 short), 28% of bytes match; the score means little because the gap starts in the first delay loop and shifts everything after it. The whole difference is one register copy per delay loop: the original loads 20000 into one register and copies it into the loop's register.

## What is left

No source spelling reaches that copy: with the constant used once the compiler drops the copy, and with it used twice it keeps the constant in an extra saved register across the calls; the original is neither. This needs a reading of how the compiler treats a single-use constant (per-pass debug dumps), not more spellings, and once it matches the function needs the flash block's -O1 override.

## Already tried

- Default -O2 settings: right size, but the address of the delay counter is kept across both calls in an extra saved register (59.9%); -O1 removes that.
- Starting the delay with `i = 20000; goto test;`: a separate first store, 4 bytes longer; a goto into the body so one store does both is kept.
- A separate counter variable for each delay loop: two stack slots, 4 bytes longer.
- Passing the constant through an extra variable (int or u16, either role, with a dead extra assignment or a dead copy): the copy disappears.
- One constant variable shared by both loops: the copies appear, but it takes an extra saved register.
- Storing 20000 through the pointer and reading it back: the copy appears but costs an extra store and load per loop.
- `do { } while (0)` between the two loops: no change.
- A plain static delay helper: not inlined; the caller then saves more registers.

## Files

- `sub_0808AAF4.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

152 bytes SIZE-EXACT, 59.9% identical under --profile configured (default -O2). WAVE 79 (W79-D) improved this from 28.3% at +4 bytes; the entry's long-standing claim of 'size-EXACT at 152' was STALE -- the draft as found compiled to 156 bytes.

### What still differs

ONE callee-saved register. The ROM opens `push {r4, r5, lr}`, the candidate `push {r4, r5, r6, lr}`. The extra register holds &i, the address of the volatile delay counter at sp+64: the candidate computes it once before the first delay loop and keeps it live across both `bl _call_via_rN` calls, so it must be callee-saved. THE ROM RECOMPUTES IT -- `add r1, sp, #64` appears TWICE, once before each delay loop, and the ROM correspondingly carries the 20000 delay constant as TWO separate pool words, one in each of its two mid-function pools. So the ROM has two independent short live ranges where the candidate has one long one. This is the wave-17 'binding locals are punctuation' diagnostic read backwards, and it IS a named source construct, not an allocno tie.

### Why it is close

Size-exact with the loop shape now correct. See resolution_partial for what wave 79 fixed.

### Already ruled out

- WAVE 79: `int v` instead of `u16 v` for the shared-store temporary -- 144 bytes (-8), 23.7%. The u16 keeps the size right; do not widen it to remove the truncation.
- WAVE 79: giving each delay loop its OWN block-scoped `vu16 i` and `u16 v`, the obvious way to shorten the address's live range -- 156 bytes (+4), 44.1%. gcc gives the two locals two stack slots, so the frame and the offsets both move, where the ROM uses sp+64 for both loops. The two live ranges must come from ONE variable, so the lever has to be something that stops CSE carrying the address across the calls, not a second declaration.
- WAVE 79: the -O1 branch of the toolchain axis, which is what closed sub_0808AC7C and sub_0808B5B8 in this same wave and is therefore the one configuration worth re-testing here -- o1 and o1-no-force are BOTH 160 bytes (+8), 38.2% and 34.2%, i.e. strictly worse in size than the default. Also measured on the pre-wave-79 source: no-force 28.3% at +4, old-agbcc-no-force 28.3% at +4. Default -O2 is right for this function; it is the exception in its block, and wave 60 already recorded that it gets LONGER under -O1.
- TOOLCHAIN AXIS (wave 46, W46-I): old_agbcc is byte-for-byte IDENTICAL to the default here -- it does not change one instruction. That finding stands, unlike the same sweep's conclusion about sub_0808AC7C, because -O1 genuinely IS worse here and W46-I's -O2-only enumeration therefore happened not to matter.
- Reading `_call_via_r5` as an argument count. See docs/agbcc-codegen.md, '`_call_via_rN` for a CALLEE-SAVED N is not an argument count at all'.

### Resolution partial

THE DELAY LOOP'S INITIALISING STORE AND ITS DECREMENTING STORE ARE THE SAME STORE. The ROM sets up r0 = 20000 and then branches INTO the loop body, landing on the `strh`, so one `strh r0,[r1,#0]` serves both the initialisation and every decrement. Spelled: v = 20000; goto store; body: v = i - 1; store: i = v; if (i != 0) goto body; with u16 v and vu16 i. The natural i = 20000; goto test; body: i--; test: if (i) goto body; emits a separate initialising store and is 4 bytes longer. Worth 4 bytes and 31.6 points across the two loops. Same family as the goto-into-the-body lever that was worth 12 bytes on sub_0808B5B8's inner loop.

### Notes

THIS IS SELF-RELOCATING CODE: it calls sub_0808AD6C, which copies four bytes of a sibling's machine code onto the stack, then computes r5 = sp+1 (the THUMB entry point of those bytes) and calls it twice via `bl _call_via_rN`, holding that pointer live across both calls and both delay loops. That lifetime genuinely forces one callee-saved register; the open question is only why the candidate needs a SECOND one. The ROM emits its literal pool MID-FUNCTION twice, each jumped over by a `b.n`, holding 0x0E005555 / 0x0E002AAA / 0x00004E20.

### Wave 88

WAVE 88 (W88-A then W88-D): the park's central claim is REFUTED -- this is NOT the flash block's -O2 exception. The 'gets +8 longer at -O1' note was measured on a draft whose `u16 v` was itself the +8. Under `o1-no-force` (the profile ALL sixteen matched flash entries actually use: -O1 with -fforce-addr removed) the residual is ONE register-copy of the delay constant per loop, everything else byte-identical; W88-D measured 148 B (-4) / 28.3% under o1-no-force. Mechanism settled: nothing is coalesced -- `update_equiv_regs` DELETES a single-use constant pseudo's defining insn, so agbcc has exactly two states (one use: def deleted, no copy, push {r4,r5,lr}; two uses: copies survive but the pseudo crosses the calls, takes r6, pool load hoists to one) and the ROM is neither. Nine spellings measured; the `do { } while (0)` lever was an exact no-op (not a regression); dead second def, dead copy-out and role-swap are exact no-ops (confirms the W80-F bound). Evidence: work/sub_0808AAF4/W88-notes.md.

### Wave 93

WAVE 93 (W93-A): 59.87% -> 92.76% (installing the better of the two files already in the work directory) -> 94.08% SIZE-EXACT at 152 from a 900 s / 4-thread permuter run on the DEFAULT profile. First difference still +0x0. Draft replaced; wave-start copy in sub_0808AAF4.w93-start.c, pre-permuter copy in sub_0808AAF4.w93-base.c. THE CONFIGURATION CONTRADICTION IN THIS ENTRY IS SETTLED -- DEFAULT -O2 IS RIGHT AND THE wave88 KEY IS WRONG. On the same source: configured 92.76% size-exact, o1-no-force 28.29% at size-4. The wave-88 claim that o1-no-force is 'the profile ALL sixteen matched flash entries actually use' is true of those entries and false of this function: the -O1 run in data/compiler-overrides.json begins at sub_0808AB8C, this function sits before it, and sub_0808AD6C inside the same span carries only cflags_remove [-fforce-addr] at -O2. The flash library is several files, not one. NO OVERRIDE IS NEEDED HERE; do not measure it at -O1 again. THE PERMUTER'S KEPT MUTATION IS SEMANTICALLY SOUND and was audited statement by statement: a second pointer to the same counter (`ptest = &(*p);`, i.e. `ptest == p`) whose only use is the SECOND delay loop's exit test, `if (*ptest != 0)`. `p` does not change inside that loop, so the test is identical at runtime. Renamed from new_var and re-verified at 94.08% after the rename. MEASURED BY HAND AND BYTE-NEUTRAL, WHICH KILLS THE OBVIOUS FIX: the wave-17 'one local where the original had N' lever does NOT reach the remaining residual. Splitting the single `vu16 *p` into two pointer locals, one per delay loop, is byte-identical to the baseline; so is splitting `int v` into v1 and v2; so is doing both. CSE knows every one of them holds the same stack address and coalesces them back, so the number of SOURCE variables is not the number of pseudos here. Consistent with the wave-89 splitter list: the fold-proof mask splits cse's value numbering of an address computation with a NARROW VARIABLE OPERAND, and `sp + 64` has no variable operand. W79's other direction (two block-scoped `vu16 i` objects) gives two stack slots and +4, so the two live ranges must come from one object AND one source variable. REMAINING RESIDUAL, 9 of 152 bytes: the ROM opens `push {r4,r5,lr}` and recomputes `add r1,sp,#0x40` before EACH delay loop, carrying two pool words for 20000; the candidate still computes that address once and keeps it in a callee-saved register across both `bl _call_via_r5` calls. NEXT: chain another permuter run from the 94.08% draft -- this was the first run ever made on this function and it improved on its first attempt.

</details>
