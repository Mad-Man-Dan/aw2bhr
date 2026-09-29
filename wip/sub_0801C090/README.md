# sub_0801C090

0x0801C090, 360 bytes, THUMB, parked.

Best score so far: 93.6%.

## What it does

Copies a sprite's list of OAM entries into the OAM buffer at the write cursor gUnknown_03002F2C, moving each entry by (a1, a2) and adding a4 to its tile number. When the horizontal-flip bit is set in a1, each entry's X is mirrored.

## How close it is

Compiles to the right size (360 bytes) with 93.6% of bytes identical. What is left is scratch register numbering in the mirrored arm (the width table's pool word and the 0x1ff mask copy). The draft still has a `long long nextCount` that should become plain C.

## What is left

Work out which registers the remaining values belong in. The frame and the counter's stack slot are solved; the original also keeps two source halfwords in registers across the mirrored branch where the draft only keeps some of them.

## Already tried

- The permuter, three chained runs: from 14% to 46% of bytes matching (a `do { } while (0)` around the loop, constants bound to locals), then only about a point per run and never a match. That result is in best.c as preprocessed output and contains a `(char)` cast that is probably wrong.
- Holding `src[0]` and `src[1]` in their own locals in each branch: 4 bytes closer in size but fewer bytes match, and the counter still stays in a register.
- Self-assignments of the counter, or of memory indexed by it, to lengthen its life: registers move around, but the counter never goes to the stack.
- Pinning values to fixed registers: the compiler spills the parameter instead of the counter.
- Ideas from the matched non-mirrored version, sub_0801BD00 (walking the a3 parameter directly, a different pointer-increment tail): no change. Its signed counter does not apply here, because the original treats this counter as unsigned.
- Assigning each 32-bit masked expression straight to a u16: the compiler shrinks the mask constant. Going through a `u32 t` local first, as the draft does, is required.

## Files

- `sub_0801C090.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 344/360 (-16), 14.2%. The authoritative readable draft is retained; best.c is contaminated preprocessed output. Count self-assignment is inert and memory-index self-assignments rotate registers without producing the ROM's spill slot. Residual is loop allocation/frame placement.

WAVE86: WAVE 86 (W86-F, vocabulary-twin axis): twin sub_0801BD00 (src/decomp/c_0801BD00.c) is a TRUE SHAPE TWIN and names three constructs the park never tried: (a) `s16 n` for the counter -- REFUTED FROM THE ROM without a probe: the decrement is `lsls #16 / adds 0xFFFF0000 / lsrs #16`, i.e. UNSIGNED, so `u16 count` is correct and the twin's declaration is a real difference between the functions (method note: the ROM's own shift settles signedness in ten seconds; do not transplant a twin's declarations wholesale); (b) walking the `void *a3` PARAMETER itself, cast at each use, instead of a fresh `u16 *src` local, and (c) `*dst++; *dst++; *dst = ...; dst += 2;` -- both transplanted in one probe: ALLOCATION-NEUTRAL, byte-identical in every figure. Configured, 344/360, 14.2%, draft unchanged (w86-start.c).

### Wave 93

WAVE 93 (W93-C). 14.17% at -16 -> 53.33% at size+0 (SIZE-EXACT), and the FIRST DIFFERENCE MOVED FROM +0xa TO +0xe, which is the park's whole point: +0xa IS the `sub sp, #N` instruction in this prologue, so every draft for five waves has been failing on the frame and nothing else. Full table in NOTES.md.
THE BIGGEST SINGLE FACT: **W86's `u16 count` IS WRONG AND THE TWIN'S `s16` WAS RIGHT.** Two chained permuter runs from the 33.33% base changed exactly one thing in the body -- `u16 count` to `short` -- and that one declaration is worth 8 bytes and 15 points and reproduces the ROM's loop bottom instruction for instruction, including the `0xFFFF0000` pool constant. W86 recorded `s16 n` as 'REFUTED FROM THE ROM without a probe', reasoning that the decrement ends `lsrs` not `asrs` so the counter is unsigned, and generalised that into 'the ROM's own shift settles signedness in ten seconds'. MEASURED: a `short` counter ALSO ends in `lsrs` here. The trailing shift is the 16-bit TRUNCATION of the result and is unsigned for both declarations, because the only use is `!= 0`. What actually discriminates is the `0xFFFF0000` add, which only the signed form emits -- and it was in the same seven instructions all along. The matched twin sub_0801BD00 declares its counter `s16`; transplanting it wholesale would have worked. Recorded in docs/agbcc-codegen.md. METHOD: a reading of the ROM is a hypothesis, and `compile_probe` is free and does not count as an attempt. A refutation recorded without one becomes a ruled-out axis that stops later waves looking, which is what happened here for six waves.
THE FRAME RULE APPLIES BUT POINTS THE WRONG WAY. The ROM has one slot more than the draft, which by the W93-B rule suggests a `volatile` local. It is wrong here and the ROM says so without a probe: the slot is written `str` and read `ldr` (word), while a `volatile u16` is a 2-byte object compiling to `strh`/`ldrh`. A word slot holding a zero-extended u16 is an ALLOCATOR SPILL. Added to docs/agbcc-codegen.md: read the ACCESS WIDTH before applying the frame rule -- slot the size of the declared type means a volatile object, slot the size of a register means a spill, and they need opposite levers. (The same dump re-confirms W86: the decrement ends `lsrs`, so `u16 count` is right and `short count` is not.)
WHAT CREATES THE SPILL, measured nine ways: a zero-trip `do { } while (0) round the loop ALONE does nothing (-16, +0xa); extra bound locals in the body ALONE do nothing (-16, +0xa); TOGETHER they spill the counter (+0xe). The nest takes the counter's live range out of local_alloc; the pressure makes it lose once it is there. It is a conjunction, and a wave that tries one half and sees nothing has measured half a lever.
NEWLY RULED OUT, and it retires three park lines at once: `while`, `for (;;) { if (count == 0) break; ... }` and `if (count) do { } while (count)` are BYTE-IDENTICAL -- same size, same first difference, same percentage to the hundredth. gcc normalises loop rotation long before allocation. Do not spend probes rotating this loop.
RESIDUAL: size-exact with the frame and the counter's slot correct. This is now an ordinary register-allocation residual, which is the permuter's case -- for five waves it was a structural difference the permuter could not reach, which is why the chains before this wave gained a point per run.
BASES REJECTED: `recovered.c` (43.33%, size-exact) and `best.c` (45.83%, -8) are not wrong C -- the `(char)` cast is on a 0..12 value and the `inline_fn` is an identity -- but both reach their size with padding and their first difference is +0xe, the same as the honest spelling. The extra bytes buy nothing.

### Wave 94

W94-A adopted Vesly's local draft (75.56% size-exact), then three chained runs: 78.33, 79.44, and a third stopped unfinished by the orchestrator (draft restored to the 79.44% pre-run copy). Kept change audited: `remaining`, already scratch in the loop body, holds the negated width; it is recomputed before the loop test.

### Wave 97

wave 97
Base: `sub_0801C090.c` (79.44%, size-exact; `recovered.c` is 43% and was not used). Final: **90.56%**, size-exact, first difference +0x10 (unchanged offset; the differing bytes below it shrank from 74 to 34).

Hand steps (each measured with a one-unit harness, `build/probe/w97k.py`):
1. The mirrored arm's `remaining |= 0xffffff00 & 0xffff;` compiled to `movs #255; lsls #8` (the constant folded to 0xff00). The ROM loads `0xFFFFFF00` from the pool, ORs, and truncates. Spelling it `remaining |= 0xffffff00; remaining = (u16) remaining;` reproduces the ROM's `ldr; orrs; lsls #16; lsrs #16` and the pool word.
2. The ROM negates the width BEFORE the attr0 computation and shifts back down inside the attr1 expression (`lsls; negs` early, `asrs` late). Written as `neg = -(remaining << 16);` ahead of attr0 and `sum = (neg >> 16) + x; sum += (s16) negWidth;`. The split `sum +=` is load-bearing: written as one expression, combine sees that `& 0x1ff` discards the sign extension of `(s16) negWidth` and drops the `lsls/asrs` pair the ROM keeps (size 356 instead of 360). `hi = (x | attr1) & ~0x1ff` as its own statement puts the ROM's order (mask part before the sum).
3. Permuter run 1 (80.8 -> 86.4): `hi` declared u16 instead of u32. Run 2 (86.4 -> 89.7): `(y | sourceAttr0) & ~0xff` split into its own int; the run also added an int zero compared in the loop test, which I removed (measured byte-identical without it). Run 3 (89.7 -> 90.56): `remaining = sum;` before the final mask, reusing `remaining` as the scratch.

Negative: reading attr1 into a separate `srcAttr1` (the ROM does end with the merged attr1 in r2, not r4) made the size 364 and dropped equal halfwords from 140 to 29, so it is not the lever at this point.

Residual: register numbering in the mirrored arm (merged attr1 in r4, ROM r2; the 0x1ff mask in sl vs r7) and the prologue order of `str r3,[sp]` / `adds r5,r2,#0`. The parked entry's "two copies too many" was not settled: the copy delta is unchanged by anything above.

Proposed summary: does = copies a counted sprite template into the OAM shadow with optional horizontal mirroring; status = "360 bytes, size exact, 90.6%; register numbering in the mirrored arm differs"; left = "the merged attribute word sits in r4 where the original has r2, and the original stores tileOffset after copying the template pointer"; tried = the steps above, the srcAttr1 split.

wave 97 (second pass)
Base: 90.56% draft (`sub_0801C090.w97-second-start.c`). Now **91.67%, 360 B size-exact** (`sub_0801C090.c`; 30 bytes differ, first +0x10).
Lever: give the mirrored arm's loaded attr1 its own variable typed `int` (`int sa1 = src->attr1;` used for the shift, the 0x1ff mask, the 0x100 test and `hi = (x | sa1) & ~0x1ff`), so the merged `attr1` is a separate pseudo and ends in r2 like the ROM. `u16 sa1` costs +4 bytes (364, 20%), `u32 sa1` -4 (356); `int` is size-exact but shifts arithmetically (`asrs`), so the shift is spelled `(u32) sa1 >> 14` (91.39 -> 91.67).
Residual: the pool word of gUnknown_0848B56C (ROM r3, ours r0), the 0x1ff mask (ROM `ldr r7; mov sl,r7; mov r3,sl; ands r3,r4`; ours `ldr r3; mov sl,r3; adds r1,r4,#0; ands r1,r3`), prologue `str r3,[sp]` order, and the loop-bottom count add/compare pair (r0/r1 swapped). All reload/scratch numbering.
Permuter (900 s, 2 threads) from the 91.67% file: 91.67 -> 92.22. Two edits, both valid C (wrongc.py OK, 117 seeds): the `>> 14` cast spelled `(((u32) sa1) >> 14)`, and the count update goes through a `long long nextCount` temp (`nextCount = count * 0x10000 + 0xffff0000; remaining = nextCount;`). Adopted.

wave 97 (W97-AB)
Base: 92.22% draft (`sub_0801C090.w97ab-start.c`, size-exact, first diff +0x10). Now **93.61%, 360 B size-exact, first diff +0x41**.
Lever 1 (by hand, 92.22 -> 93.33): walk the `template` PARAMETER itself instead of a fresh `u16 *src = template;` (`count = *(u16 *) template; template = (u16 *) template + 1; ... template = (u16 *) template + 3;`, `((struct SpriteTemplateEntry *) template)->` at every use). This is the wave-86 twin idea, which was allocation-neutral on the old base; on the current base it puts `adds r5,r2,#0` before `str r3,[sp]` like the ROM (the parameter pseudo itself becomes the walker, so its entry copy is emitted with the other parameter copies). The prologue difference is gone and the first difference moved +0x10 -> +0x41. Lesson: re-test a twin transplant after each big move; a negative on one base is not a negative on the next.
Lever 2 (permuter, 560 s, 93.33 -> 93.61): `new_var = ~0xff;` bound at the top and used in the non-mirrored arm's attr0 mask. Valid C (wrongc OK, literal WARN only).
Negatives: `sa1 & 0x1ff` instead of `0x1ff & sa1`, the commuted `0x1ff & remaining` at the second use: byte-identical; table pointer first in the width sum: +4 bytes, 28.9%. `levers.py --chain 3` found no improving lever on the 92.22 base.
Residual (all scratch numbering in the mirrored arm): the width table's pool word loads into r0 (ROM r3, i.e. the ROM's load is live across the r0 chain, so RTL order of the load differs); the 0x1ff mask is copied through r3 / sl (`ldr r3; mov sl,r3; adds r1,r4,#0; ands r1,r3`) where the ROM has `ldr r7; mov sl,r7; mov r3,sl; ands r3,r4`; the hi-mask temp is in r4 not r1; the loop-bottom count is in r2 not r1.
Note: the draft still carries a `long long nextCount` temp for the count update (adopted earlier this wave; wrongc OK, frame unchanged). A plain-C spelling that matches would be preferable.
Proposed summary status: size-exact, 93.6% identical; left: the mirrored arm's scratch registers (width-table pool word r0 vs r3, the mask copy path, one temp).

</details>
