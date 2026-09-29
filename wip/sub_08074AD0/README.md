# sub_08074AD0

0x08074AD0, 144 bytes, THUMB, parked.

Best score so far: not measured.

## What it does

Builds an accelerating step table for moving between two points held in the proc. It takes the larger of the x and y distances, fills gUnknown_0202FEF8 with growing step sizes (0, 1, 1, 2, 2, ... up to 8) until the rest is smaller than the next step, and stores the remainder last and the step count in the proc.

## How close it is

Compiles 4 bytes short (140 against 144), about 51% of bytes match; the score means little because the difference starts at the loop's entry test and shifts everything after it. The draft loads the table's address once before the loop and reuses a shift across iterations, where the original reloads both inside the loop.

## What is left

Make the compiler load the table's force-addr address word into a register before the loop's entry test, so the path that skips the loop reaches the table through that same register instead of getting its own direct pool word; the if/else form described in the draft's notes already gets the loop shape right and fails only on this. No spelling is known that does it without losing the extra level of address loading.

## Already tried

- for, while, and an if wrapping a do/while: identical output, with the address load moved out of the loop.
- if/else with the loop in the else arm: gets the branch direction, the in-loop shift and the shared final store, but the skip path gets its own pool word; 8 bytes too long (152).
- Binding the table to a local pointer before the loop, and other cast spellings: no change.
- Declaring the compiler-made address word as a real pointer global: three levels of loads and an extra data word the original does not have.
- Other compiler configurations: none helps (older compiler 38%, -O1 21.5%, force-addr off no change).

## Files

- `sub_08074AD0.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

53.5% -- 144 bytes, candidate 140 (-4). TWO agents this wave (W38-B, W38-E), W38-E starting from scratch as the wave-37 inherited-draft rule requires. The residual is one source-level fact and all three symptoms follow from it: the original loads the gUnknown_081CC4D4 base ONCE into r5 and reuses it in both arms (`ldr r5,[r7,#0]` then `adds r0,r4,r5`), the candidate reloads it at the bottom; the original's pool word sits MID-FUNCTION at +0x54 with the code branching over it (`ldr r0,[r7,#0]; b.n 7e`) where the candidate dumps it after the epilogue -- that IS the -4 bytes; and allocation is shifted by one register throughout (r6 vs r5). Ruled out: binding the array base to a local before the loop, which is the wave-37 chapter that predicts exactly this pool movement -- it was tried and does not reproduce the mid-function dump. Not tried: the toolchain axis. WAVE 38. ---- WAVE 73 (W73-A): THE TOOLCHAIN AXIS IS RULED OUT, and without spending a probe. This entry listed 'Not tried: the toolchain axis' as the one remaining lead. It is not a lead. Wave 38's measurement was never about the 0x0807xxxx address range: the two functions that genuinely needed old_agbcc were the two WHOSE LOOP BODY CONTAINS A CALL. This loop body is 'dist -= step >> 1; gUnknown_0202FEF8[i] = step >> 1; if (step <= 0xf) step = step + 1; i++;' -- four statements, no 'bl' of any kind. The trigger mechanism is absent, so old-agbcc / old-agbcc-no-force were deliberately NOT probed; wave 56 already ran four such profile probes on mechanism-free candidates for zero positives, including one that met the documented address-range trigger exactly and changed not one byte. The ONLY open lever remains W38-E's: make r7 (the force-addr pool pointer) live BEFORE the guard without collapsing the indirection, so the zero-trip edge reaches the table through the same register instead of const-folding to its own direct pool word. Verified unchanged at 140/144 bytes this wave. WAVE 81 (W81-C): temporary-profile sweep on the unchanged draft -- configured 51.4% (NOTE the entry's recorded 53.5% was not reproduced on today's draft, re-measure before reasoning from either), no-force 51.4% identical, o1/o1-no-force 21.5%, old-agbcc pair 38.2%. The 'not tried: toolchain' gap is closed; only W38-E's lever remains.

### Wave 93

WAVE 93 (W93-C). UNCHANGED at 140/144 (-4), 51.39%, first difference +0x5. NO PROBE SPENT -- the recommended base was a spelling this repo had already rejected in writing, and finding that out cost one grep.
BASE REJECTED: `best.c` (53.47%, size-exact) declares `extern s8 *gUnknown_081CC4D4;` IN THE .c FILE (which the brief forbids outright) and indexes it in place of the real array gUnknown_0202FEF8. include/unknown-globals.h already carries the verdict on exactly that spelling: 'WAVE 38 (W38-E) -- 0x081CC4D4 RE-TESTED AS A REAL POINTER OBJECT AND IT IS NOT ONE ... agbcc applies force-addr to the pointer variable as well and emits three levels ... plus an R_ARM_ABS32 .rodata word the ROM does not have (53.5%, size exact).' That is best.c, to the hundredth. It reaches the ROM's byte count by ADDING an indirection level and a data word the ROM lacks. 0x081CC4D4 is this function's -fforce-addr word holding &gUnknown_0202FEF8; the ROM's `ldr r7,=<word>; ldr r5,[r7]` is TWO levels, which is what the draft's honest `gUnknown_0202FEF8[i]` already emits. Renamed `best.c.wrongc`; the draft is the correct base.
METHOD NOTE FOR THE NEXT WAVE: a base scan that ranks by percentage will keep recommending this file. Before adopting any base that names a 0x081CCxxx / 0x0808xxxx word as a global, grep include/unknown-globals.h for that address -- the force-addr words are documented there as NOT OBJECTS, with the measurement attached.
RESIDUAL unchanged (wave 38's): the ROM loads the force-addr word's address into r7 BEFORE the loop's entry test, so the loop-skip path reaches the table through the same register and shares the final store, and the pool word dumps mid-function after the resulting unconditional branch.

### Wave 97

wave 97
Base: sub_08074AD0.c (51.39%, -4). Not vesly-best.c (its 53.5% is the wave-38 stacked-pointer artefact).
Probed: `for (i = 0; ...; i++)` form -> byte-identical (140 B). Guard-and-early-return form
(`if (dist - (step>>1) < 0) { table[0] = dist; ...; return; }` before the loop) reaches 144 B size-exact but 12.5%: it
moves the `ldr` of the force-addr word to a per-arm position and reallocates `proc` into r3 (ROM keeps it in ip), i.e. it
gives the ROM's size for the wrong reason. A dummy `table[i] = 0` store before the loop: +12, extra frame register.
No lever found in 4 probes; residual is unchanged (the force-addr word address must be live before the guard).

</details>
