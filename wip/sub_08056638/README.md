# sub_08056638

0x08056638, 144 bytes, THUMB, parked.

Best score so far: 95.8%.

## What it does

Bubble-sorts side `side`'s five sort keys, gUnknown_02029822[side][0..4], into ascending order, skipping empty slots (0xff), bumping a key by 1 when it equals the key before it, and moving the matching entries of gUnknown_0202980A[side] along with the keys.

## How close it is

Compiles to the right size (144 bytes); 6 of 144 bytes differ (95.8% identical). The only difference is that the original keeps gUnknown_02029822's address in a register and reloads gUnknown_0202980A's from the pool, and the draft does the opposite: three instructions.

## What is left

The compiler hands out registers by a score of use count (uses inside loops count more, and `do { } while (0)` counts as a loop) over lifetime. To match, gUnknown_02029822's address needs 8 or 9 weighted uses and gUnknown_0202980A's at most 7, while `side * 0x6c` keeps its register; every change found so far that raises the first also raises `side * 0x6c`.

## Already tried

- Flat byte-offset indexing for both arrays, or for the payload only: 8 or 4 bytes too long.
- A row pointer for the keys: folds the two reads into one base and a load offset.
- Separate temporaries for the payload swap: 4 bytes too long; reusing `a` and `b` is required.
- Removing the `do { } while (0)`, or moving the swap into it: the loop counters take other registers, or up to 14 bytes differ.
- Delaying the key store until after the payload writes: 21 bytes differ.
- An empty `+= 0` on the payload row, dead address statements, and extra `do { } while (0)` blocks around the key loads: no change, or the right address in the register but another value spilled.
- All 1,024 ways of wrapping the ten inner statements in `do { } while (0)`: 320 put the right address in the register, but every one moves `side * 0x6c` out of its register.
- Permuter runs (over 100,000 tries in total, undirected and directed): nothing better than the draft.

## Files

