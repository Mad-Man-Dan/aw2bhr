# sub_08068038

0x08068038, 172 bytes, THUMB, parked.

Best score so far: 41.3%.

## What it does

Draws a string as sprite glyphs and returns how many characters it drew. For each character it scans the table of 8-byte records gUnknown_08581160 (character code, then compressed graphics) from the global cursor gUnknown_0202F208 until the character equals the code or the code plus 0x20, and decompresses that glyph into sprite tile memory at 0x06010000 + index * 512 + a2 * 32; a 0xFF code ends the table and falls back to entry 12's graphics.

## How close it is

Compiles to the right size (172 bytes) with 41.3% of bytes identical. The table is read through a local set after the first store to gUnknown_0202F208, while the two reads of its data field stay on the global; binding every read makes the function too short. What is left is which values live on the stack and in which registers.

## What is left

Get the table and cursor addresses loaded once before the outer loop while keeping the goto search loop. Not yet tried: bind both addresses to locals before the loop and leave the first `gUnknown_0202F208 = 0` store using the plain global, the pattern that recovered a register copy in sub_08061DCC.

## Already tried

- The search loop as `for (;;)` with two breaks: the compiler repeats the 0xFF test at the loop bottom. The goto form fixed 24 bytes (kept).
- Dropping the `next` and `dst` locals: 156 bytes (16 short); the compiler turns the string index and the destination into walking pointers the ROM does not have.
- A goto loop with a plain `i++`: the same walking-pointer problem.
- `continue` against `goto bottom`: byte-identical.
- Chasing the extra constant gUnknown_08581164 on its own: it is the table address plus 4 and follows from which register holds the table address.

## Files

- `sub_08068038.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

36.6% identical, candidate 176 bytes (+4), first difference at +0xc -- in the frame. First measurement of this function; wave 59 (W59-E).

### What still differs

ONE ALLOCATION DECISION, and every other hunk follows from it. ROM: the str parameter is SPILLED to a stack slot ('str r0,[sp]', reloaded at all three uses), a2 * 32 lives in sb, &gUnknown_08581160 in sl, &gUnknown_0202F208 in r8 (held for the whole function) and i in ip -- four HI registers plus one stack slot. Candidate: str in sl, a2 * 32 spilled to the stack, and &gUnknown_0202F208 rematerialised from the pool after the call instead of held. The candidate carries one live value fewer than the original.

### Why it is close

The whole control-flow skeleton is byte-exact in place: the outer test, the inner scan loop over the global cursor, the 'goto advance' re-entry, the short-circuit ||, the CROSS-JUMP MERGE that gives both arms one shared 'bl Decompress', the 'ldr r0,[r0,#0x64]' fold for the sentinel's entry-12 data, and the epilogue. Every branch and every constant agrees. The scan cursor genuinely lives in memory -- gUnknown_0202F208 is zeroed before the loop, loaded at the top of every inner iteration and stored back on increment -- and no local reproduces that.

### Already ruled out

- Wave 59 (W59-E): DO NOT chase the '=gUnknown_08581164' pool word. It is not a separate global and not a second table: '&tbl[g].data' is '(g * 8) + (base + 4)', and which spelling agbcc picks is decided by where the base landed. With the base in a HI register (the ROM, sl) there is no 'adds rD,rHI,#4' available, so it forces a fresh pool word holding base+4; with the base in a LOW register (the candidate, r6) CSE reaches it with 'adds r0,r6,#4'. The pool word and the register assignment are ONE fact.
- Wave 59 (W59-E): dropping the 'next' and 'dst' binding locals -- writing 'i++' after the Decompress call and the destination expression inline at the call site, on the theory that both are inner-loop-invariant and so would be LICM-hoisted anyway. STRICTLY AND BADLY WORSE: 156 bytes (-16), 11.6%. Without the locals gcc strength-reduces BOTH walks, turning str into a walking pointer ('adds r4,#1') and the destination into a giv ('movs r0,#128' / 'lsls r0,#2' / 'adds r6,r6,r0'). The ROM carries NEITHER and recomputes 'str + i' and 'i * 512' from the index on every outer iteration. So the two locals are load-bearing. This is the sub_0800CAA0 direction of the induction-variable axis -- the CANDIDATE carrying givs the ROM does not -- and it is a useful counter-example to the usual 'binding locals are punctuation, split them' advice: here REMOVING binding locals cost 16 bytes.

### Wave 95

Base: existing draft (176/172, +4, 35.8%), kept as `sub_08068038.w95-start.c`. Result: SIZE-EXACT (172), 41.3%, first difference still +0xc (frame/spill choice). Draft at `sub_08068038.c` is the permuter's run-1 result, checked by reading: same C as the start except the table is read through a local `new_var = gUnknown_08581160;` bound AFTER the first `gUnknown_0202F208 = 0;` (the sentinel `[12].data` and the matching-entry `.data` reads stay on the bare global; the chr compares use the local).
- Pre-registered hypothesis (bind table and cursor addresses before the loop, first store bare): as written it OVERSHOOTS. Both bound: 160 (-12), 14.5%; cursor only: 164 (-8), 21.5%; table only: 160 (-12), 16.3%. Binding every table read removes the ROM's folded `gUnknown_08581164` pool word (member offset becomes `ldr [r3,#4]`). The permuter's mixed form (local for the chr reads, bare global for the two `.data` reads) is what restores the size: the bare reads keep the folded base+4 pool word.
- Chained permuter: run 1 (900 s) 35.8 -> 41.3, run 2 NO-IMPROVEMENT.
- Residual: str spill vs a2*32 spill and where &cursor lives (frame/reg allocation only; instruction multiset now close).
Proposed summary: does=renders a string glyph by glyph; status=size-exact, allocation differs; left=ROM keeps str on the stack, a2*32 in sb, table in sl and &cursor in r8 for the whole function; tried=binding table/cursor addresses (all reads: too small; mixed: size-exact).

### Wave 97

wave 97
Base: wave-95 draft (41.28%, 172 B), kept as `sub_08068038.w97-start.c`. No movement.
- Also binding the sentinel `[12].data` read to the table local: 180 B (+8), 19.4%. Also binding the matching `.data` read: 164 B (-8), 27.9%. Cursor pointer local bound after the first store: byte-identical. `off = a2 * 32` as a user local (before the loop or inside): 38.9% / 41.3%.
- Classified the residual from the disassembly: the candidate reaches the cursor through a `.rodata` force-addr word for the pre-loop store (`ldr r1,=word; ldr r0,[r1]; str r6,[r0]`), the ROM uses a plain pool word and then holds the address in r8 (`mov r3,r8; ldr r5,[r3]`). Same split as the wave-96 note; unsolved. The ROM also spills `str` to the stack and holds `a2*32` in r9, opposite to the candidate.

</details>
