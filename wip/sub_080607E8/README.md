# sub_080607E8

0x080607E8, 172 bytes, THUMB, parked.

Best score so far: 79.7% (best.c).

## What it does

Spawns up to three units in a row. It looks up record 7 with sub_0803E354 to get a cell (x, y + 4); for each of the cells x, x + 1 and x + 2 on that row, if no unit stands there and the factory schedule gFactoryUnitSchedule (three unit-type bytes per row, row chosen by the low five bits of gUnknown_03004080) names a type, it creates that unit with sub_08025CC8, clears the unit's bytes 9 and 10, and sets byte 11 from PickWeightedAiUnit(type - 1).

## How close it is

Compiles 8 bytes short of 172; 54.7% of bytes are in place. Three instructions are missing, all around the unit-creation call: the ROM re-narrows the second argument to 16 bits where the draft's compiler proves that unnecessary, and the ROM loads a fresh 0 for byte 9 where the draft reuses a register already known to hold 0.

## What is left

Find a way of writing b (the row plus 4) whose 16-bit narrowing the compiler cannot prove redundant; the other missing instruction and the swapped registers should follow. The permuter has never actually searched this function: the inline-asm line that made it refuse is no longer in the draft, so a run is now possible.

## Already tried

- Seven spellings of the table index (`band * 3 + i`, a two-dimensional array view, a struct wrapper and others): all let the compiler move `band * 3` out of the loop, which the ROM does not. Writing the loop as a plain goto loop fixed that (kept; do not turn it back into a for loop).
- A packed 3-byte row struct: gives the right row size but the multiply is still moved out of the loop.
- u8 or int for band, c and d; `continue` against nested ifs; merging the two tests with &&: no help. d must stay u8.
- `u16 b` instead of int: byte-identical.
- Splitting b's definition into two statements: gets 4 bytes back but moves b and the loop counter into other registers, 30.2% overall.
- Raw byte arithmetic on gUnknown_08499590 for the map reads: adds the offsets in the wrong order; the file-local struct cast is right (kept).

## Files

- `sub_080607E8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

164 bytes (-8), 54.7% identical (was 32.0% before wave 77). THE HOIST IS FIXED; the residual is now THREE INSTRUCTIONS.

### What still differs

WAVE 77 (W77-J) CLOSED DEFECT (1). A PURE GOTO LOOP -- `i = 0; _loop: {body} i++; if (i <= 2) goto _loop;` with NO for/while/do construct -- is invisible to loop.c, so LICM never runs on it, and the candidate now emits the ROM's in-loop synthesis `mov r3,rBAND / lsl r0,r3,#1 / add r0,rBAND / add r0,r0,r7 / add r1,r1,r0` AND the ROM's high-register mask shuffle, exactly. Defect (2) went with it: `unk09 = 0` is still cse'd onto the known-zero c, so that knock-on was NOT caused by the hoist and the old entry's explanation of it is refuted. THE REMAINING -8 BYTES ARE THREE INSTRUCTIONS, all in the sub_08025CC8 call block: (a) the ROM sign-extends the SECOND argument, `mov r2,r8 / lsls r1,r2,#0x10 / asrs r1,r1,#0x10`, where the candidate emits a bare `mov r1,rB` -- combine proves the narrowing redundant because b = (u8 load) + 4 has >= 17 sign-bit copies, and the ROM's compiler did not; (b) the ROM materialises `movs r0,#0` for `u->unk09 = 0` where the candidate reuses r6, the register holding c, which the `c == 0` guard proved zero. Everything else is byte-exact. Band and b hold swapped high registers (candidate band=r8/b=r9, ROM band=sb/b=r8), which follows from the missing instructions rather than being independent.

### Why it is close

The shape is fully derived and exactly ONE compiler decision is wrong. The lever is known and half-demonstrated: casting the table to a 3-byte-row STRUCT (`((struct Row3 *)base)[band].v[i]`) stops the hoist dead and reproduces the ROM's sb allocation AND its mask shuffle exactly, first try -- but agbcc pads `struct { u8 v[3]; }` to 4, so the stride comes out `lsls #2` and the association becomes `(base + band * 4) + i` instead of the ROM's `base + (band * 3 + i)`. What that measures is the underlying rule: agbcc's LICM hoists a TWO-instruction invariant (the *3 synthesis, lsl + add) but leaves a ONE-instruction one (the *4 shift) in the loop. So the job is a spelling that keeps a 3-byte stride while making the multiply part of an addressing tree rather than a standalone MULT_EXPR in the index -- or a way to get agbcc to emit the *3 as one instruction.

