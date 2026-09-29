# sub_080560A4

0x080560A4, 680 bytes, THUMB, parked.

Best score so far: 39.0%.

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
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 684/680 (+4 section; code-size exact), 25.4%. The matched sibling's flat fill-loop spelling plus a separate (u16)(i - 1) improves 24.9% to 25.4%; residual is missing-live/address allocation.

### Wave 95

Base: existing draft (684/680, +4, 25.3%, carries the `new_var` from an earlier permuter run).
- ROM slot [sp,#4] holds the spilled `q` (= gUnknown_08554A00[b*5+t], stored before q[200] and reloaded for q[201]); `b * 20` lives in r8 across both loops as a shared base (`lsls r0,i,#2; add r0,r8` for gUnknown_0855218C[b][i][0] and for the 08554A00 lookup).
- Hypothesis tried: hoist `b20 = b * 20` and spell both lookups as byte-offset reads `(u8 *)tbl + b20 + t * 4`: 692 (+12), 17.8%. Worse. The extra source local costs pressure but the cast spelling also loses the array's scaled-index form.
- Not permuter-run.

### Wave 97

wave 97 (W97-G)
Base: draft (25.29%, 684/680). Hand probe `for (i = n; ...)` was byte-identical (cse folds n = 0 to a fresh constant).
One permuter run (900 s, --threads 2, from the draft): **25.29% -> 38.97%, still 680 bytes size-exact, first
difference +0xa**. Two semantic changes, both read and valid (wrongc.py OK, 230 seeds): the branch-1 store index is
computed before the increment, `j = a - 1; n++; ... [j] = gUnknown_08552148[b]` (j is dead-then-reassigned, so a
distinct short-lived pseudo for the `a - 1`), and the second guard is spelled `a > n` instead of `n < a`.
Rest is reformatting. `j` reuse and the comparison flip are the moves; the size delta +4 is now 0.

Residual: prologue allocation (ROM keeps `a` in ip via `mov ip, r0` with `sub sp, #8`, b in r7, n in r6; draft has a in r7,
b in r5, `sub sp, #4`), and `adds r3, r6, #0` for the loop counter start. Not chained further (turn budget).

Proposed summary: does = builds the list of unit slots to show for side b (first branch copies preset slots, second
picks changed then unused slots), marks a of them and adds any surplus, then fills the row from the offset table and
calls sub_08056638. status = 39.0% size-exact. left = whole-function register assignment differs from the prologue on.
tried = un-bound `b*20` byte-offset form (worse), `i = n`, permuter.

</details>
