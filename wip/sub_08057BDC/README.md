# sub_08057BDC

0x08057BDC, 360 bytes, THUMB, parked.

Best score so far: 81.1%.

## What it does

Per-frame update of the two-side display. For the first eight frames it slides a widening slice of tiles between two tilemap buffers; then, for each side whose shown value has not reached its target, it steps the value toward the target and redraws that side.

## How close it is

Compiles to the right size (360 bytes) with 81.1% of bytes identical; the first loop matches exactly after writing `p->unk02 << 5` inline instead of reusing a local. What is left: in the second loop the original keeps gUnknown_085D6A48 and gUnknown_03004582 in registers across the loop and the draft reloads them.

## What is left

Get the compiler to keep the row table gUnknown_085D6A48 and the selector table gUnknown_03004582 in registers across the second loop, the way it now no longer does. Once those two are hoisted the trailing constants should fall into the original's order as well.

## Already tried

- Binding the row table gUnknown_085D6A48 to a local before the second loop: the compiler substitutes the global straight back; no change.
- Also binding gUnknown_03004582 to a row-pointer local: the compiler turns it into a pointer stepped by 16 each pass, which the ROM does not have in this loop.
- Dummy self-assignments of the gUnknown_08551A04 pointer and of what it points to (`+= 0`): gUnknown_08551A04 still wins the register.
- Dead uses of locals: bring the size back to 360 (this is what best.c holds, 61.9%) but keep the wrong register choice and disturb the first loop.

## Files

- `sub_08057BDC.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at 356/360 (-4). Everything before loop 2 is byte-exact. ROM hoists gUnknown_085D6A48 and gUnknown_03004582 while rematerialising gUnknown_08551A04; candidate does the reverse. Pointer bindings, pointer/pointee self-use barriers and dead-local uses are ruled out: the latter recover size but preserve the wrong hoist and perturb loop 1. Residual is one LICM priority choice.

### Wave 93

WAVE 93 (W93-C). 58.06%% at -4 (first difference +0x19) -> **76.94%%, SIZE-EXACT 360/360, first difference +0x90**. The whole gain came from one thing, and it is a process finding as much as a codegen one: **THE PERMUTER HAD NEVER BEEN RUN ON THIS FUNCTION.** Eight waves of parking with a long ruled-out list, on a residual the park itself describes as one LICM priority choice, 4 bytes short, instruction order otherwise correct -- which is exactly the tool's case. One 900 s run closed the size and moved the first difference 0x77 bytes later.
WHAT IT FOUND is the lever this entry had been asking for: an empty `do { } while (0);` BETWEEN the two redraw calls. gUnknown_08551A04 is the only one of the three addresses used twice per pass, that pair is what lets it win the hoist, and a statement boundary between the two references stops them being unified (wave 89's EBB-table splitter). Every lever the park had tried -- pointer self-use, pointee `+= 0`, self-assignment, dead local uses -- acted on the VALUE or on the loop as a whole, never BETWEEN the two references. Two further edits came with it, both audited: `sel`, a `u16 (*)[8]` bound to gUnknown_03004582 (load-bearing -- without it 59.44%% and the size is lost again), and `idx` reused for `p->unk02` in the first loop's else arm after `p` is computed from the old value (value-identical, recomputed next pass).
MEASURED WARNING, do not 'fix' it: the guard `active = 0 != gUnknown_030005E8[i]` compiles to a branchless `negs/orrs/cmp/bge` where the ROM has a plain `cmp r0,#0; beq`, and it looks like free bytes. Binding the VALUE instead (`active = gUnknown_030005E8[i]; if (active != 0 && ...)`) is 68.33%%, and binding `sel` late is also 68.33%%; all four variants share the first difference at +0x90, so the odd-looking form is the one with the fewest differing bytes downstream.
EARLIER NEGATIVES THIS WAVE (all on the pre-permuter draft, and they narrow the park): the ROM's two hoists sit AFTER `movs r7,#0`, so by the preheader rule both are LICM hoists rather than source bindings -- yet moving the `rows` binding makes no difference anywhere. Removing it and casting inline is 57.78%%, moving it inside the loop body is BYTE-IDENTICAL to the draft, and pulling the selector into its own local is 57.78%%. So 'the compiler substitutes the global straight back' holds for every placement, not just the one the park measured. Also: +0x19 was never an early divergence -- it is an odd address, the high byte of the `ldr rN,[pc,#imm]` at +0x18, whose immediate moved because the pool at the END of the function was reordered.
BASE REJECTED: `best.c` (61.94%%, size-exact) was the draft plus five dead statements (`gUnknown_08551A04[0] += 0;`, `rows += 0;`, `c = c;` and two more). Value-preserving, so not wrong C, but its first difference was +0x19 -- IDENTICAL to the draft's. Four bytes of padding that moved nothing. Renamed `best.c.wrongc`.
RESIDUAL: size-exact, first difference +0x90. The ROM hoists gUnknown_085D6A48 and gUnknown_03004582 into sb/r8 in the second loop's preheader; the candidate now rematerialises all three inside the body, which is also why the tail pool words sit in a different order. The 08551A04 half of the park is solved, the other two are not.

### Wave 96

Base: the wave-93 draft (`sub_08057BDC.w96-start.c`, 76.94%). Now 81.11%, size+0, first diff +0xc1 (was +0x90): the FIRST LOOP IS NOW BYTE-EXACT.
- What moved it: delete the `idx = p->unk02;` reuse in the first loop's else arm and write `((p->unk02 << 5) + p->unk00)` directly. The reuse forced `p->unk02` to be loaded into a register BEFORE the `14 - c` arithmetic (`ldrh r5,[r6,#2]` first); the ROM loads it after, at the use. So the wave-93 note that `idx` is reused in the else arm was a compensation for the old second-loop state and is now wrong; the source comment above the function still mentions it (leave for the orchestrator, or trim when promoting).
- Second loop, unchanged residual: the ROM hoists gUnknown_085D6A48 (r9) and gUnknown_03004582 (r8) into the preheader after `movs r7,#0` and tests `gUnknown_030005E8[i]` with a plain `cmp r0,#0; beq`; the draft rematerialises both and uses the `negs/orrs/bge` form. Re-measured on the new base: plain `if (gUnknown_030005E8[i] != 0 && ...)` 63.9%, size-4 (with `rows` bound at the top or in-body, with `sel` bound or inline, with or without the do{}while(0)): 57.8-63.9%, all -4 at +0x19/+0xc0. Binding position of `rows` (top, in-body, inline cast): byte-identical at 81.11%. So the hoist is still unreached.
- Permuter (900 s, 2 threads, from the new base): NO-IMPROVEMENT (scores 2660 -> 2340 candidates were not size/verify-clean).
Proposed status: first loop matches; left = the second loop keeps two table addresses in registers in the ROM (LICM hoists) and the draft rematerialises them, plus the `ldrsh; cmp; beq` test form.

</details>
