# sub_08070F44

0x08070F44, 104 bytes, THUMB, parked.

Best score so far: 58.6% (best.c).

## What it does

Sound-library routine CgbModVol: sets a Game Boy-style (CGB) sound channel's stereo pan and target volumes from its left and right volumes. It pans fully to one side when that side is at least twice as loud, otherwise to both, then sets the envelope target to (left + right) / 16 and the sustain target from it.

## How close it is

Compiles to the right size (104 bytes) with every instruction right and in the right order; 26 bytes differ (75% match) only because the channel pointer sits in a different register and every later register renumbers. Those numbers are with the older compiler its sound-library neighbours are built with; no compiler override entry exists for it yet, so the default settings score much lower.

## What is left

Find why the compiler gives the channel pointer a different register from the original's. It is a near-tie in how the register allocator ranks the pointer against one of the two volume values, and no source construct has been found behind it. The automatic search is no longer an option here: its objective rewards making the function shorter, and this function is already the right length, so it ranks candidates in the wrong order (measured over 40 checked candidates). Read the allocator's decision in the compiler's per-pass debug dumps instead, and add the older-compiler override entry once it matches.

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
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

104 bytes SIZE-EXACT, 75.00% identical under --profile old-agbcc (W93-A's multiply-operand order and tail local re-use; re-measured by W93-F's permuter baseline). Under --profile configured the same source is far worse; old_agbcc is and remains the right configuration for this function, and it still has no override entry.

### What still differs

One hard-register assignment and nothing else. The ROM puts `chan` in r1, the leftVolume <<24 temp in r2 and rightVolume in r3; the candidate puts `chan` in r2, the temp in r3 and rightVolume in r1, and every later byte renumbers. 33 of 104 bytes. THE INSTRUCTION STREAM AND THE INSTRUCTION ORDER ARE IDENTICAL -- there is no load the ROM performs at a different point, no extra copy, no different block layout. Only the register numbers differ.

### Why it is close

Size-exact, every instruction correct, block layout including agbcc's cross-jump of the two envelopeGoal stores exact.

### Already ruled out

- WAVE 79 (W79-D): THE COMPILER PROFILE SPACE IS NOW EXHAUSTED FOR THIS FUNCTION, which is the one thing the entry had not done. All six temporary profiles against the same source: configured/default 56.7%, no-force 56.7%, old-agbcc 68.3%, old-agbcc-no-force 68.3%, o1 19.2%, o1-no-force 19.2%. old_agbcc is the ceiling, -fforce-addr is INERT here (it does not change one byte under either compiler), and -O1 is catastrophic. This matters because -O1 with -fforce-addr removed is exactly what closed sub_0808AC7C and sub_0808B5B8 in this same wave; it was tested here and it is not the answer. Do not re-run the toolchain axis on this function.
- The pre-wave-79 list stands: local types and declaration order (u8/u8 in both orders, u16/u16, u32/u32 all 68.3%; int and s32 break the shape), decomp-permuter 300 s x 4 threads under old_agbcc (best 73.1%, no match), flattening the four arms, a single common clamp, and binding chan->unk06 to a local.

### Why it is parked

NO SOURCE CONSTRUCT SITS BEHIND THIS RESIDUAL, and wave 79 looked specifically for one. Its two block-mates in this wave, sub_08070D98 and sub_0808B5B8, both LOOKED like pure register renumbering in exactly the same way and both turned out to be evaluation-order facts -- a load the ROM performs after a multiply, a pool pair the ROM creates in the other order -- each visible in the diff as an instruction-ORDER difference. This function has no such difference: order and instruction multiset are identical to the ROM's and only the hard-register numbers move. THAT IS THE DISCRIMINATOR between the two kinds of near-miss. It is an allocno-priority near-tie between `chan` (many refs, function-long range) and `right` (few refs, short range) in gcc 2.9 local-alloc, and it needs a reader of local-alloc priority, not another spelling.

### Wave 93

