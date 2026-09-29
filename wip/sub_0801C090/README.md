# sub_0801C090

0x0801C090, 360 bytes, THUMB, parked.

Best score so far: 90.6%.

## What it does

Copies a sprite's list of OAM entries into the OAM buffer at the write cursor gUnknown_03002F2C, moving each entry by (a1, a2) and adding a4 to its tile number. When the horizontal-flip bit is set in a1, each entry's X is mirrored.

## How close it is

Compiles to the right size (360 bytes); about 79% of bytes line up.

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

</details>
