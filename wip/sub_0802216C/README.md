# sub_0802216C

0x0802216C, 560 bytes, THUMB, parked.

Best score so far: 39.5%, -8 bytes (best.c).

## What it does

Writes a 2x2 block of background tilemap entries for one army: a per-army base tile plus a graphic from sub_080261A4 on top, and status tiles chosen by flags below. Two of the army slots are drawn mirrored. What the picture shows (probably a unit icon with status marks) is not confirmed.

## How close it is

Compiles 12 bytes short (548 against 560), 25.0% of bytes identical. The draft no longer pins a variable to a register (the pin was not real source and blocked the automatic search); the pinned version, 8 bytes short, is kept beside it as sub_0802216C.w95-start.c. What is left is register assignment: the original keeps a2 in a register, 0x400 in its own register, and t and r the other way round.

## What is left

Three register choices are tied together: the original keeps a2 in a register instead of on the stack, keeps 0x400 in its own register, and gives t and r the opposite registers to ours. The permuter has no recorded run on this draft and is the next thing to try.

## Already tried

- Swapping the declaration order of t and r: identical output.
- Holding 0x400 in an ordinary local: the compiler folds it back into constants; no change.
- Pinning that local to the register the original uses: the constant is now set up once and shared, as in the original, but a2 is still stored on the stack and another value moves register.

## Files

- `sub_0802216C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 552/560 (-8), 39.5%. Fixed-r8 base binding correctly groups repeated 0x400 uses, but a2 still spills instead of occupying sl, leaving the frame and t/r allocation wrong. Declaration order and ordinary scalar base locals are byte-neutral.

### Wave 95

Base: sub_0802216C.w95-start.c (39.5%, size -8, `register int base asm("r8")` pin). Draft now has the pin removed (plain `int base;`, permuter-compatible); NOT scored or permuted this wave (permuter slot was used up by sub_0801C01C and sub_08022BB8's hand work). ROM detail read: a5..a8 and a3 are spilled at [sp,#0..#0x10] (five slots), a2 in sl, a4 in sb, 0x400 set in r8 at the start of the a3==0/0x80 arm. Next step: chained permuter run from the unpinned draft (never run on this function).

</details>
