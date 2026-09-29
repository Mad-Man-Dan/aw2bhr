# sub_08061308

0x08061308, 864 bytes, THUMB, parked.

Best score so far: 25.2%.

## What it does

AI: chooses a target cell for the current unit type. For each suitable empty record in a ROM list of cells, it computes the cells reachable from there, keeps those that pass the test chosen by a2 (terrain, transports, ports, infantry, or flagged unit types), and lets sub_08059C00 pick one. Returns 0 as soon as a search finds nothing, otherwise 1.

## How close it is

Compiles to the right size (864 bytes) with 25.2% of bytes identical. Binding the address of gMap inside the first test and reading the map through it only in the switch cases fixed the size. What is left: one extra stack slot (the draft spills i * 4, which the original keeps in r7).

## What is left

Reproduce the ROM's register choice for the map pointer's address and its duplicated constants. Worth one probe first: reach the map through gMap and the named members of struct Map in include/map.h (width, height, unit, terrain, rowOffset, move) instead of the file-local cast with unk names and the one raw byte offset for the movement plane, since how the map pointer is reached is where the 12 bytes are.

## Already tried

- Writing case 2's accept step inside case 2 before `continue`: reproduces the ROM's duplicated block and gained 8 bytes (kept).
- Volatile, byte-pointer and bound-local spellings of the first gUnknown_030046C0.unk06 read: its address stays in the same register; no change.
- Binding the map pointer's pool address to a fixed-register local: the stack frame grows from 16 to 20 bytes.
- A separate function-pointer local for the callback in each arm of `a2 == 4`: splits the constant as the ROM does but overshoots to 872 bytes (38.2%). A volatile function-pointer view compiles the same as the draft.
- The older compiler: 828 bytes, and 804 without the force-addr option.
- Permuter runs: nothing matched. The saved best (best.c, 860 bytes, 45.6%) is not usable: it is built on an older draft and in case 4 it reads the unit pointer before setting it.

## Files

- `sub_08061308.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 844/864 (-20 section, -12 code), 13.5%. Branch-local function-pointer bindings split the references but overshoot to 872; a volatile function-pointer view is byte-identical. Residual is the r7/r8 allocation swap and duplicate pool words.

### Wave 95

Base: existing draft (844/864, -20, 13.5%), kept as `sub_08061308.w95-start.c`. Now SIZE-EXACT (864), 25.2%, first difference +0xa (frame is `sub sp,#20`, ROM #16: one extra slot, the spilled `i * 4`).
- `gMap` / `struct Map` members (width, height, unit, terrain, rowOffset, move) in place of the file-local cast: BYTE-IDENTICAL to the cast draft (844, 13.5%). The park's named probe changes nothing; it is kept because it is readable.
- The lever is the bind of the map pointer's ADDRESS: `struct Map **mp = &gMap;` bound inside `if (gUnknown_030045C8 != a1)` before the k loop, with every switch-case read written `(*mp)->...` and everything else (k-loop head, loop bounds, `gMap->move`) left on bare `gMap`. That is the chapter "bind the address and leave the first reference bare"; it is what buys the ROM's r8 copy of the address word and the extra `mov r0,r8` before each case, and turns -20 into exact size.
- Variants: mp bound at function top: exact size, 24.2%; bound in the loop body before the first map read: -8, 16.0%; bound after the first compare: -4, 14.4%; `(*mp)` also in the i/j loop bounds: -8, 16.7%.
- Permuter (900 s, 2 threads, chained run 1): NO-IMPROVEMENT (raw form 29.9% only).
- Residual: the ROM keeps `i * 4` in r7 (no slot); the draft spills it to [sp,#16]; the ROM's 030046C0 read uses r3 and the ROM copies the bound word after the first bare read (`mov r8,r2` mid-loop), the draft copies at the loop entry.
- The .rodata pool word for the map pointer: candidate emits `.rodata` for `gMap`; the ROM's word is gUnknown_0816DAF4 (same address); pool words to record if this ever matches: 0816DAEC, 0816DAF0, 0816DAF4.
- Transfer test of the sub_08022BB8 lever (two variables, compare temp assigned before the bound value, one temp per block): NOT applicable, no probe run. That lever needs a narrowed or compare form of the same value beside an `adds rN,rM,#0` copy; here the copy is of the map pointer's ADDRESS word and there is no narrowed twin. The address bind already reproduces the copy (see above).

### Wave 96

Base: unchanged draft (864, 25.2%, first +0xa). Screened per rule 2 (delta +3 copies).
Reading of the ROM (target.s): the compiler makes THREE distinct `.rodata` cells that hold &gUnknown_03003F20
(0816DAEC), &gMap (0816DAF0 for the loop head, 0816DAF4 for the body). At the body start the ROM does
`ldr r2,=0816DAF4; ldr r0,[r2]; ldr r3,[r0]` and later `mov r8,r2` -- r8 HOLDS THE CELL ADDRESS (the literal
pool word), and every case arm reads the map as `mov r1,r8; ldr r0,[r1]; ldr r1,[r0]` (two loads). The draft's
`mp = &gMap` instead loads the cell once (`ldr r1,=cell; ldr r2,[r1]; mov r8,r2`), so r8 holds &gMap and each arm
needs ONE load. So the held-vs-re-derived question is answered ("held", per rule 1), but the held object is the
compiler's own cell, which has no C name (unlike sub_08050FF8 where the cell is a real ROM object).
Probe (one-unit harness, 5 placements of `mp = &gMap`): before the i loop gives the ROM's frame (`sub sp,#16`, the
i*4 slot disappears) but size 852 (-12), 21.5%, first diff +0x16 -- saved as `.w96-e-bindbeforeloop.c`, not adopted
(loses the size). After the sub_0801F92C call / before it: 864, frame unchanged. Before the unit test: 860.
Residual: the two-load cell chase in every arm and the mid-loop `mov r8,r2` timing.
Proposed summary: left: r8 holds the compiler's cell address (two loads per arm); draft holds &gMap (one).
tried: mp bound at 6 positions; binding before the row loop fixes the frame but is 12 bytes short.

### Wave 97

wave 97 (W97-X)
Base unchanged (25.23%, size-exact; kept as `sub_08061308.w97x-start.c`). levers.py finds only +1%: `5a-65+4a-101` (26.27%, wrongc OK) = the table row read as a struct copy `lv = gUnknown_085766E4[k]` plus swapping the arms of `if (a2 == 4)`; not adopted (a one-point gain that does not move the first difference off +0xa, and it adds a struct copy that says nothing about the ROM). The ROM's r8 holds the address of the compiler's own .rodata cell for &gMap (three such cells, one per region), which has no C name, so no lever in the pool can name it. Residual unchanged: two loads per arm vs one.

</details>
