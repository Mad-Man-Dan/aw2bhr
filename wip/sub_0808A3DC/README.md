# sub_0808A3DC

0x0808A3DC, 160 bytes, THUMB, parked.

Best score so far: 60.0%.

## What it does

Builds a BG screen from compressed data. It clears the tilemap buffer, decompresses the tile graphics into VRAM and the tilemap into the buffer, adds a tile and palette offset (0x1020) to every entry, and copies the finished map to the BG's screen block.

## How close it is

Measured with the settings the neighbouring flash code uses (-O1, force-addr off; no override entry yet): right size (160 bytes), 55.6% of bytes match, differing from the first instruction because the original saves two more registers. The one real difference: the original reaches gUnknown_0849957C and gUnknown_03001FE8 through this function's own compiler-made address words (one more load each) and holds the word addresses in saved registers, where the draft reaches the globals directly.

## What is left

Make the code use those address words: the compiler builds them under every spelling tried but leaves them unused. Untested idea: the ROM words from 0x08499578 on are a table of EWRAM buffer pointers 0x800 apart and gUnknown_0849957C is its second entry, so reading it as element 1 of an array at 0x08499578 would make the table's address the value used, the only case in which such words have been seen in use.

## Already tried

- Default -O2 and the older compiler: worse (the older compiler 71 against 64 differing bytes at -O2).
- A plain u16 fill value instead of volatile: the compiler computes the stack address once where the original computes it twice.
- Non-volatile element access in the loop: 12 bytes too long (172).
- `*(u16 **)&gUnknown_0849957C` at every use: fixes the pool word order but is still one load short.
- Binding both addresses to local pointers: the original's register use, but 4 bytes short (22.5%).
- Mixing a bound local and the plain name: the address word is still built and ignored.

## Files

