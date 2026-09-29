# sub_08031824

0x08031824, 292 bytes, THUMB, parked.

Best score so far: 97.3%.

## What it does

Fills the record gUnknown_02025764: copies 2, 3 and 24 header bytes from gUnknown_02028040, gUnknown_0202805A and gUnknown_02028042, then for each of three 28-byte slots stores the byte from sub_0803CD14(slot), a 17-byte block from sub_0803CCB8 (zeros if it fails), 5 bytes from gUnknown_020280D4 and two bytes from gUnknown_020280C0[slot]. What the record is for is not known.

## How close it is

Compiles to the right size (292 bytes); 8 bytes differ (97.3% identical), all at three places where the compiler picks a different temporary register. Every instruction is otherwise the original's.

## What is left

At each of the three places a value held in a high register must pass through a low one for one instruction, and the compiler takes the lowest free register. The original's higher choice means some other value was still alive there in the original source; find which value, and keep it alive across those three places.

## Already tried

- Writing the else-arm clearing loop counting down by hand: same loop body, but its setup lands in the wrong order. The source loop counts up and the compiler reverses it.
- One local for both bytes copied at the end of the loop: the original clearly uses two.
- Permuter with the old scoring: two chained 5-minute runs, about 30,600 attempts; nothing past 97.3%.
- Permuter with corrected scoring: a free 15-minute run (11,049 attempts) and a directed one shuffling all declarations, the function's start and the copy setup (10,679 attempts): nothing better.
- Other compiler settings: none match.

## Files

- `sub_08031824.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

97.3% -- work/sub_08031824/sub_08031824.c (== w90-start.c == best.c)

### What still differs

Three register picks. Loop 1's `mov r0, r8` (ROM r1); `ldr r0, =gUnknown_020280C0; mov sl, r0` (ROM r1); the copy setup's `ldr r1` pair (ROM r3). The instruction stream is otherwise the ROM's.

### Why it is close

All three picks are RELOAD spill registers, not local_alloc choices: the .greg dump prints `Spilling for insn N. Spilling reg 0.` at each (a hi-reg pseudo, p in r8 or c in sl, needs a lo register for one insn). reload1.c find_reg takes the lowest spill_cost and breaks ties by REG_ALLOC_ORDER, which starts at r0. The ROM's r1/r3 mean some pseudo allocated to r0 (and r1/r2 at the third) was live across those insns in the original.

### Already ruled out

- Descending else-arm loop written by hand (W88): keeps the init in the source-bind slot; the source loop is ascending and check_dbra_loop reverses it (W89-B)
- One tail local for both ldrb values (W89-B): the ROM has two locals v and v2
- Permuter, old scorer: two chained 300 s runs, ~30.6k it (W89-B); the second found nothing past 97.3%
- Permuter, fixed scorer (PENALTY_REGALLOC 60, --stack-diffs): undirected 900 s 11,049 it and directed 900 s 10,679 it (LINESWAP over all 14 declarations + RANDOMIZE over the prologue and the 5-byte copy setup; w90-directed.perm.txt), both nothing better than 480 (W90-A)

### Settled

- The else arm is an ascending loop that gcc reverses; the if arm is not reversed because its biv feeds two givs (W89-B)
- `v = (v2 = sub_0803CD14(k));` is the 97.3% step (W89-B permuter)
- The residual picks are reload spill registers chosen by find_reg; the lever is which pseudos are live across those three insns, not how the insns are spelled (W90-A, .greg dump)

### Why it is parked

Parked in wave 90 (W90-A) after W88, W89 and W90 worked it. Four permuter runs across both scorers (~52k iterations) never moved the three reload picks. Notes: work/sub_08031824/W88-notes.md, W89-notes.md, W90-notes.md.

### Wave 90

W90-A: unchanged 97.3%/8 bytes. The fixed-objective permuter found nothing in 2 x 900 s (11,049 undirected, 10,679 directed). The .greg dump shows that the three residual picks W88/W89 called bare local_alloc scratch picks are reload spill registers (find_reg: spill_cost, then REG_ALLOC_ORDER from r0). The next attempt should make a pseudo that lives in r0 stay live across loop 1's r8 read, the gUnknown_020280C0 bind, and the copy setup.

### Wave 92

W92-C: unchanged 97.26%/8 bytes, size-exact, first difference +0x14. NEWLY RULED OUT, each measured: (a) separate counters for the three header loops (i / i2 / i3 in place of one reused i) is BYTE-IDENTICAL -- they coalesce, so this adds no pressure and is not a lever; (b) binding `c = gUnknown_020280C0` BEFORE `j = 0` instead of after gives 12 bytes / 95.89%, confirming the bind must follow the counter init; (c) a pointer local for loop 1's destination does not compile at all -- the member is volatile, so `&p->unk00[i]` will not convert to `u8 *`. A fresh -dg dump (work/sub_08031824/rtl-w92/) reconfirms W90-A: the three picks are reload spill registers (`Spilling for insn 11/26/64/167. Spilling reg 0.`) chosen by spill cost, so the lever is unchanged -- make a pseudo that holds r0 live across those points. A 900 s permuter chain from --current ran this wave (perm-w92-1.log). A 900 s permuter chain from --current ran this wave (13,907 iterations, perm-w92-1.log): nothing better than the 480 base score.

### Wave 93

W93-F: one 900 s run from the draft under the new length-penalty scorer returned nothing.

### Wave 97

wave 97 (W97-M)
Base: the 97.26% draft, unchanged. No improvement. Hypotheses (pre-registered: the r0/r1 scratch pick is moved by respelling how the 5-byte copy's two addresses
or the `c` bind are formed) did not hold. Measured with tools/spellings.py (size 292 every time, first diff always +0x14):
- 5-byte copy: `i = 4` before/after the src/dst binds 93.8/97.3%; dst before src 95.2%; `(int)gUnknown_020280D4 + j*0x1c` order 95.6%;
  shared `off2` 92.8% (first diff moves to +0x67, worse); ascending `for (i = 0; i < 5; i++)` 97.26% (byte-identical); pointer arithmetic on u8* 95.6%.
- `c = gUnknown_020280C0` bind: before `j = 0` 97.26%; `&gUnknown_020280C0[0]` 95.9%; cast, a dead `d5 = 0`, a trailing `i = 0`, a dead `v2 = 0` before it: all byte-identical 97.26%.
So the scratch pick is insensitive to every statement-order/spelling around it; the wave-92 finding (the pick is made in reload's scratch choice) stands.
Proposed summary: unchanged (status 97.26% size-exact; left: three scratch-register picks r0/r1 vs r1/r3).

wave 97 (W97-PG)
Permuter chain: 1 link, 97.26% -> 97.26%, NO-IMPROVEMENT.

</details>
