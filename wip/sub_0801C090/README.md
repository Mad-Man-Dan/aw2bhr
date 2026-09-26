# sub_0801C090

0x0801C090, 360 bytes, THUMB, parked.

Best score so far: 45.8%, -8 bytes (preprocessed form, not included).

## What it does

Copies a sprite's list of OAM entries into the OAM buffer at the write cursor gUnknown_03002F2C, moving each entry by (a1, a2) and adding a4 to its tile number. When the horizontal-flip bit is set in a1, each entry's X is mirrored.

## How close it is

Compiles 16 bytes too short (344 against 360). The instructions are the right ones; the difference is that the original keeps the loop counter in a stack slot (with a larger stack frame) and reuses two source halfwords already in registers, while the draft keeps the counter in a register and reloads the halfwords.

## What is left

Make the compiler put the loop counter on the stack as the original does. That probably needs one more value live at the same time somewhere in the loop body, and no draft has found it.

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

</details>
