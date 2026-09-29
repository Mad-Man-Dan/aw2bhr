# sub_0801E9B0

0x0801E9B0, 824 bytes, THUMB, parked.

Best score so far: 27.8%, -12 bytes.

## What it does

Copies a sprite's list of OAM entries into the OAM buffer, combining each with the template a5, moving it to (a2, a3) and applying flips and the owning object's scale. Each entry is passed to its object's callback; returns 1 without writing if the entries would not fit.

## How close it is

Compiles 12 bytes too short (812 against 824). The stack layout and nearly every statement are right; the difference is that the compiler merges the ends of the two scaling branches (full offset and half offset) into shared code, where the original keeps them separate.

## What is left

Make the two scaling branches come out with different registers, as in the original (its half-offset branch needs an extra register for the rounding), so the compiler stops merging their final x and y updates. The permuter has not been tried on this function.

## Already tried

- Shared `dx`/`dy` locals for both scaling branches: the branches merge almost completely, 72 bytes short.
- Separate locals per branch (`dx`/`dy` and `ex`/`ey`): 20 bytes short; the branches split, but their last updates still merge.
- Binding the width table to a local pointer in only one branch (the current draft): 8 bytes better, still 12 short.
- Writing each branch's final updates as different inline table expressions: identical code to the current draft.
- Other declaration orders for the locals: the stack slots follow declaration order, and only the current order gives the original's layout.

## Files

- `sub_0801E9B0.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 78 at 812/824 (-12), 27.8%. Inlining distinct expressions does not recover the missing code. Call/arithmetic semantics are retained; residual is expression lifetime and allocation.

### Wave 95

Base: sub_0801E9B0.w95-start not needed; draft unchanged (27.8%, size -12). Confirmed by diff that the allocation differs from the prologue (ROM: a4 in r9, a1 in ip, a2 spilled at [sp,#0]; draft: a2 in ip, a1 spilled). Not hand-probed and permuter not run this wave (ran out of time). Next step: chained permuter from this draft; the branch-merge hypothesis (different final update registers in the two scaling arms) is untested.

</details>
