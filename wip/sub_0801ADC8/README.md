# sub_0801ADC8

0x0801ADC8, 556 bytes, THUMB, parked.

Best score so far: 77.9%.

## What it does

Tidies the save slots in flash memory. It marks the parts of the newest complete save as kept (writing a fresh save with sub_0801A7D8 if there is none), then wipes or frees every slot that is not kept.

## How close it is

Compiles to the right size (556 bytes); about 77% of bytes line up.

## What is left

Find a way of writing the two identical wipe-and-retry loops that makes the compiler pick the original's registers and block order. The flag update that was missing, `(unk20 | 8 | v) & 0xEF` with v a zero held in a register, is already solved by declaring v as `int`.

## Already tried

- Declaring the zero operand as `u8 v = 0`: the compiler drops the OR and the code is 4 bytes short.
- Other ways of writing that zero (setting it before a call, in both arms of an if, inside an earlier loop, as `0 & global`, or as `(v & 0x10)` / `(v * 0x10)`): all drop the OR.
- Leaving v uninitialised: the compiler combines `v | 8` once before the loop, which the original does not do.
- A zero-filled local array indexed by the loop counter: the OR survives in the right shape, but it adds a load.

## Files

- `sub_0801ADC8.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at exact size 556/556, 64.6%. The retained int zero restores the otherwise missing OR operand; remaining differences are duplicated-retry allocation/block placement. Earlier u8, uninitialised, array, global-derived and arithmetic-zero spellings are ruled out.

### Wave 93

First permuter run ever (W93-E): 64.57% -> 76.26% size-exact. The kept form reads `unk20[i] | 8` into a volatile local before the 0x10 test in the second loop and uses it inside; audited by the orchestrator: same meaning, no read before set. The volatile local is what moved it (a volatile local always gets a stack slot). Next: chain further runs from this form.

### Wave 94

W94-A: two chained runs, 76.26 -> 76.80 -> 77.34 size-exact. Kept change audited: -1 through an int local and the unk00 store moved before the unk10 store (independent arrays).

### Wave 97

wave 97
Base: `sub_0801ADC8.c` with the volatile removed (`sub_0801ADC8.w97-start.c` is the wave-94 file, 77.34%). **The `volatile unsigned long keptFlags` local of the old draft was wrong, not a lever**: it makes the frame `sub sp, #8` where the ROM has `sub sp, #4`, which is why the first difference sat at +0xa. Without it the frame is right; the volatile only "won" score by turning the loop into a different shape. Final: **77.88%**, size-exact, no volatile anywhere, first difference +0x80 (was +0xa).

Permuter from the honest base (65.83 -> 76.80 -> 77.88 -> none): the flag update `unk20[i] = ((unk20[i] | 8) | v) & 0xef` became `unk20[i] = unk20[i] | 8; unk20[i] = unk20[i]; unk20[i] = (unk20[i] | v) & 0xef;` (the self-assignment is load-bearing: it splits the memory value cse; plain `|= 8;` then the mask lands far worse), and later `new_var = (... | v) & 0xef; unk20[i] = new_var;`. Both are value-preserving.

Zero-operand question (why the ROM's `orrs r0, r3` with r3 = 0 is not folded): `v = 0` set in the else arm before the loop is folded to nothing when the use is one expression. `v = 0` at function entry keeps a register zero but costs +4 bytes and a `mov r8` (the value stays live across the calls). The split statements above keep the operand without either.

Residual: the flag loop counts up with an index where the ROM counts down with a walking pointer, and the PRE hoist takes `&unk10[i]` where the ROM takes `&unk00[i]`.

Proposed summary: does = tidies the 16 save slots after a load, re-erasing bad sectors and rewriting the slot table; status = "556 bytes, size exact, 77.9%"; left = "the flag loop counts down with a walking pointer in the original; register allocation in the two retry loops"; tried = zero-operand placements, the volatile flag temp (wrong: frame +4), scratch splits.

</details>
