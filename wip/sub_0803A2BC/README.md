# sub_0803A2BC

0x0803A2BC, 124 bytes, THUMB, parked.

Best score so far: 44.4% (best.c).

## What it does

Draws two text items for a unit's type at column (a1 + 8) / 8: the text ids at offsets 2 and 4 of the unit-type record gUnknown_085D5ABC[a2->type] (0x5c-byte records), on rows 7 and 13, through the text drawer sub_08014A5C with the buffer gUnknown_08499578.

## How close it is

Compiles 4 bytes short (120 of 124 bytes); 23.4% of bytes line up, which means little because a missing instruction near the start shifts everything after it. From the first table load onward the code is instruction-for-instruction right. The saved 78.2% 'best' is permuter output with a nonsense signature and is not usable.

## What is left

The original computes the column in seven instructions with two extra copies, and copies a2 out before touching a1; every C spelling gives six, which leaves each later register one off. If resumed, try a statement order that uses a2 before a1, which the original's first instruction hints at.

## Already tried

- About a dozen ways of splitting `(a1 + 8) >> 3` into locals (in place, three single-use locals, bound once, inline at both calls, reverse declaration order, an int parameter with a cast): all give the same six-instruction start.
- Loading `a2->type` into a local first, or aliasing a2: moves the load earlier, the wrong way.
- The automatic permuter, about 140,000 tries over three runs: reaches the right size (78.2%) only with a nonsense signature, then stalls.
- Looking at other callers of the same text function for a solved example: they pass only constants, nothing to copy.
- Checking the text function's prototype for an argument narrowing: all parameters are full width, so there is nothing to exploit.

## Files