### Already ruled out

- WAVE 61 -- THE PERMUTER CANNOT RUN ON THIS FUNCTION AS DRAFTED. permute.py aborts during setup with `Syntax error in base.c` at the `asm("" : "+r"(bb));` barrier: decomp-permuter parses the source with pycparser, which does not accept that extended-asm form. The run exits before any search happens, so an empty permuter log here means REFUSED, not SEARCHED-AND-FAILED. To permute this one, first find a spelling of the barrier the parser accepts (or drop it and re-measure from whatever that costs).

### Already ruled out

- SEVEN index spellings, all reassociating to the identical tree and ALL hoisting: `[band * 3 + i]`, `[i + band * 3]`, `[band * 2 + band + i]`, `[(band << 1) + band + i]`, `*(base + band * 3 + i)`, `((u8 (*)[3])base)[band][i]`, and `((struct T *)base)->rows[band][i]` (the struct-wrapped-array lever from the .rodata chapter). The struct wrapper only flipped the operand order of one `add`.
- u8 / int for band (u8 band also breaks the `ldrh` of gUnknown_03004080 down to a `ldrb`), for c, and for d -- d must stay u8, because the third sub_08025CC8 argument is a bare `adds r2, r5, #0` with no sign extension, which an int would need.
- the `continue` form against the nested-if form; `0x1f & g` against `g & 0x1f`; merging the two guards into one `&&`.
- Register pressure as the cause: the ROM and every candidate use exactly seven callee-saved registers and the identical push list, so the hoist is not agbcc running out of registers.
- WAVE 77 (W77-J): A PACKED 3-BYTE ROW STRUCT DOES give the 3-byte stride the entry asked for -- `struct Row3 { u8 v[3]; } __attribute__((packed));` is size 3, so `((struct Row3 *)base)[band].v[i]` emits `lsl #1 / add` and not `lsl #2` -- AND IT STILL HOISTS (164 B, 34.3%). That settles the mechanism the old entry only half-measured: the unpadded struct did not stop the hoist because it was a struct, it stopped it because the padded *4 stride collapsed to ONE insn. Any two-insn invariant is a scan_loop movable and gets hoisted, whatever the source spelling. The seven index spellings and the struct wrapper were never going to work; only taking the loop away from loop.c does.
- WAVE 77 (W77-J): on the surviving second-argument narrowing -- `u16 b` in place of `int b` is BYTE-IDENTICAL here (164 B, 54.7%, not one byte moved), so agbcc's local promotion does not make a u16-to-s16 conversion visible the way it does for a parameter. Splitting the definition into two sets (`b = p->unk01; b += 4;`) DOES cost 4 bytes back (168 B, -4) but pays for them elsewhere: b drops out of the high registers into r7 and the loop counter's allocation changes with it, net 30.2%. The narrowing and the register split are one fact, not two.

### Notes

The gUnknown_08499590 access is settled and must not be re-litigated: the file-local `struct Map7E8` cast reproduces the ROM's `adds r0,r1,r3(=0x417A); adds r0,r0,r2` and `adds r1,#0x12; adds r1,r1,r0` associations exactly, per the wave-34 rule in include/unknown-globals.h. Byte arithmetic on the u8 * does not. WAVE 77: the goto-loop form is now load-bearing and must not be tidied back into a `for`. W73-D's test for the goto lever -- does the body need a giv? -- is NEGATIVE here: the ROM rematerialises every address from band and i each iteration and carries no strength-reduced giv at all, which is why the lever that is mutually exclusive with a giv on sub_0806412C is free on this function.

### Wave 92

