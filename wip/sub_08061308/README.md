# sub_08061308

0x08061308, 864 bytes, THUMB, parked.

Best score so far: 13.5%, -20 bytes.

## What it does

AI: chooses a target cell for the current unit type. For each suitable empty record in a ROM list of cells, it computes the cells reachable from there, keeps those that pass the test chosen by a2 (terrain, transports, ports, infantry, or flagged unit types), and lets sub_08059C00 pick one. Returns 0 as soon as a search finds nothing, otherwise 1.

## How close it is

Compiles 20 bytes short of 864; 13.5% of bytes are in place, low because the shortfall starts near the top. 12 bytes are register choice: the ROM keeps the address of the map pointer in a saved register and pays a copy before each of the five switch cases, the draft uses it directly. The other 8 are two constants (gUnknown_030013EC and gUnknown_030046C0) that the ROM stores twice, once per arm of `if (a2 == 4)`, and the draft shares.

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

</details>
