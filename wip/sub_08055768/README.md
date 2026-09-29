# sub_08055768

0x08055768, 472 bytes, THUMB, parked.

Best score so far: 31.8%, -4 bytes (best.c).

## What it does

Fills one side's row of gUnknown_020296BC with up to `count` slot numbers, scanning that side's five slots in a fixed order and taking the live ones. It then fills a matching row of values from a ROM table.

## How close it is

Compiles 4 bytes too short (468 against 472): the second scan loop is exactly two instructions short, and the first scan loop and the final loop already match. 318 of 472 bytes differ (31.8% identical), which means little because the size difference starts early and shifts every later byte.

## What is left

In the second scan loop the original keeps the two branches' endings separate, while in the draft the compiler merges their identical tails into one and saves two instructions. The original keeps side * 5 on the stack and recomputes side * 40 inside the loop, which makes the tails differ; find a spelling that does the same.

## Already tried

- Moving the empty `gUnknown_08551E64[0][0] += 0` statement elsewhere in the loop, or writing it through x, out or side: identical output.
- Wrapping the conditional in `do { } while (0)`, or giving `x` its own block: worse register choices.
- Reading `x` inline in the `if` instead of through a local: only changes which address is moved out of the first loop, and disturbs the second loop; no improvement.
- Other attempts to shorten variable lifetimes: no improvement; the draft was restored.

## Files

- `sub_08055768.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at 468/472 (-4). Wave 63's opaque-store LICM barrier makes loop 1 exact; the whole residual is loop 2 cross-jumping two arm tails. Moving the barrier, addressing it through x/out/side, do-while wrapping and block-scoping x are neutral or regress allocation. Wave 70 restored and reverified the strongest draft after new lifetime probes failed. Preserve work/sub_08055768/sub_08055768.c and its evidence comment.

### Wave 95

Base: existing draft (468/472, -4, 31.8%).
- Pre-registered hypothesis (local `s5 = side * 5` used in loop 2, side*40 recomputed from it as `s5 * 8` inside the loop via `gUnknown_020296BC[0][s5 * 8 + out]`, so the tails differ): 25.6%, still 468 (-4). Refuted as written: the size is unchanged and the pool/register order moved further from the ROM (mov r9/r8/sl rotation in loop 3). The recomputed side*40 does not change whether jump2 cross-jumps the two tails; the tails end at the shared `strh` regardless of how the index was prepared.
- Not permuter-run (budget went to the three permuter-friendly functions).

</details>