- `sub_0803A2BC.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

WAVE 79 (W79-C) MEASURED THE DRAFT: 120 bytes against 124 (-4), 23.4% identical, first difference at +0xc. The 78.2% this entry used to quote is best.c-s. best.c is permuter output carrying the signature sub_0803A2BC(unsigned long long a1, ...) and header-expanded prototypes: size-exact at 124 but not promotable and not a draft. The honest hand draft is the -4 one.

### What still differs

ONE INSTRUCTION in the argument head, plus 2 bytes of literal-pool padding. The ROM head is SEVEN instructions: adds r6,r1,#0 (a2 copied out of r1 FIRST, before a1 is touched at all), lsls r0,r0,#24, lsrs r1,r0,#24 ((u8)a1 into a scratch), adds r4,r1,#0, adds r4,#8, adds r0,r4,#0, lsrs r4,r0,#3. Every candidate emits SIX: adds r4,r0,#0, adds r7,r1,#0, lsls r4,r4,#24, lsrs r4,r4,#24, adds r4,#8, lsrs r4,r4,#3 -- doing the +8 and the >>3 IN PLACE on the parameter-s own promoted pseudo. The ROM has THREE pseudos where the candidate has one: (u8)a1 dies immediately into a call-clobbered r1, a1+8 dies immediately, and only the >>3 result lives across the two calls in r4. Because the candidate-s single pseudo takes r4 at once it never needs r1 freed, so a2 is copied SECOND (into r7) and the 0x8000 constant takes r6 -- the ROM has a2 in r6 and 0x8000 in r7, which is why every later register name is one off. Everything from the first pool load onward is instruction-exact.

### Why it is close

Not assessed. The high percentage is POSITIONAL and says only that the allocation diverges late; per the wave-58 ordering rule it is not evidence that little is left.

### Already ruled out

- SUPERSEDED. -Nothing was ruled out- was true of THIS ENTRY only, never of the function: waves 35 (W35-E) and 43 (W43-I) both worked it and their evidence sat in work/sub_0803A2BC/sub_0803A2BC.c the whole time. Waves 35/43 ruled out: u32 i; i = a1 + 8; i >> 3; three locals u8 v / u32 w / int i; those three declared in reverse order; int a1 with the cast at the use; and about 140k permuter iterations over three runs (63.7 -> 74.2 -> 78.2, then flat).
- Wave 79 (W79-C), NINE MORE SPELLINGS by compile_probe, every one byte-identical to the draft-s six-instruction head: y = a1; y += 8; y >>= 3 (in place, so the single-set nonzero_bits rule does not apply); y = a1 + 8u then i = y >> 3; y = a1 + 8u with y >> 3 written inline at both calls; u8 c / u32 y / u32 i as three single-assignment locals; i = (a1 + 8u) >> 3 bound once; the asymmetric best.c shape written honestly (y = a1 + 8u used by one call, (a1 + 8u) >> 3 inline at the other); u8 v = a2->unk00 hoisted; a struct Unk08499594 *p = a2 alias; and u32 a1 with ((u8)a1 + 8u) >> 3. Only the a2 hoist changed anything, and it moved the ldrb EARLIER rather than moving the copy, which is the wrong direction.
- Wave 79 (W79-C) CLASSIFICATION: a COPY-COALESCING decision with no source handle. No C spelling makes agbcc keep (u8)a1 and a1+8 as separate short-lived pseudos, because every spelling of a single-use chain lets local_alloc coalesce them into the parameter-s pseudo; and lsrs rD,rS,#imm5 is a 3-operand form, so the ROM-s adds r0,r4,#0 before the shift is a non-coalesced copy that no correct allocation would emit and no expression tree requests. Under the wave-77 screen this is -register numbers with nothing behind them-. Do not spend another wave-s budget on the head.

### Notes

Wave 79 (W79-C) -- THIS IS NOW AN INVESTIGATED PARK; the position-only wave-59 text is replaced. THE HARNESS QUESTION THE OLD NOTE ASKED IS ANSWERED: W79-C surveyed all 155 entries and this was the ONLY never-investigated one. 66 entries are bare strings and 7 dict entries carry an empty axes_ruled_out, but every one of those still records a measured score and a named wave, so they are investigated parks that merely lack the structured field. The -uninvestigated entry hides an unworked function- failure is therefore a ONE-OFF, not a class -- but it still cost this function twenty waves of invisibility, and the fix is the one the old note proposed: a position-only entry must never be written into parked.json at all, because parked.json is the exclusion list every pre-flight reads. Note for the record that the named exemplar src/decomp/c_0803A338.c is the matched CALLER (sub_0803A2BC(gUnknown_0849D89C->unk00, gUnknown_0849D89C->unk04)); it confirms the parameter types and nothing else, and cannot address an allocation residual -- exactly what the wave-57 rule about named exemplars predicts.

### Wave 87

WAVE 87 (W87-C, unnamed-twin axis): twins sub_080059FC / sub_08005AA0 (src/decomp/c_080059FC.c, shared callee sub_08014A5C + gUnknown_08499578) LACK the construct -- all eleven of their sub_08014A5C calls pass integer LITERALS in argument 1, both are `void f(void)`, no computed argument anywhere. Prototype check also negative: include/unknown-functions.h:6450 `void sub_08014A5C(int, int, void *, int, int, int)` (all six wide, read off the callee's prologue) agrees with the promoted callers, so `(a1 + 8u) >> 3` is passed with no narrowing and there is no argument-narrowing lever. Clean NO, 0 probes, 0 try_match. DO NOT batch this on the shared callee again: the sub_08014A5C / gUnknown_08499578 vocabulary is a UI-drawing hub shared by dozens of literal-argument functions and carries no information about the head residual. Configured, 120/124 (-4), 23.4%, unchanged.

### Wave 96

Base: `sub_0803A2BC.w96-start.c` (23.4%, -4). The four missing bytes are two register copies in the prologue. The ROM
keeps the incoming u8 in its own pseudo (`lsls/lsrs` into r1), copies it to r4, adds 8, copies the sum to r0, and shifts
into r4: sum and shifted column are separate pseudos and the widened argument is a third. The draft widens in place in
r4 and shifts in place. Also a2 lands in r6 in the ROM and r7 in the draft (r7 holds 0x8000 there).
Tried, every one byte-identical to the draft (23.39%): locals for `x = a1`, `sum`, `col` in every split
(u32, int, u8 temp; sum and col on separate statements; `>>=`); the shift spelled `/ 8u`, `(u32)(a1+8) >> 3`,
`((u32)a1 + 8) >> 3`. Single-block locals collapse in cse/flow before local_alloc, so the copies are not reachable this way.
Declaring the parameter `int` conflicts with the header's `u8` prototype. Not tried: permuter.

### Wave 97

wave 97 (W97-U)
Base: the 23.39% -4 draft. Hand probes (spellings.py): `i = (a1+8u)>>3` as call argument, `(i = a1+8u)>>3` at both calls,
`a1 - -8u`, `>> 3u`, `/ 8`, `i = a1 + 8u; i >>= 3;`, `(u32)(a1+8) >> 3`, `i = a1;` then `(i+8u)>>3`: ALL byte-identical to
the draft (120 B, 23.39%; `>> 3u` 22.6%). One 500 s permuter run: 23.39 -> 44.35%, size-exact 124, kept as
`_w97-longlong-44pct.c`: `unsigned long long new_var = (a1 + 8u) >> 3;` passed to both calls. wrongc: same behaviour on 400
seeds, so the value is right, but it is a size PAD, not the ROM's structure: the extra pseudo is a 64-bit one, the
constant 0 is no longer held in r5 across the calls (two `movs r0,#0`) and a2 is still r7 not r6. NOT adopted; the draft
stays the clean 23.39% form. Reading the ROM head: `adds r4,r1,#8` is not encodable (imm3), so the `adds r4,r1,#0;
adds r4,#8` pair is natural once the zero-extended argument sits in r1 and the sum in r4 -- the missing thing is only that
the parameter pseudo must NOT be the one that lives across the calls (in the ROM the widened argument dies in r1; the sum
and the shifted value live in r4/r0). Any spelling where one pseudo carries a1 across the calls (all of ours) coalesces.

</details>
