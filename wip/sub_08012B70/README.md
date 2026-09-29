# sub_08012B70

0x08012B70, 88 bytes, THUMB, parked.

Best score so far: 87.5% (best.c).

## What it does

Copies a rectangular block of tilemap entries into a tilemap buffer (callers pass gBG0TilemapBuffer) at column x, row y, adding `add` (a palette and tile-base offset) to every entry. The block's first u16 holds the width in its low byte and the height in its high byte; the entries follow row by row, and the destination rows are 32 entries apart.

## How close it is

Compiles to the right size (88 bytes); 11 bytes differ. The instructions are the same as the original in the same order; only the register numbers differ, for four values: `dst`, `src`, the row base and the row pointer.

## What is left

The original keeps `dst` alive only until the row base is computed and then reuses its register for the row pointer. The draft needs a way to copy `dst` early (as the original does) without making that copy the long-lived row base.

## Already tried

- Giving the row base its own local (five spellings): the registers split the right way, but the early copy of `dst` disappears and the code is 4 bytes short.
- The permuter, four runs of about 43,000 tries in total plus two more later: it stopped improving at the current draft. Its two useful edits (a `do { } while (0)` wrapper and `y * 0x20` in an `int` local) are kept.
- The older compiler build and every other compiler profile: worse, or the same at best.
- Taking parameter 2 as `void *` and walking a `u16 *` local: the copy of `src` moves to the wrong place.
- Declaring parameters 3 to 5 as `int` with casts at each use: loses the narrowing at entry that the original has.
- Writing the offsets as subtraction of a negative (the trick that matched sub_0802FA64): worse, loses the copy of `src`.
- Reordering `*p = *src + add` or the two pointer increments: no change.

## Files

- `sub_08012B70.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

87.5% -- 11 of 88 bytes differ, size EXACT (wave 39, W39-A; RE-MEASURED and unchanged in wave 73 by W73-B and again by W73-G)

### What still differs

The 43-instruction stream is 1:1 with the ROM: same opcodes, same order, same immediates, same branch structure, same `mov ip, r0`. EVERY differing byte is a register NUMBER, in two linked pairs. (1) dst and src are swapped -- draft r5/r4, ROM r4/r5. (2) the row base and the row pointer are swapped -- the draft keeps the base in callee-saved r5 (it is the SAME pseudo as dst, which is reassigned) and puts p in scratch r2, where the ROM puts the base in scratch r2 and recycles dst's now-dead r4 for p. w->r6, h->r7, i->r1, j->r3 and add->ip already match exactly, as does everything from `lsrs r7, r0, #8` onward apart from the register numbers.

### Why it is close

The type model and the statement order are settled and independently corroborated rather than guessed: the split PROMOTE_MODE pair fixes the parameter widths (and the retype was confirmed by re-verifying all three callers byte-for-byte), and the position of the src prologue copy fixes parameter 2 as a directly-walked pointer. No statement-order or type error remains to find.

### Already ruled out

