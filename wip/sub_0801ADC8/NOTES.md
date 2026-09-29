
## Wave 94 (W94-A) - 76.26% -> 77.34%, size-exact

Renamed the wave-93 volatile temporary `new_var` to `keptFlags` (it holds
`unk20[i] | 8`). Byte-neutral.

Run 1: 76.26 -> 76.80. The mutation wraps the FIRST wipe-and-retry loop in
`do { ... } while (0)`. The inner `break` still binds to the `for (k ...)`
loop, so it is equivalent. Reformatted onto separate lines, which is also
byte-neutral.

Run 2: 76.80 -> 77.34. The mutation moves `unk00[i] = -1` ahead of
`unk10[i] = 0xff` and binds the `-1` to a local (renamed `erased`). The two
stores are to different members of the same struct with no call between them,
so the reorder is equivalent.

Vesly's fork also carries a draft for this function; it compiles to 64.57%,
below ours, so it was not adopted.

### Residual

556/556, 77.34%, first difference +0xa. Unchanged in kind: register allocation
and block placement around the two duplicated scan loops.

## wave 97

Base: `sub_0801ADC8.c` with the volatile removed (`sub_0801ADC8.w97-start.c` is the wave-94 file, 77.34%). **The `volatile unsigned long keptFlags` local of the old draft was wrong, not a lever**: it makes the frame `sub sp, #8` where the ROM has `sub sp, #4`, which is why the first difference sat at +0xa. Without it the frame is right; the volatile only "won" score by turning the loop into a different shape. Final: **77.88%**, size-exact, no volatile anywhere, first difference +0x80 (was +0xa).

Permuter from the honest base (65.83 -> 76.80 -> 77.88 -> none): the flag update `unk20[i] = ((unk20[i] | 8) | v) & 0xef` became `unk20[i] = unk20[i] | 8; unk20[i] = unk20[i]; unk20[i] = (unk20[i] | v) & 0xef;` (the self-assignment is load-bearing: it splits the memory value cse; plain `|= 8;` then the mask lands far worse), and later `new_var = (... | v) & 0xef; unk20[i] = new_var;`. Both are value-preserving.

Zero-operand question (why the ROM's `orrs r0, r3` with r3 = 0 is not folded): `v = 0` set in the else arm before the loop is folded to nothing when the use is one expression. `v = 0` at function entry keeps a register zero but costs +4 bytes and a `mov r8` (the value stays live across the calls). The split statements above keep the operand without either.

Residual: the flag loop counts up with an index where the ROM counts down with a walking pointer, and the PRE hoist takes `&unk10[i]` where the ROM takes `&unk00[i]`.

Proposed summary: does = tidies the 16 save slots after a load, re-erasing bad sectors and rewriting the slot table; status = "556 bytes, size exact, 77.9%"; left = "the flag loop counts down with a walking pointer in the original; register allocation in the two retry loops"; tried = zero-operand placements, the volatile flag temp (wrong: frame +4), scratch splits.