WAVE 92 (W92-B): no movement (54.65%, -8), but the residual is now closed arithmetically and the permuter result was REJECTED as wrong C. THE -8 IS FULLY ACCOUNTED FOR: the ROM narrows b to 16 bits before sub_08025CC8's second argument (mov r2,r8 / lsls r1,r2,#16 / asrs r1,r1,#16) where the draft copies in one instruction, +4 bytes; the ROM materialises movs r0,#0 for u->unk09 = 0 where the draft reuses c's register, +2; the code then reaches 170 and the literal pool's alignment needs the ROM's .short 0x0000, +2, giving 172. Nothing else in the function differs. The narrowing is requested (the prototype is (s16, s16, s16)) and combine deletes it because b = zero-extended byte plus constant has 23 sign-bit copies. The contrast that names the mechanism is the FIRST argument a + i, whose narrowing survives in both, because the loop counter gives combine no range. CORRECTING THIS ENTRY: the recorded claim that splitting b's definition gets 4 bytes back is true of the size and false of the reason. Compiled and diffed, the narrowing is STILL absent; the split only moves b out of a high register so the copy becomes adds r1,r7,#0, and the four bytes come from an unrelated register shuffle earlier in the loop. It is not a partial fix and the draft is the better base. Also measured negative this wave: u->unk0a re-read from the map, 16.11% at +8; that plus the split, +12; the map cell's address bound to a local with unk0a re-read through it, 25.57% at +4; the two stores swapped in source order, BYTE-IDENTICAL, so store order is not a lever either. PERMUTER (900 s, 4 threads, the first run ever on this function): reported 54.65% -> 80.81% at the ROM's exact size and kept that source. The mutation inserts a second p = sub_0803E354(7) INSIDE THE LOOP -- a dead assignment whose call cannot be deleted, worth exactly the missing 8 bytes, after which every later instruction lands on the ROM's address. The ROM's loop body has no such call, so the candidate would call it four extra times per invocation: a behaviour change, not a spelling. Rejected; draft, best.c and best.json restored. NOTE the last pool word's relocation prints as a difference (gUnknown_030046B4 against gFactoryUnitSchedule) and is NOT one: the map puts gFactoryUnitSchedule at 0x030046b4.

### Wave 94

W94-A: Vesly's 86.63% size-exact file was judged wrong C and quarantined (vesly-best.c.wrongc), as was a permuter run's 79.65% form (w94-perm1-7965.c.wrongc). Draft unchanged at 54.65%, 8 bytes short.

### Wave 97

wave 97 (W97-G)
Base: draft (54.65%, 164/172). `best.c` (79.65%) is WRONG C and stays rejected: it writes `band = c; i = band;`
inside the loop body, so `band` (loop-invariant, used for `band * 3 + i` next iteration) and the loop counter `i`
are both clobbered with the known-zero `c`. Its size-exact score comes from that clobber, not a valid twin.

Residual re-read: (1) the ROM keeps `lsls #16; asrs #16` on `b` at the sub_08025CC8 call; the draft drops it
(b = u8 field + 4, so nonzero_bits proves it fits). (2) `movs r0,#0; strb r0,[r4,#9]` vs draft `strb r6,[r4,#9]`
(cse substitutes the known-zero `c`).

Probes (trymatch, all restore to the draft): `b = p->unk01; b += 4;` -> 168 (-4) but 30% (i moves to r8, ext still
folded); `(s8)` on the field -> 168; `*(u8 *)((u8 *)p + 1) + 4`, `(u16)(...)`, `(u32)p->unk01 + 4`: byte-identical
to the draft (164). Two-set b does not restore the extension either, so the fold is not reg_n_sets-gated here.
Unresolved: what makes the ROM's `b` non-provable (value not from a ldrb+4 chain visible to nonzero_bits).

Proposed summary: does = spawns up to three factory units in a row at the map slot's row from the factory schedule.
status = 54.65% draft, 8 bytes short. left = missing sign extension on the y argument, `unk09 = 0` uses the zero
register instead of a literal. tried = see above plus waves 92/94; best.c is wrong C (clobbers band and i).

wave 97 (W97-Y)
Base: draft (54.65%, -8). levers.py's 81.4% (`s8` copies of d at both uses) is WRONG: the third argument of sub_08025CC8 becomes 0xFFFFFF8B for d >= 0x80 (wrongc, confirmed by reading); no other lever beat the draft.
Probes for the missing sign extension on b (`s16 bs = b` at the call, `s16 b`, `u16 b`, `(s16)(...)` on the assignment or at the call): all 164 bytes, the extension stays folded (nonzero_bits proves b fits); `s16 b` plus a separate `s16 bs; bs = b;` gives 168 at 30.8%. So the fold is not a copy-count or type issue on b.
Permuter (3 links, 500 s each): the kept "improvements" (63.4%, 65.7%, size 172) are PADDING, not progress: `new_var = i <= 2; if (new_var) goto` and a do { } while (0) around the loop make agbcc emit `movs r0,#0 / movs r0,#1 / cmp r0,#0 / bne` at the loop end (visible in the diff), replacing the ROM's `ble`. Rejected; draft restored. A size-exact score on this function is a padding artefact until the tail `ble` is reproduced.
Residual unchanged from W97-G: b's `lsls/asrs` before the call, `movs r0,#0` for unk09, and the sb/r9 band allocation.

</details>
