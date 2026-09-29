# sub_0801C01C

0x0801C01C, 116 bytes, THUMB, parked.

Best score so far: 78.5% (best.c).

## What it does

Draws one sprite through PutSpriteExt at screen position (a1, a2). It takes an 8-byte OAM template a4 by value, ORs the 9-bit X into its upper half-word (OAM attribute 1) and the 8-bit Y into its lower half-word (attribute 0), passes the template's second word as attribute 2 and a3 as the sprite data pointer, and passes a5 through as the first argument.

## How close it is

Compiles to the right size (116 bytes) with 70.7% of bytes identical; what differs is which registers hold the values. The current draft came from an automatic search and was checked by reading.

## What is left

Find a way of writing it that keeps the shifted template word alive in a high register across the other calculations, as the original does. The odd 64-bit shift of a zero upper half, which the original clearly contains, is already reproduced by the in-place `u64 pair` local.

## Already tried

- Writing the attribute calculations with plain 32-bit locals: the compiler drops the original's 64-bit shift and the code was 40 bytes short.
- Using a literal zero or other casts to a 64-bit type instead of shifting the `u64` local in place: the 64-bit shift is again optimised away.
- The permuter on an earlier draft (over 160 saved results): no match.

## Files

- `sub_0801C01C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 108/116 (-8), 27.6%, improved from -40. A mutable in-place u64 shift reproduces the ROM's unique zero<<16 DImode path and high-half expression. Remaining eight bytes are r8/sl/r9 lifetime allocation; direct locals, literal zero and alternative DImode casts fold away.

### Wave 95

Base: sub_0801C01C.w95-start.c (27.6%, size -8). Best sound draft now sub_0801C01C.c = permuter run 1 output (70.69%, size exact, first difference +0x10); saved as sub_0801C01C.w95-perm1-out.c.

What moved it:
- Hand: computing the `(u16)pair` high half as its own variable and reading a4.unk04 (z) before x/y matched the ROM's order of temporaries (27.6 -> 28.4%), still -8.
- Permuter run 1 (900 s, from the hand draft): 28.4 -> 70.69%, size 116 exact. Read: the kept form splits the a4.unk00 load, the mask constant, the pair-to-long-long copy and the `<< 6 << 10` shift into extra pseudos (new_var*); statements are equivalent C (mask BEFORE the >> 16, same z/x/y).
- Permuter run 2: 70.69 -> 78.45% but WRONG C: it swapped `pair >>= 16; pair &= mask` to shift first, then mask with the sign-extended int constant, which zeroes the high half x should carry. Discarded (kept as sub_0801C01C.w95-perm2-out-WRONG.c). Not chained further.

Negatives: `int w = a4.unk04; ... (u16)w` sinks the load to the call (z must be computed before x/y); `u32` pre-shifted hi/lo temporaries and a `t` intermediate for z fold to the same code.
Parameter-width question: callers pass s16 values (c_08022DD4.c) and sub_08022BB8 matched with u16 first two parameters left alone, so int-vs-u16 was not needed.
Residual: register choice; ROM keeps lo<<16 in sb and the pair in r5/r6, our build uses low registers.
Proposed summary: does = as before; status = "116 bytes, size exact, 70.7%; only register allocation differs"; left = "ROM parks lo<<16 in r9 and the pair halves in r5/r6"; tried = above plus the wrong shift-first form.

</details>
