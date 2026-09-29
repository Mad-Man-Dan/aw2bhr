# sub_0808A3DC — builds a BG screen from compressed data (160 bytes)

## Wave 93 (W93-A): 48.17% size+4 -> 60.00% size-exact, first difference +0x4

The draft was the worse of the two files already in the work directory.
`best.c` has been the better base since wave 46 and nobody installed it. The
only differences are `u16 fill` rather than `vu16 fill`, and no cast on
CpuSet's first argument. Both matter: putting either one back costs 12 points
and 4 bytes.

## A permuter run reported 77.50% and the form is WRONG — rejected

The 900 s run moved `fill = 0;` to **after** the
`CpuSet(&fill, ..., 0x01000400)` that reads it. Mode bit 24 is set, so that
call is a FILL: the BIOS reads the source halfword once and writes it 0x400
times. With the store moved after it the tilemap buffer is filled from an
uninitialised stack slot, and the first half of the buffer is not overwritten
by the Decompress that follows — that one lands at `+0x200`.

Its other mutation, holding the loop's `0x1020` addend in a variable assigned
inside the loop body, is legitimate. **On its own it measures 33.75%, well
below the 60.00% base**, so the entire 17.5 points came from the illegal move.

`best.c` and `best.json` have been overwritten with the valid 60.00% draft so
the false score does not propagate (wave-73 precedent). The rejected source is
kept as `w93-permuter-invalid.c` and under `permuter/output-880-1/`.

## Configuration settled — the `wave60_o1` key is wrong for this function

The matched neighbours from 0x0808A2F4 to 0x0808AA88 carry no override, the
-O1 region of `data/compiler-overrides.json` starts at sub_0808AB8C, and this
function is size-exact at the default -O2 while o1 and o1-no-force are not.

## The park's `remaining_diff` is stale — the address words are already right

`best.c` emits `ldr r6,[pc]; ldr r4,[r6]; ldr r1,[r4]` and
`ldr r5,[pc]; ldr r1,[r5]; ldr r1,[r1]`, matching the ROM's three-load chains
for gUnknown_0849957C and gUnknown_03001FE8 exactly, with the word addresses
held in r6 and r5 as the ROM has them. "Make the code use those address words"
is solved.

The entry's untested idea is now tested and is a **negative**:
`(&gUnknown_08499578)[1]` is 13.41% at size+4. Also negative — and worth
knowing, because it is the lever that worked on sub_0805D344 in the same wave —
a volatile read of the pointer global (`*(u16 *volatile *)&gUnknown_0849957C`)
at all four uses is byte-identical to the plain name, with or without a
volatile read of gUnknown_03001FE8. It does not transfer, because the address
materialisation here is already correct.

## What is actually left — 64 of 160 bytes, two places, neither about addresses

1. **The stack address of `fill` is computed twice in the ROM, once in the
   candidate.** `mov r1,sp` for the store, then `mov r0,sp` again as CpuSet's
   first argument. Negative: `*(u16 *)&fill = 0;` and binding a
   `u16 *fp = &fill;` for the store both drop to 48.17% at size+4. Same shape
   as sub_0808AAF4's twice-computed `add r1,sp,#0x40` — and the permuter only
   "solved" it by breaking the semantics, which is itself evidence that no
   correct statement order reaches it.
2. **The loop's addend is hoisted.** The ROM rebuilds 0x1020 inside the loop
   every iteration (`movs r7,#129; lsls r7,r7,#5`); the candidate hoists it
   into the preheader and pays a copy, which also swaps the registers holding
   0x1020 and the 0x3FF bound. Writing the read-modify-write out in full
   instead of `+=` is byte-identical; holding the addend in a variable
   assigned inside the loop is 33.75%.

Flag diagnosis: `-fno-rerun-loop-opt` changes nothing, so the hoist is the
first loop pass and not the rerun; `-fno-rerun-cse-after-loop` is 18.02% at
+12 and `-fno-gcse` 20.62% at -4.

## wave 97 (W97-M)
Base: current draft (60.00%, size-exact), unchanged. Probes with tools/spellings.py, 11 spellings:
- fill zero store: `u16 fill = 0;` initialiser, `u16 fill[1]`, a `z = 0` temp, `u32 fill` (55.6%): the ROM's `mov r1,sp; movs r0,#0; strh r0,[r1]` (address first, value second,
  address recomputed for CpuSet) never appears; all are byte-identical to the draft or worse. `struct{u16 v;}` +8 bytes, `u8 fillb[2]` +8, `*fp = 0` bind +4 (48.17%, as before).
- loop addend: `= (u16)(x + 0x1020)` and `|= 0x1020` byte-identical; a `t` temp byte-identical; `+ 0x81 * 32` (spelt as multiply) folds back, byte-identical.
No source construct found that keeps the 0x1020 rebuild (`movs r7,#0x81; lsls r7,#5`) inside the loop. The hoist is loop.c's; not source-reachable with these spellings.
Proposed summary: unchanged.