- old_agbcc with -fprologue-bugfix removed -- MEASURED, not assumed, via a throwaway data/compiler-overrides.json entry: 85.2%, WORSE than the default toolchain's 87.5%. It keeps the same wrong register assignment AND additionally emits `ldrb` before `ldrh`. Consistent with the wave-38 discriminator, which locates the two allocators' divergence at a CALL inside a loop body -- this function has no calls at all.
- A separate local for the row base (`d = dst + x; d = d + y * 0x20;`). This DOES produce the ROM's fresh-pseudo base and would free dst's register for p, but it always costs the `adds r4, r0, #0` prologue copy: with dst read-only, gcc loads the stack parameter into r1 rather than r0, dst never leaves r0, and the function comes out 42 instructions / 4 bytes SHORT. Probed in five spellings -- own local in one statement and in two, base assigned back into dst, the mixed `dst = dst + x; d = dst + y * 0x20;`, and declaration order both d-before-p and p-before-d -- all 42 instructions. THIS IS THE CORE TENSION: the prologue copy needs dst long-lived, the base register needs dst dead.
- A `void *` second parameter with a `u16 *s = src;` local -- puts the src prologue copy seven instructions later, after the stack argument's narrowing, instead of at instruction 3. Not reachable by statement reordering, because parameter setup always precedes a body statement.
- `int` parameters 3-5 with explicit `(u16)` casts at the uses -- loses both prologue `lsls`, because the narrowing is then emitted whole at the use.
- decomp-permuter, FOUR runs totalling roughly 43,000 iterations: 60.2 -> 72.7 -> 86.4 -> 87.5%, and then a full 600 s run starting from 87.5% produced nothing better at all. It has CONVERGED; do not simply re-run it longer. Its two surviving edits are already in the draft and are worth keeping -- a zero-trip `do { } while (0)` around the second offset add plus `src++`, and `y * 0x20` bound to an `int` local -- together worth 14.8 points.
- `*p = add + *src` operand order, and the `p++` / `src++` order inside the inner loop -- byte-neutral.
- Toolchain axis re-measured across all seven profiles, confirming and extending the old_agbcc note: configured/default/no-force all 87.5% (identical); o1 and o1-no-force 8.0% at -4 bytes; old-agbcc and old-agbcc-no-force 85.2%. Nothing beats the configured profile.
- SUBTRACTION OF A NEGATION (`dst - (-x)`, `p = dst - (-(i * 0x20))`), the spelling that matched sub_0802FA64 this wave by making a symbolic base operand 1: NEGATIVE here, and it makes things worse in the exact direction the entry's 'core tension' predicts. Both the row-pointer-only and the both-sites variants compile identically to each other and LOSE the `adds r4, r1, #0` src prologue copy -- src stays in r1 and is walked from there, i is promoted to callee-saved r4. Measured by compile_probe.
- decomp-permuter re-confirmed CONVERGED: two further runs this wave (300 s from the draft with --current, 300 s from best.c) both reported base score 70 and no candidate better than the starting point.

### Settled

- Parameters 3, 4 and 5 are u16, NOT the `int` they were declared with. A prologue `lsls rN, rN, #0x10` whose matching `lsrs` is MISSING -- it reappears at the use fused with the scale, `lsrs #0xf` for `dst + x` (*2) and `lsrs #0xa` for `y * 0x20` (*0x40) -- is a sub-word PARAMETER, whereas an `int` with an explicit `(u16)` cast emits the narrowing whole at the use and leaves no prologue `lsls`. Retyped in include/unknown-functions.h; sub_080399F8, sub_0807FE90 and sub_08080498 were all re-run through try_match and are still byte-identical.
- Parameter 2 is a directly-walked `u16 *`, not a `void *`: `adds r5, r1, #0` is a PROLOGUE copy sitting ahead of the `ldr r0, [sp, #0x14]`, which only happens when the parameter itself is the walking pointer.
- gUnknown_080A31A4 is `u16 []`, not `u8 []` -- sub_08012B70 is its only reader and walks it as halfwords. Byte-neutral at all three call sites, which pass only the base address.
- w and h come from ONE halfword, not two byte loads: `hdr = *src` (ldrh), `w = (u8)hdr` (agbcc re-loads it as `ldrb` off the same address), `h = hdr >> 8`. The ldrh-BEFORE-ldrb order is what distinguishes this from `w = *(u8 *)src; h = *src >> 8;`.
- The destination offset is `dst + x` plus `y * 0x20` in u16 units (a 0x40-byte row stride), and the row pointer is `p = dst + i * 0x20` -- a MULTIPLY per outer iteration, not an accumulate: the ROM has `lsls r0, r1, #6; adds r4, r2, r0` every time round.
- Both loop counters are u16 tested unsigned; the outer counter's increment being hoisted above the inner loop is the compiler's doing, not the source's.

### Why it is parked

