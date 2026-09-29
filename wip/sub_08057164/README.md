# sub_08057164

0x08057164, 268 bytes, THUMB, parked.

Best score so far: 32.5%.

## What it does

Fills the five entries of gUnknown_02029A10[y]. Each entry's unk1a comes from gUnknown_0855203C, gUnknown_08551F60 or gUnknown_08551E84, chosen by two flags in the gUnknown_085D6A48 row for gUnknown_03004582[y][0], at index a * 10 + y * 5 + i (b instead of a for gUnknown_0855203C when y is not gUnknown_0300450C). unk00 and unk01 are bytes from gUnknown_085521DC at y * 55 + a * 5 + i and y * 55 + b * 5 + i, and unk02, unk04 and unk06 are cleared.

## How close it is

Compiles to the right size (268 bytes) with 32.5% of bytes identical, but with a 4-byte frame where the original's is 12, so the size match is by a different shape. What is left: the loop pass merges the separate index walker with i.

## What is left

Make the compiler step one element index by 1 and scale it at each table read, as the original does, instead of also turning the gUnknown_0855203C read into a separate stepping pointer. The original then keeps gUnknown_0855203C's address in a register and reloads gUnknown_085521DC's inside the loop.

## Already tried

- Binding `tbl = gUnknown_0855203C` before the loop: the compiler still builds a stepping pointer from it.
- A real `idx` counter incremented at the bottom of the loop, with an explicit record-pointer walk: gives the original's step-by-1 index, but the stepping pointer for gUnknown_0855203C is still created.
- Removing `tbl` in that form: no change.

## Files

- `sub_08057164.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at exact size 268/268. All statements and branches are present; the residual is strength_reduce retaining an address GIV for gUnknown_0855203C where ROM keeps an element-index IV. A bound table pointer and Wave 70's explicit idx counter plus record-pointer walk still form the address GIV. The new source counter does recover the desired +1 element IV, proving the remaining problem is the second GIV decision rather than loop semantics.

### Wave 93

W93-E: two negatives recorded in work/sub_08057164/NOTES.md; never permuted yet.

### Wave 94

W94-B: first permuter run on this function (--current, 900 s, 4 threads, length-penalised scorer). It reported and KEPT an improvement, 20.90% -> 30.22% size-exact -- and the form is WRONG C, now work/sub_08057164/w94b-perm1-idx-clobber.c.wrongc. It reassigns `idx` to `a * 5` inside the loop body, after the three unk1a table reads that use `idx` have already run once, so four of the five entries read the wrong table element. Neither guard can see it: every read follows an assignment, and the length does not change. Draft restored and re-verified at 268/268, 20.90%, first difference +0xe. Screen that works here: this candidate and both wave-93 negatives all move the first difference EARLIER, from +0xe to +0xa, while scoring higher -- on this function that signature marks a reorganised loop opening, not progress. The GIV residual in `left` is untouched.

### Wave 96

Base: draft unchanged (20.90%, size+0, first diff +0xe). One 900 s permuter run (2 threads): reported IMPROVED 20.90 -> 26.87%, first diff +0xa (earlier), and it is WRONG C: it introduced `idx = 0; e[i].unk02 = idx; e[i].unk06 = idx;` at the bottom of the body, which clobbers `idx` (the per-side table index) for passes 2-5 of the loop. Kept as `sub_08057164.w96-perm1-idx0-clobber.c.wrongc`; draft restored. Same signature as the wave-94 rejection (`idx` reassigned inside the loop, first diff moving earlier).
Reading the ROM again: the ROM keeps `tbl` (0855203C) in sl as a register and steps the element index `idx + i` as its own +1 walker (r7) with an explicit `lsls #1; add sl` at each of its three uses, and `e` as a +0x24 walker (r5), `i` in sb, `b` in ip, `y` in r6, `&gUnknown_03004582[y]` spilled at [sp] (hoisted row address), `y*5` at [sp+4]. The draft folds `tbl + (idx+i)*2` into one address walker (steps by 2) — the GIV noted in wave 93. Also tried `for (...; i++, idx++)` with `tbl[idx]` reads: 20.2%, +4 bytes, first diff +0xc. No lever found to keep the idx+i giv unmerged with the base symbol.
Proposed status: unchanged; left = whole-loop register roles and the unmerged idx+i walker.

### Wave 97

wave 97 (W97-Y)
Base: draft (20.90%, 268). levers.py's 30.97% `5b-94+5b-32` is WRONG C though wrongc says OK: it assigns `idx = (b*10 + y*5) + i` inside the loop's else arm, so idx (the a-side index for passes 2-5) is clobbered. Rejected.
Separate-walker lever (W96-C's lead): `j = idx;` before the loop, `tbl[j]`/`gUnknown_08551F60[j]`/`gUnknown_08551E84[j]` for the three unk1a reads, `j++` at the loop end -> 272 bytes (+4), 20.96%. Copy-back forms (`nx = i + 1; j++; i = nx;`, `j` copy-back, i and j orders) are byte-identical: loop.c still reduces `j` and eliminates `i` (the exit test becomes a compare on the walker), so the ROM's separate `i` in sb (`movs r0,#1; add sb,r0`) is not reproduced by any step form.
Permuter (3 links of ~500 s from the walker form, each wrongc OK and diffed): the kept edits are a do { } while (0) around the unk1a if/else chain and `j++` moved inside a second do-while; size back to 268 exactly, 32.46%, first diff +0xa. This is size-exact only by another shape: frame is `sub sp, #4` against the ROM's `#12` and `tbl + j` is still one pointer walker (`ldr r7,[sp]`), so it is not the ROM's structure.
Proposed status: 32.5% (size-exact, frame #4 vs #12); left = ROM keeps tbl in sl, idx+i as its own +1 walker in r7, i in sb, and two spilled row/product slots; no source spelling tried keeps i alive next to the idx walker.

</details>
