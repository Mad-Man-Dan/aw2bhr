# sub_080560A4

0x080560A4, 680 bytes, THUMB, parked.

Best score so far: 25.3%, +4 bytes.

## What it does

Chooses which slots side `b` puts in its list gUnknown_0202980A[b], up to `a` entries, preferring slots whose unk00 and unk01 differ, then fills and sorts that side's sort keys (sub_08056638). Only the first two parameters are used.

## How close it is

Compiles 4 bytes too long (684 against 680). 507 of 680 bytes differ (25.4% identical), which means little because the difference starts at the top. Every block is in the original's order; what differs is which table address the first branch computes first, and register allocation: the original keeps one more value live and spills two values where the draft spills one.

## What is left

Find the value the original keeps live that the draft does not. It shows up as the second branch scaling b * 20 and i * 4 separately instead of computing (b * 5 + i) * 4, and as a second stack slot. The fixes found so far for the first branch's address order remove the size difference but make the register choices worse.

## Already tried

- A row or element pointer local as the first statement of the first loop: right address order and right size, but 15.6% identical.
- A scalar temporary in the first loop (`t = source; dest = t;`): right size (680 bytes), 19.1%; a block-local `u16 v` instead: 18.5%.
- A row pointer for gUnknown_0855218C in the second branch: 24 bytes too long (9.9%); combined with the scalar temporary: 20 bytes too long (15.1%).

## Files

- `sub_080560A4.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 684/680 (+4 section; code-size exact), 25.4%. The matched sibling's flat fill-loop spelling plus a separate (u16)(i - 1) improves 24.9% to 25.4%; residual is missing-live/address allocation.

</details>
