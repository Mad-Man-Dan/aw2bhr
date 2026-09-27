# sub_080611D8

0x080611D8, 304 bytes, THUMB, parked.

Best score so far: 93.4% (best.c).

## What it does

AI: picks a map cell for a unit of type gUnknown_030046C0.unk06 and writes its x and y to the caller's buffer, returning 1 on success and 0 otherwise. It first tries sub_08061308 in up to five search modes, then falls back to the first unused record in a ROM list of cells.

## How close it is

Right size (304 of 304 bytes) and 93% of them now line up; the first difference is in the last twenty bytes. The ROM keeps two separate `return 1` endings and one shared `return 0` placed last, and holding the success value in a local rather than writing the literal 1 at both endings is what stops the compiler merging them.

## What is left

Close the last twenty bytes, from offset 0xde: the two return endings are now arranged as the ROM has them, so what is left is after them.

## Already tried

- Nesting the whole chain of sub_08061308 calls in an `if` inside the unk07 test: the worst layout of all.
- Inverting the final test (`if (out[0] != 9999) { mark; return 1; } return 0;`): changes which return is shared but does not fix it.
- A single exit through a result variable: the compiler turns the last result into branch-free arithmetic, further from the ROM.
- Explicit labels for each return (a trailing `ret0:` plus separate return-1 labels): gives the ROM's endings but moves the constant pool 16 bytes later.
- Switch and do-while spellings of the same logic: come out like one of the attempts above.
- Kept from earlier attempts: `break` in the list walk and the literal 9999 in the final test, which fixed the loop.
- Writing the literal 1 at both `return 1` endings while keeping everything else: measured, and it puts the whole 76-byte residual straight back. The non-constant return is load-bearing.

## Files

- `sub_080611D8.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at exact size 304/304 with 76 positional bytes different and an exact instruction multiset. ROM keeps two short return-1 copies and merges the final return-0 before the epilogue; every structured spelling merges return-1 instead. Wave 70's labeled late-ret0 CFG reaches the desired tails but shifts the four-word pool barrier by 16 bytes. Residual is basic-block/pool layout only.

### Wave 93

- **agent:** W93-E
- **result:** 75.00% -> 93.42%, size-exact 304/304, first difference +0x10 -> +0xde
- **how:** First permuter run this function has ever had (900 s, 4 threads, --current). It found the lever six hand spellings across waves 70-92 had missed: bind the success value to a local `r` (`r = 1;` before the list-walk's final test, `r = sub_08061668(out);` on the alt path) and `return r;` at BOTH endings. `r` is provably 1 on the first ending and not on the second, so the late jump2 cross-jumping pass no longer merges the two `return 1` tails -- which was the entire parked residual.
- **audit:** PASSED. sub_08061668 is promoted (src/decomp/c_08061668.c) and returns only 0 or 1, so `return r;` on the alt path is the same function as `return 1;`; the compiler simply cannot prove it. No read-before-set, no duplicated or dropped call, no store moved across a call. The permuter's `char t` mutation was reverted to `u8 t` and measured byte-identical, which also confirms plain char is unsigned under agbcc here.
- **measured_negative:** work/sub_080611D8/w93-p1-clean.c -- the same form with the alt path's return written as the literal 1: 75.00%, first difference back at +0x10.
- **files:** work/sub_080611D8/NOTES.md, perm-w93-1.log, sub_080611D8.w93-start.c, sub_080611D8.w93-r1.c

</details>