- `sub_08056638.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at exact size 144/144 with 6 differing bytes. The only residual is the allocno/pool inversion between gUnknown_02029822 and gUnknown_0202980A. Flat indexing, typed pointer binding, separate swap temps, four permuter runs and Wave 70's payload opaque-use barrier are ruled out; the barrier flips the held base but over-hoists both rows and removes the required inline load. Preserve the verified readable draft. Wave 77, W77-L: configured re-verdict still 144/144 with SIX differing bytes, and they are exactly three halfwords -- +0x28 `mov r3,sb` vs `ldr r3,[pc]`, +0x34 `mov r0,sb` vs `ldr r0,[pc]`, +0x56 `ldr r2,[pc]` vs `mov r2,sb`. NEWLY RULED OUT, each measured: moving the whole payload swap inside the do/while(0) so its base pseudo lives in ONE basic block (14 bytes, 90.3%); moving only the last payload store inside it (byte-neutral, still 6); deleting the do/while(0) entirely (still 6 bytes, but the held base moves from sb to sl and the outer counter takes sb -- the do/while is load-bearing for the COUNTERS, not for the payload); and byte-offset-first/base-last flat spelling of the PAYLOAD ONLY, `*(u16 *)(j * 2 + side * 0x6c + (u8 *)g)` (+4 bytes, 26.4%) -- the doc's one `later` lever, refuted here by its collateral exactly as on sub_0804C5A4. The wave-57 `two basic blocks leaves local_alloc` rule does NOT explain this residual: block count of the payload accesses was varied over 1, 2 and 3 and the held register never flipped. Both address constants have 2 references and near-equal live ranges, so this is an allocno TIE, and no source-level block or operand-order lever this project has reaches a tie-break. Wave 80 (W80-F), dead-statement/creation-order transfer, four compile_probes, no try_match spent, still 144/144 and 6 bytes: dead row/element address locals for gUnknown_02029822 at the loop top or in the swap block are byte-identical (flow deletes them before refs are counted); a dead gUnknown_0202980A address created BEFORE the first gUnknown_02029822 use is byte-identical too, which lowers 0A's pseudo number below 22's and shows the allocno-number tie-break is not the decider -- the priorities differ; a second do/while(0) region around the gUnknown_02029822 loads does put 22 in sb but drags side*0x6c into ip and spills the outer counter (worse), and around the first load only it swaps the symbols with the same collateral. W83-B: no new axis measured (batch budget went to the permuted siblings); state per W77-L/W80-F stands.

WAVE90: WAVE 90 (W90-A): unchanged 95.8%/6 bytes. The residual is now arithmetic: global.c priority = floor_log2(refs)*refs/live_length from the .greg dump; &gUnknown_02029822 (pseudo 31) 7 refs/88 = 0.159 gets NO register (rematerialised), &gUnknown_0202980A (109) 8 refs/80 = 0.300 gets sb, side*0x6c (36) 0.333 r8, i+1 (169) 0.229 sl. Refs are loop-depth weighted and the swap do/while(0) counts as a loop level: it is worth exactly the one ref that lifts 0A over the floor_log2 step at 8. The ROM needs refs(22) in {8,9} and refs(0A) <= 7 with the rest unchanged. All 1,024 per-statement do/while(0) wrap combinations compiled (-dg): 320 give the ROM's 22-in-sb / 0A-rematerialised split, and in every one side*0x6c moves from r8 to ip because the statements that reference 22 also reference 36. Permuter: undirected 11,720 it, directed 10,674 it, exhaustive PERM_GENERAL do/while enumeration 1,024 it: nothing better. Next lever must raise 22's refs without 36's (or the j+1 copy's with them).

### Wave 92

W92-C: unchanged 95.83%/6 bytes, first difference +0x28. No new axis was measured; the W90-A allocno arithmetic stands (keys address 7 refs / live_length 88, payload address 8 refs / 80, priority = floor_log2(refs)*refs/live_length). Recorded so it is not re-derived as though already measured: there are TWO arithmetic routes nobody has tried, both via LIVE_LENGTH rather than refs -- cutting the keys address's live_length below about 47, or raising the payload address's above about 151, reverses the ranking with both ref counts unchanged. Each needs a LIVE reference in a place the sort does not have one, and a dead one is deleted before refs are counted (W80-F). THE MATCHED-SIBLING SPELLING AXIS IS NOW CLOSED, measured. Both arrays are rows of struct Unk02029808 (stride 0x6c): the payload is the unk02 row and the keys are the unk1a row, so gUnknown_0202980A[side][j] == gUnknown_02029808[side].unk02[j] and gUnknown_02029822[side][j] == gUnknown_02029808[side].unk1a[j]. Matched sub_0805653C writes that memory BOTH ways in one function, and the gUnknown_020297CC note in include/unknown-globals.h records matched sub_080546F0 deliberately mixing the flat and member forms for two rows of one record -- so this was the strongest open lead. All three combinations compiled: keys flat + payload member = 144/144, 7 differ (95.1%); keys member + payload flat = 144/144, 7 differ (95.1%); BOTH member = 148 bytes (+4), 132 differ (10.8%). The mixed form costs one byte MORE than the draft and the doubled member form collapses, because with both rows reached through one record the compiler shares a single base address while the ROM plainly keeps two. The draft's row-symbol spelling for both arrays is correct.

### Wave 93

W93-F: one 900 s run from the draft under the new length-penalty scorer returned nothing.

### Wave 96

Base: draft unchanged (95.83%, size+0, first diff +0x28). Pre-registered hypothesis (bind the keys table address for a subset of reads) tested three ways, bound as `u16 (*k)[54] = gUnknown_02029822;`: loads only 18.8% (size+0, diff at +0x2), all reads 15.3% (-12), stores only 10.1% (+24). Mechanism: the bind makes agbcc fold `side*0x6c` into the base (one shared row pointer), which the ROM plainly does not have (it keeps side*0x6c in r8 and the symbol separately). The wave-90 arithmetic still stands: the keys address needs 8-9 weighted refs with side*0x6c unchanged; a bind adds a ref but also restructures the address arithmetic. Not re-run through the permuter (waves 52/90 did ~110k iterations).
Proposed status: unchanged; left = keys-vs-payload row symbol takes the callee-saved register (3 halfwords); tried adds "row-pointer bind of the keys table, three subsets, all restructure the address arithmetic".

### Wave 97

wave 97 (W97-S)
Draft unchanged (95.83%). Tried by spellings.py: one shared `n = j + 1` temp for every j+1 subscript in the keys array: 19.4%, -4 (frame differs: drops a callee-saved register); `-~j` for the payload's j+1: 31.8%, +4. Both restructure the address arithmetic. No movement on the sb-vs-rematerialised pair.

wave 97 (W97-PG)
Permuter chain: 1 link, 95.83% -> 95.83%, NO-IMPROVEMENT.

</details>
