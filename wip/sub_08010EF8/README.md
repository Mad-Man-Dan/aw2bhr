# sub_08010EF8

0x08010EF8, 64 bytes, THUMB, parked.

Best score so far: 79.7% (best.c).

## What it does

Writes a number into a tilemap buffer as decimal digits, right to left. It starts at `dest + x + 32` (one 32-entry row down, x cells across) and, for each digit, stores the digit plus the base tile of the digit glyphs (the first u16 of gUnknown_0300308C, reached through a pointer word at 0x0808DF8C), dividing by 10 until the number reaches 0. The second parameter is never read.

## How close it is

Compiles to the right size (64 bytes); 13 bytes differ. All of the difference is where one load sits: the original re-reads the pointer to the digit base tile on every loop pass, while our build reads it once before the loop.

## What is left

Get the compiler to keep that pointer read inside the loop instead of moving it out (loop hoisting). Every way of declaring the global gives the same code, so this is thought to have no fix in the C source.

## Already tried

- Using the plain name `gUnknown_0300308C[0]`: one instruction short, because the compiler loads the address directly instead of through the pointer word.
- Ten declarations of the pointer word (const pointer, plain pointer, volatile pointer, pointer array, const pointer array, sized array and more): all move the read out of the loop, and the volatile and plain-pointer forms also add an extra load.
- Copying the global into a local inside the loop, or indexing it with a variable (`g[value % 10]`): no change.
- Blaming the two division calls or the store through `dest` for the reload: ruled out, because our build has both and the compiler still moves the read out.

## Files

- `sub_08010EF8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

79.7% identical, SIZE-EXACT, 13 differing bytes

### What still differs

One instruction's POSITION, across a loop boundary. The ROM keeps `ldr r6,<pool>` in the preheader and re-reads BOTH levels inside the loop (`ldr r1,[r6]; ldrh r1,[r1]`); every candidate hoists the middle `ldr` into the preheader and leaves only the `ldrh` in the body. Same instruction count, hence the exact size.

### Why it is close

The body is not in question -- prologue, the `dest + x + 0x20` start cell, both libgcc calls, the digit arithmetic, the u16 re-narrowing and the exit test are byte-exact, and wave 51 re-derived the same body independently from the assembly.

### Already ruled out

- TEN declaration spellings across waves 42, 51 and 58: honest `gUnknown_0300308C[0]`; volatile array; sized array [2]; base bound to a local inside the loop; variable index `g[value % 10]`; `u16 *const` scalar; `u16 *`; `u16 *volatile`; `u16 *const volatile`; and (wave 58) `u16 *g[]` and `u16 *const g[]` measured side by side in one probe. The last two are byte-identical to each other AND to the scalar draft, which closes the wave-51 row that called the non-const array 'one LICM decision away'.
- Wave 58: call-clobbering and store aliasing as explanations. This loop contains TWO calls (__umodsi3, __udivsi3) and a store through a pointer parameter, and agbcc hoists a non-const, non-volatile global load out of it regardless. The two compilations differ in neither, so neither can be the mechanism.
- Not a permuter case: the difference is instruction ORDER across a loop boundary, not a register assignment.
- Wave 79 (W79-C) re-measured and CONFIRMED this entry unchanged: 64/64 size-exact, 79.7%, 13 differing bytes, first difference at +0x11. No new axis was opened, and the smallest budget in the batch was spent here deliberately. Two candidates were checked on paper and neither reaches it: the wave-79 u32-pointer-arithmetic lever that closed sub_080228D8 moves a pool ldr past an index scaling and says nothing about an LICM hoist; and the W40-J / W43-A chapters of docs/agbcc-codegen.md already explain why a constant subscript cannot be talked into the reloaded form -- in sub_08010ADC, the function that DOES get the reload, the index is an INDUCTION VARIABLE with strength reduction in play, which g[value % 10] is not. Treat this entry as closed.

### Settled

- Classification: constant placement realised as an LICM decision -- instruction ORDER across a loop boundary. This is the same unreachable family as sub_080373F0's basic-block layout: a choice made inside a compiler pass with no source-level handle, and no spelling of the declaration reaches it.
- `const`ness and `volatile`ness do not decide the hoist; all ten spellings hoist. The question was never how to spell the global.

### Why it is parked

Wave 58 (W58-A), carried from waves 42 and 51. Ten spellings measured; the residual is an LICM hoist, which is instruction order across a loop boundary and has no source lever.

### Wave 92

- **agent:** W92-A
- **measured:** 79.69%, size-exact 64/64, first difference +0x11 (tools/drafts.py bases; no try_match spent).
- **verdict:** Entry confirmed closed. The brief's instruction to 'fix the pool-word spelling' names an axis that is already the first line of axes_ruled_out: the honest gUnknown_0300308C spelling is one instruction SHORT.
- **new:** Three other functions in this batch closed part of their gap by naming the global directly, and agbcc answered each with the three-level chain this function needs, so the chain is reachable. The mechanism is already settled in docs/agbcc-codegen.md ("The `.rodata` force-addr word is made by GCSE's PRE", wave 91 W91-B): the .LC word survives only when the pool pseudo has more than one use, and PRE is what creates the extra uses -- and that chapter names this function's shape, a loop containing a call, as a case where PRE fires. So the open question is NOT which declaration to write. It is why, with the honest spelling, the pool pseudo ends up with ONE use here. Read it off a -da dump of the .gcse pass rather than spending an eleventh declaration. Note that -fno-gcse is already in the swept flag set and did not move this function, which argues against a PRE insertion being what the ROM has.

</details>