WAVE 93 (W93-A): 68.27% -> 75.00% under --profile old-agbcc, still size-exact at 104, first difference still +0x2. Draft REPLACED (wave-start copy in sub_08070F44.w93-start.c). THE PARK'S CENTRAL CLAIM WAS WRONG: it says 'the instruction stream and the instruction order are identical -- only the register numbers differ', and there were TWO real order facts left. (1) THE MULTIPLY'S OPERANDS ARE THE OTHER WAY ROUND. The ROM loads unk06 (+0x06) BEFORE envelopeGoal (+0x0a) and copies unk06 into the product register; the old draft's `chan->unk06 * chan->envelopeGoal` emits them in the opposite order, because agbcc evaluates the SECOND operand of a MULT first. Writing `chan->envelopeGoal * chan->unk06` fixes the load order and is worth 73.08 -> 75.00. (2) The tail re-uses the two volume locals for the pan mask (`left = chan->pan; right = chan->panMask; chan->pan = right & left;`) instead of `chan->pan = chan->panMask & chan->pan;`. This was already in best.c and nobody had installed it: 68.27 -> 73.08, because it lengthens the locals' live ranges. Its statement order is the reverse of the ROM's load order and that is correct -- putting the panMask read first is 71.15%. MEASURED NEUTRAL: reusing the locals for the multiply's operands as well (75.00, byte-identical); declaring left before right (75.00); `right >> 1` instead of `right / 2` (75.00). REMAINING RESIDUAL, 26 of 104 bytes, is ONE hard register: the ROM keeps `chan` in r1 and the `rightVolume << 24` temporary in r2, the candidate has them swapped, and the sum's operand ownership follows from it. Read as global.c's allocno_compare (floor_log2(refs)*refs/live_length, ties broken by the LOWER allocno number): `chan` is the parameter so its allocno is created first and wins any tie, which is the ROM; for the candidate to lose, the <<24 temporary's priority must be strictly higher, i.e. its live range is short (7 insns, 3 refs) against chan's function-long one. The lever would lengthen that temporary's range or shorten chan's, and neither is spellable -- the temporary is agbcc's own zero-extension of a volatile QImode load. TOOLING GAP THAT BLOCKS THE OBVIOUS NEXT STEP: tools/permute.py calls agbenv.flags(name) with no profile argument (permute.py:209), and agbenv.flags(fn, profile) does take one. This function has no compiler-overrides.json entry, so a permuter run here would score against the DEFAULT toolchain while the function needs old_agbcc, and the result would not transfer. A --profile passthrough on permute.py would make this function permutable. That matters because the twin case this wave, sub_08073480 (size-exact, pure register-name residual), CLOSED on a 900 s permuter run.

W93-F: FIRST permuter campaign under the compiler this function needs. tools/permute.py now takes --profile, which is the gap W93-A's notes said blocked this step. Run: --profile old-agbcc --seconds 900 --threads 4 --current. 43,742 iterations, 47 outputs, the best 40 checked against the real bytes: NOTHING beat the starting point, and the draft is restored binary-equal to the pre-run copy. The permuter's own baseline line independently re-measures the draft at 75.00%, size-exact, first difference at +0x2. THE RESULT IS AN INVERSION, NOT A NEAR MISS: the permuter's objective is anti-correlated with the real match here. The draft scores 2100 on that objective; all 40 verified candidates scored BETTER (620-1940) and matched WORSE (4.63%-62.50%). Best permuter score 620 = 45.19% and 4 bytes short; score 820 = 56.73%; score 1900 = 62.50%. The objective rewards shrinking the body, and this residual is one register name inside a body that is already exactly the right length, so every step it calls progress moves away. Chaining cannot fix a wrong direction. DO NOT spend another undirected run on this function; it needs either a size-exactness term strong enough to dominate on a 104-byte body, or the allocator read directly. Also correcting the record: the 'tried' line crediting a 300 s permuter run 'with the older compiler' at 73% predates --profile, so it cannot be what it says -- permute.py had no way to select a compiler and used the configured one.

### Wave 94

W94-B: first permuter campaign under the length-penalised scorer (AW2_PENALTY_SIZE=1000), --profile old-agbcc --current from the 75.00% draft. 32,175 iterations, 40 outputs kept, and every one re-verified by hand afterwards against the real bytes (the run's own verify phase was contaminated by a second concurrent run on the same function; see work/sub_08070F44/NOTES.md). Best candidate 69.23%; NOTHING beat the draft. The scorer fix does what it was meant to do -- 40 of 40 kept outputs are size-exact, where the old objective's best was 4 bytes short -- but the inversion survives it: all 40 score better on the objective (1340-2080 against the draft's 2100) and match worse on the bytes. Holding the length fixed is necessary and not sufficient on a register-numbering residual. Draft unchanged: 75.00%, size-exact (104/104), first difference +0x2 under old-agbcc; still no compiler-overrides entry. Do not spend another undirected run here.

</details>
