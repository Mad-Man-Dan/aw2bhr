# sub_08070F44

0x08070F44, 104 bytes, THUMB, parked.

Best score so far: 58.6% (best.c).

## What it does

Sound-library routine CgbModVol: sets a Game Boy-style (CGB) sound channel's stereo pan and target volumes from its left and right volumes. It pans fully to one side when that side is at least twice as loud, otherwise to both, then sets the envelope target to (left + right) / 16 and the sustain target from it.

## How close it is

Compiles to the right size (104 bytes) with every instruction right and in the right order; 33 bytes differ (68% match) only because the channel pointer sits in a different register and every later register renumbers. Those numbers are with the older compiler build its sound-library neighbours match with; the default settings score 57%, because no compiler override entry exists yet.

## What is left

Find why the compiler gives the channel pointer a different register from the original's: it is a near-tie in how the register allocator ranks the pointer against one of the two volume values, and no source construct has been found behind it. Read the allocator's decision in the compiler's per-pass debug dumps rather than trying more spellings, and add the older-compiler override entry once it matches.

## Already tried

- Every compiler configuration: the older compiler (68%) is best; -O1 is far worse (19%) and turning force-addr off changes nothing.
- Other local types and declaration orders (u8 in either order, u16, u32): same 68% or worse; int or s32 break the code's shape.
- Writing the four pan cases out flat: duplicates the capping code, 12 bytes too long.
- One shared cap for every case: loses one of the two envelope-target stores the original keeps.
- Binding the sustain byte to a local before the multiply: no change to the registers, and one right shift becomes the wrong kind.
- Automatic permuter, 300 seconds x 4 threads with the older compiler: best 73%, no match; check its candidate's body before trusting that score.

## Files

- `sub_08070F44.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

104 bytes SIZE-EXACT, 68.3% identical under --profile old-agbcc. Unchanged by wave 79. Under --profile configured the same source is 56.7%; old_agbcc is and remains the right configuration for this function.

### What still differs

One hard-register assignment and nothing else. The ROM puts `chan` in r1, the leftVolume <<24 temp in r2 and rightVolume in r3; the candidate puts `chan` in r2, the temp in r3 and rightVolume in r1, and every later byte renumbers. 33 of 104 bytes. THE INSTRUCTION STREAM AND THE INSTRUCTION ORDER ARE IDENTICAL -- there is no load the ROM performs at a different point, no extra copy, no different block layout. Only the register numbers differ.

### Why it is close

Size-exact, every instruction correct, block layout including agbcc's cross-jump of the two envelopeGoal stores exact.

### Already ruled out

- WAVE 79 (W79-D): THE COMPILER PROFILE SPACE IS NOW EXHAUSTED FOR THIS FUNCTION, which is the one thing the entry had not done. All six temporary profiles against the same source: configured/default 56.7%, no-force 56.7%, old-agbcc 68.3%, old-agbcc-no-force 68.3%, o1 19.2%, o1-no-force 19.2%. old_agbcc is the ceiling, -fforce-addr is INERT here (it does not change one byte under either compiler), and -O1 is catastrophic. This matters because -O1 with -fforce-addr removed is exactly what closed sub_0808AC7C and sub_0808B5B8 in this same wave; it was tested here and it is not the answer. Do not re-run the toolchain axis on this function.
- The pre-wave-79 list stands: local types and declaration order (u8/u8 in both orders, u16/u16, u32/u32 all 68.3%; int and s32 break the shape), decomp-permuter 300 s x 4 threads under old_agbcc (best 73.1%, no match), flattening the four arms, a single common clamp, and binding chan->unk06 to a local.

### Why it is parked

NO SOURCE CONSTRUCT SITS BEHIND THIS RESIDUAL, and wave 79 looked specifically for one. Its two block-mates in this wave, sub_08070D98 and sub_0808B5B8, both LOOKED like pure register renumbering in exactly the same way and both turned out to be evaluation-order facts -- a load the ROM performs after a multiply, a pool pair the ROM creates in the other order -- each visible in the diff as an instruction-ORDER difference. This function has no such difference: order and instruction multiset are identical to the ROM's and only the hard-register numbers move. THAT IS THE DISCRIMINATOR between the two kinds of near-miss. It is an allocno-priority near-tie between `chan` (many refs, function-long range) and `right` (few refs, short range) in gcc 2.9 local-alloc, and it needs a reader of local-alloc priority, not another spelling.

</details>
