
## wave 95

Base: `best.c` (explicit `case 0: break;`, 676 bytes, +4, 49.3%). Result: **61.8%, size-exact (672)**,
draft `sub_08046A84.c` (also `v7.c`; the old draft kept as `sub_08046A84.w95-start.c`).

What moved it:
- **The `+ 0x50` bind, per loop.** The park tried it in both loops at once (664, worse). Only in the case-1
  loop, on the explicit-`case 0` base: `x = gUnknown_084C2112[i * 2] + 0x50;` then `x + a` as its own
  statement -> 672 (size-exact), 61.3%. Case-2-only: +4 (50%). Base without `case 0`: -8 / -4.
  So the two named residuals were not independent: the bind (-4) exactly cancels the explicit `case 0` (+4).
- Casting the case-2 loop bound to non-const, `((struct Unk085D583C *)&gUnknown_085D583C[b])->defense`, makes
  the bound be re-read through the row address (the ROM reloads `defense` each iteration through the held row
  pointer; the const table makes agbcc hoist it). Case-2 loop only: 61.8%, still 672. In the case-1 loop it
  costs +4 (the bind and the non-const read conflict).

Not reproduced: the first ternary's layout. The ROM has `cmp #6; beq A; cmp #8; bne C; <8 arm>; b join;
<6 arm>` (the 6 arm out of line), the draft `cmp #6; bne; <6 arm>...`. Writing it as
`b != 6 ? (b == 8 ? X8 : C) : X6` merges the arms' loads and shrinks 16-20 bytes (12.95%, 15.0%): wrong.
First difference stays +0x1e. Remaining diffs otherwise: the register of the row address (`ldr r1` vs `ldr r0`)
and add order in the second loop.
The W95-B note (forced `.rodata` word then plain literal after a join) was checked: the two literal-pool
differences here (`gUnknown_085D5ABC+0x54` vs `gUnknown_085D5B10`) are the same-address class, not that.
The one-temp-per-block lever transferred: YES in effect (per-loop temp `x`, not one shared across both loops).

### wave 95, permuter (chained 600 s runs, 2 threads, from the 61.8% size-exact draft)

61.8% -> 63.0% -> 66.8% (size-exact 672, kept in `sub_08046A84.c` = `.w95-perm2-out.c`). Kept mutations, read:
a `const struct Unk085D583C *new_var` row pointer, `new_var2 = ...movementChart[gPlaySt.weather];` (row of the
movement chart bound, then indexed), `(gUnknown_085D583C + b)->defense`. All the same C.
**Run 3 reported 83.3% and is WRONG C -- rejected.** It added `volatile int new_var3; new_var3 = 0x50;` and used
`new_var3` for the `+ 0x50`. The volatile local makes a stack slot the ROM lacks: `sub sp,#20` against the ROM's
`#16`, first difference moves EARLIER (+0x1e -> +0xa). The banner was a frame artefact, exactly the wave-94
pattern. Kept file `.w95-perm3-out.c` only as evidence; the permuter was killed during run 4. The `+ 0x50`
order residual is therefore still open; the permuter's own answer for it (a stored constant) is not the ROM's.
Residual as before: the first ternary's layout (+0x1e), the row-address registers.