- `sub_0808A3DC.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

60.0% (wave 46, W46-D then W46-I)

### What still differs

First difference at +0xa. Calls CpuFastSet, CpuSet and Decompress -- all hub callees with settled prototypes, so the residual is not a prototype problem. The usual trap in this shape is the CpuSet/CpuFastSet mode word (count OR'd with the fill/copy flag), which is a constant expression at the call site.

### Already ruled out

TOOLCHAIN AXIS RULED OUT (wave 46, W46-I): the candidate C was held constant and compiled under five configurations -- default, -fprologue-bugfix off, -fforce-addr off, old_agbcc, and old_agbcc + no force-addr -- with .text compared against work/<fn>/_target.bin each time. old_agbcc is byte-for-byte IDENTICAL to the default on sub_0808AAF4 and sub_0808AC7C (it does not change one instruction) and is strictly WORSE on sub_0808A3DC (64 -> 71 differing bytes). Dropping -fforce-addr is worse on three of four. The wave-46 brief predicted this block needed old_agbcc on the strength of compiler-overrides.json's 'the sound and flash libraries came out of the SDK prebuilt with their own settings' plus four of six functions differing at +0x0; that prediction is REFUTED. Do not re-run it. Full table in docs/agbcc-codegen.md, 'The flash driver at 0x0808A000 is NOT a toolchain-axis block'. This is the function old_agbcc makes strictly WORSE (64 -> 71 differing bytes), which is the strongest single piece of evidence against the axis for this block.

### Wave 60 o1

WAVE 60: MEASURED AT -O1, WHICH IS THE CORRECT CONFIGURATION FOR THIS BLOCK. The flash library is -O1, established this wave by nine byte-for-byte matches across 0x0808A3DC-0x0808B91C with no source change (see data/compiler-overrides.json). EVERY SCORE AND EVERY RULED-OUT AXIS RECORDED HERE BEFORE WAVE 60 WAS MEASURED AT -O2 AND IS THEREFORE INVERTED EVIDENCE, NOT WEAK EVIDENCE -- do not trust the axes above. This function has NO override entry, because the project's standard for that file is a byte-for-byte match under the named configuration and this is not one yet. To work it, add a throwaway entry {"cflags_remove": ["-O2"], "cflags_add": ["-O1"]} and remove it if it does not close. -O1 score at the end of wave 60: 60.0%, size-exact, first difference at +0x0.

### Wave 88

WAVE 88 (W88-A then W88-D): the brief's 60.0% was stale; current is 55.6% size-exact under BOTH `o1` and `o1-no-force`. Residual isolated to the address-constant indirection. Wave-60 note REFUTED: agbcc builds the two-level `.rodata` block every time and leaves it dead; a load-base use never triggers the indirection at any count, only an address VALUE does -- and (W88-D) the doc's mixed-spelling rule is a PRESERVATION rule, not a creation rule: the ROM's mixed shape still emits .LC0 and ignores it. Settled from data/promoted.json: the function's two words are the gap between c_08089C14.c's and c_0808A978.c's carved blocks in a run of seven, so they are compiler-generated and must NOT be declared as variables. Evidence: work/sub_0808A3DC/W88-notes.md.

### Wave 93

WAVE 93 (W93-A): 48.17% size+4 -> 60.00% SIZE-EXACT, first difference +0x4, under the DEFAULT -O2 profile. The draft was simply the worse of the two files already in the work directory: best.c has been the better base since wave 46 and nobody installed it. The only differences are `u16 fill` rather than `vu16 fill` and no cast on CpuSet's first argument; putting either back costs 12 points and 4 bytes. Wave-start copy in sub_0808A3DC.w93-start.c. *** A 900 s PERMUTER RUN REPORTED 60.00 -> 77.50 SIZE-EXACT AND THE FORM IS SEMANTICALLY WRONG. IT WAS REJECTED AND THE DRAFT IS BACK AT 60.00%. *** The mutation moved `fill = 0;` to AFTER the `CpuSet(&fill, ..., 0x01000400)` that reads it. That call is a FILL: mode bit 24 is set, so the BIOS reads the source halfword once and writes it 0x400 times. With the store moved after it the tilemap buffer is filled from an uninitialised stack slot, and the first half of the buffer is not overwritten by the Decompress that follows (it lands at +0x200). The permuter's OTHER mutation, holding the loop's 0x1020 addend in a variable assigned inside the loop body, is legitimate -- and ON ITS OWN it is 33.75%, WELL BELOW the 60.00% base, so the entire 17.5 points came from the illegal move. work/sub_0808A3DC/best.c and best.json have been overwritten with the valid 60.00% draft so the false score does not propagate (wave-73 precedent); the rejected source is preserved as w93-permuter-invalid.c and under permuter/output-880-1/. CONFIGURATION SETTLED, AND THE wave60_o1 KEY IS WRONG FOR THIS FUNCTION: the matched neighbours from 0x0808A2F4 to 0x0808AA88 carry NO override, the -O1 region starts at sub_0808AB8C, and this function is size-exact at the default -O2 while o1 and o1-no-force are not. THE PARK'S remaining_diff IS STALE -- THE ADDRESS-WORD INDIRECTION IS ALREADY CORRECT. best.c emits `ldr r6,[pc]; ldr r4,[r6]; ldr r1,[r4]` and `ldr r5,[pc]; ldr r1,[r5]; ldr r1,[r1]`, matching the ROM's three-load chains for gUnknown_0849957C and gUnknown_03001FE8 exactly, with the word addresses held in r6 and r5 as the ROM has them. That problem is solved. THE ENTRY'S UNTESTED IDEA IS NOW TESTED AND IS A NEGATIVE: reading the buffer pointer as element 1 of the table at 0x08499578, spelled `(&gUnknown_08499578)[1]`, is 13.41% at size+4. Also negative: a volatile read of the pointer global (`*(u16 *volatile *)&gUnknown_0849957C`) at all four uses is BYTE-IDENTICAL to the plain name, with or without a volatile read of gUnknown_03001FE8 -- the W87-A lever that worked on sub_0805D344 does not transfer, because the address materialisation here is already right. WHAT IS ACTUALLY LEFT, 64 of 160 bytes in TWO places, neither about addresses: (1) the stack address of `fill` is computed TWICE in the ROM (`mov r1,sp` for the store, `mov r0,sp` again as CpuSet's first argument) and once in the candidate; `*(u16 *)&fill = 0;` and binding a `u16 *fp = &fill;` for the store both drop to 48.17% at size+4. This is the same shape as sub_0808AAF4's twice-computed `add r1,sp,#0x40`, and the permuter only 'solved' it by breaking the semantics, which is itself evidence that no correct statement order reaches it. (2) the loop's 0x1020 addend is hoisted into the preheader with a copy where the ROM rebuilds it every iteration; writing the read-modify-write out in full instead of `+=` is byte-identical, and holding it in a variable assigned inside the loop is 33.75%. Flag diagnosis: -fno-rerun-loop-opt changes NOTHING (the hoist is the first loop pass, not the rerun), -fno-rerun-cse-after-loop is 18.02% at +12 and -fno-gcse 20.62% at -4.

### Wave 97

wave 97 (W97-M)
Base: current draft (60.00%, size-exact), unchanged. Probes with tools/spellings.py, 11 spellings:
- fill zero store: `u16 fill = 0;` initialiser, `u16 fill[1]`, a `z = 0` temp, `u32 fill` (55.6%): the ROM's `mov r1,sp; movs r0,#0; strh r0,[r1]` (address first, value second,
  address recomputed for CpuSet) never appears; all are byte-identical to the draft or worse. `struct{u16 v;}` +8 bytes, `u8 fillb[2]` +8, `*fp = 0` bind +4 (48.17%, as before).
- loop addend: `= (u16)(x + 0x1020)` and `|= 0x1020` byte-identical; a `t` temp byte-identical; `+ 0x81 * 32` (spelt as multiply) folds back, byte-identical.
No source construct found that keeps the 0x1020 rebuild (`movs r7,#0x81; lsls r7,#5`) inside the loop. The hoist is loop.c's; not source-reachable with these spellings.
Proposed summary: unchanged.

</details>