Register allocation, not source semantics. The instruction stream, the type model and every read form are settled; what remains is which hard register the row base and the row pointer land in, and that is coupled to whether dst survives as a pseudo of its own. Both sides of that coupling have been probed to exhaustion from source, the toolchain axis is measured and worse, and the permuter has converged. Unparking needs a construct that keeps dst alive across the stack-parameter load WITHOUT making it the row base. WAVE 73: still the correct diagnosis. This was the ONE function in the wave-73 pure-register-name batch that no lever moved, and it is also the only one of the six that had ALREADY had the permuter run on it before this wave.

### Wave 93

WAVE 93 (W93-D): still 87.5%, size-exact, draft unchanged. NEW STRUCTURAL FAMILY, and a sharper statement of the park. Reusing the PARAMETER `dst` as the row pointer while giving the row base its own local (base = dst + x; base = base + y * 0x20; then dst = base + i * 0x20; inside the outer loop) reproduces the ROM's pseudo structure exactly for the first time: the `adds r4, r0, #0` prologue copy of dst, the row base as a separate SCRATCH pseudo, and the row pointer recycling dst's now-dead register. Every earlier attempt gave the base its own local while ALSO keeping p separate, which let dst die at once and lost the copy. It is still not a match: agbcc then honours src's copy-preference for its incoming r1, leaves src there, and DROPS the `adds r5, r1, #0` prologue copy -- 42 instructions against the ROM's 43, 2 bytes short. Measured at 42 instructions with src in r1: both orders of the two inner increments; the draft's do-while(0) + int yoff wrapper carried over; src++ before or after the base computation; and parameter 2 taken as `const void *` and walked through a local `u16 *` (the local folds away entirely, byte-identical to the direct form). The mirror-image spelling `p = dst;` at the top is copy-propagated away: dst then stays in r0, the stack parameter loads into r1 instead, and it is SRC that gets the copy and dst that loses it. CONCLUSION: the dst/base SPLIT and the TWO prologue copies are mutually exclusive in every spelling measured -- agbcc always leaves exactly one of the two pointer parameters in its incoming register. Full write-up in work/sub_08012B70/NOTES.md.

### Wave 97

wave 97 (W97-L)
Base unchanged (87.50%, size-exact, only register numbers differ). Read sub_080726E8's copy-back step (lever 3):
it does not apply -- this function has no strength-reduction residual, the instruction stream is already 1:1.
Separate-row-base spellings compiled through spellings.py (all keep the row base in its own pseudo):
`base = dst + x + y*0x20`, `dst = dst + x; base = dst + y*0x20`, `d2 = dst` copy, `dst = base` after -- size-exact but
46.59% (the base goes to ip, dst stays in r0, so the ROM's prologue `adds r4, r0, #0` is missing: dst is never a
pseudo that outlives the stack-parameter load); `dst += x; base = dst; base += y*0x20` and `do { base = dst + ...
} while (0)` are 4 bytes short (14.77%). Nothing gives dst a prologue copy without making it the row base.
Mechanism of the tension: gcc only copies a parameter out of r0 when the pseudo is handed a callee-saved register
by global-alloc, which needs a live range beyond one block; the row-base spellings shorten dst's range to the
entry block. Not run through the permuter again (converged in wave 93).

wave 97 (W97-AA)
Base unchanged (87.50%). Re-measured the W93 split form (`base` own local, `dst = base + i*0x20` reassigned per row) with
`spellings.py`: 84 bytes (-4). Assembly (compile_probe): the dst prologue copy IS kept (`add r4, r0, #0`) but src stays in r1
(no copy), base goes to r6 (callee-saved), w to r5, i r3, j r2. ROM: src r5 (copy), base r2 (scratch), i r1, j r3, w r6, h r7.
So the ROM's residual is an ALLOCATION ORDER fact, not a missing copy source: src's r1 preference wins in ours because src is
handed a register before `i` is; in the ROM `i` has taken r1 first, forcing src to a callee-saved copy, and base (long-lived
but call-free) sits in scratch r2. A construct must raise i's allocno priority (floor_log2(refs)*refs/live_length) above src's,
or lower src's, without changing instruction count. Not found in 3 further spellings (respell base as `dst = dst + x; base = ...`
gives 88 bytes 44%: base to ip).

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.

</details>
