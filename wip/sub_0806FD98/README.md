# sub_0806FD98

0x0806FD98, 76 bytes, THUMB, parked.

Best score so far: 7.0%, +24 bytes.

## What it does

m4aSoundVSync from the MP2K sound library, called once per frame. If the sound info block that gUnknown_03007FF0 points at has a valid ident, it counts pcmDmaCounter down and, when it reaches zero, reloads it from pcmDmaPeriod and restarts the two sound DMA channels (DMA1 and DMA2).

## How close it is

Cannot be scored on its own: its first two constants sit in a later function's constant pool, more than 600 bytes past its end, which a function compiled alone cannot reproduce. Apart from that the draft is 24 bytes longer than the ROM's 76.

## What is left

Unpark it when this sound-library region is promoted as one file. Before then: settle whether pcmDmaCounter is really volatile (check that the matched sub_08070AF8 still matches with a plain u8; plain u8 here removes 4 of the 24 bytes), and find spellings for the bit-25 test (the ROM shifts the bit into the carry flag) and for the ident check (the ROM subtracts a positive constant, which suggests the source held it in a variable).

## Already tried

- A local temporary for the counter decrement, with the shared volatile type: still an extra dead load. `--p->pcmDmaCounter` is worse, three loads.
- The older compiler: bit-identical output; -O1: 4 bytes worse.
- hardware.h's separate DMA1CNT/DMA2CNT macros: two base addresses instead of the ROM's one (the local DMA struct is kept).
- The bit-25 test: every spelling tried gives a mask and compare instead of the ROM's shift.
- `p->ident - MPLAY_ID_NUMBER` written plainly: the compiler stores the negated constant and adds it.

## Files

- `sub_0806FD98.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

not scorable -- see reason

### What still differs

The first two `ldr`s are `ldr r0, _08070044` and `ldr r2, _08070048`, i.e. PC+680, while the function ends at 0x0806FDE4. Those two pool words live 600+ bytes past the end of the function, inside a LATER function's pool, and this function also carries a local pool for the two constants its tail uses. A function compiled on its own can only emit its own pool, so the `ldr rN, [pc, #680]` encodings cannot be reproduced at any spelling: the differing bytes are the PC offset, not the code.

### Why it is close

The C is believed right -- it is the m4aSoundVSync shape, the fe_matches index agrees, and the 'Smsh' guard plus the pcmDmaCounter/pcmDmaPeriod reload are the canonical MP2K ones. This is a POOL park, not a codegen park: check every pool word's address against the function's extent before spending attempts, and unpark it when this region is promoted as a whole file.

### Already ruled out

- WAVE 47: the local-temp spelling `t = p->pcmDmaCounter - 1; p->pcmDmaCounter = t; if (t > 0)` does NOT rescue the `volatile` qualifier. With `vu8` it still emits a dead `ldrb` between the `subs` and the `strb` -- four instructions where the ROM has three. `--p->pcmDmaCounter > 0` is worse again at three loads. So the wave-33 contradiction between this function and the matched sub_08070AF8 stands; my hypothesis for dissolving it is refuted.
- hardware.h's separate REG_DMA1CNT/REG_DMA2CNT macros -- they CSE to 0x040000C4 and 0x040000D0 instead of the ROM's single 0x040000BC base with the 12-byte DMA stride

### Settled

- `subs r3,r3,r2; cmp r3,#1; bhi` is ONE unsigned expression, `(ident - MPLAY_ID_NUMBER) > 1`, not two equality tests.
- The ROM pools 0x68736D53 POSITIVE and subtracts it. The honest spelling does not do that: agbcc canonicalises subtract-a-constant into add-the-negative and pools the NEGATED value. A positive pool word plus a register `subs` means the constant reached the expression from a register, i.e. the source held it in a variable -- the same idiom as the 0xFFFF mask in sub_08070CD0.
- `ldr r1,[r2,#8]; lsls r1,r1,#7; blo` is a single-bit test of bit 25 shifted into the carry; `blo` is carry-clear, so the store runs when the bit is SET.

### Why it is parked

Blocked by a literal pool that sits outside the function's own extent. Not a source problem and not scorable per-function; it needs whole-file promotion of the region.

</details>
