# sub_08055768

0x08055768, 472 bytes, THUMB, parked.

Best score so far: 98.3%.

## What it does

Fills one side's row of gUnknown_020296BC with up to `count` slot numbers, scanning that side's five slots in a fixed order and taking the live ones. It then fills a matching row of values from a ROM table.

## How close it is

Compiles 8 bytes too long (480 against 472), 46.0% of bytes identical; the first loop's body now matches. The third loop's table is its own symbol, gUnknown_08551D26. What is left: the second loop's two store tails, which the original merges only in their last three instructions.

## What is left

In the second scan loop the original keeps the two branches' endings separate, while in the draft the compiler merges their identical tails into one and saves two instructions. The original keeps side * 5 on the stack and recomputes side * 40 inside the loop, which makes the tails differ; find a spelling that does the same.

## Already tried

- Moving the empty `gUnknown_08551E64[0][0] += 0` statement elsewhere in the loop, or writing it through x, out or side: identical output.
- Wrapping the conditional in `do { } while (0)`, or giving `x` its own block: worse register choices.
- Reading `x` inline in the `if` instead of through a local: only changes which address is moved out of the first loop, and disturbs the second loop; no improvement.
- Other attempts to shorten variable lifetimes: no improvement; the draft was restored.

## Files

- `sub_08055768.c`: the current draft
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

### Wave 96

Base: w95 draft (`sub_08055768.w96-start.c`, 31.8%, -4). Now 46.0%, size+8, first diff still +0x1c.
- FOUND: the third loop's second table is NOT `gUnknown_08551D22[..][2]`: the ROM pool word is the bare symbol `gUnknown_08551D26` (0x08551D22 + 4, its own linker symbol, like D2A). Spelled `gUnknown_08551D26[gUnknown_030045A0[side]][0]`; I added `extern const u16 gUnknown_08551D26[][5];` next to D2A in include/unknown-globals.h. This removes the `.word 4` addend and one pool difference.
- FOUND: the -4 was the first loop. Spelling the value as `*(u16 *)((u8 *)gUnknown_08552148 + side * 2)` (also `*(gUnknown_08552148 + side)` and `(&gUnknown_08552148[side])[0]`, all byte-identical) makes the compiler hoist the whole `&gUnknown_08552148[side]` into ip like the ROM; the indexed spelling hoists only the symbol and leaves `lsls/adds` in the loop. Loop 1 body is then instruction-identical to the ROM. A `u16 *vp = &...` bind before the loop also hoists it but lands +8 and moves its computation above the loop guard.
- Remaining in loop 1: preheader order. ROM: side*40 (sb), &gUnknown_020296BC (sl), side*0xb4 (r8), value address (ip); here the 0xb4 multiply comes before the 020296BC load (r8/sl swapped roles).
- The `gUnknown_08551E64[0][0] += 0;` in the SECOND while loop is what keeps the two tails apart: with it 46.0% (+8, an extra `gUnknown_08551E64` pool word in loop 2); without it, jump2 merges the two tails completely (`b .L26`) and the function is 8 bytes short (30.5%). The ROM sits between: it merges only the tail from the `adds r0,r0,r1; add r0,r8; strh` and keeps `lsls r1,r1,#3` (r1 = the [sp] slot loaded at the loop top, so side*5 spilled) in branch A but `ldr r7,[sp]; lsls r1,r7,#3` in branch B. So the ROM computes side*40 from a spilled side*5 INSIDE each branch. `u16`/`int s5 = side * 5` with `gUnknown_020296BC[0][s5 * 4 + out]` in loop 2 (block-scoped or function-scoped): 22-30%, no closer (function-scoped moves the first diff to +0x16). The first-loop `+= 0` is not needed (loop 1 tails are already distinct).
Proposed summary status: loop 1 solved (whole-address spelling); left = loop 2 tail-merge shape (ROM shares only the last three instructions of the two store tails, side*40 recomputed from a spilled side*5) and the preheader order of loop 1.

</details>
