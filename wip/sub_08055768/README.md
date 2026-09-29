# sub_08055768

0x08055768, 472 bytes, THUMB, parked.

Best score so far: 98.3%.

## What it does

Fills one side's row of gUnknown_020296BC with up to `count` slot numbers, scanning that side's five slots in a fixed order and taking the live ones. It then fills a matching row of values from a ROM table.

## How close it is

Compiles to the right size (472 bytes) with 98.3% of bytes identical, up from 8 bytes too long. What is left: the order of the first loop's setup and one commuted add. Two kept spellings are odd but value-neutral: `base - (-(side*2))` and `(0, gUnknown_08551D26)`.

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

### Wave 97

wave 97 (W97-Y)
Base: w96 draft (46.0%, +8). levers.py `5d-0+5a-63` (do { } while (0) around the whole body, wrongc OK; the `loop-const` on the `1cp` variant was the known 5d false alarm, the emulator agrees on 218 seeds) -> 86.86% size-exact. Then three chained permuter links (each wrongc OK): moved the `gUnknown_08551E64[0][0] += 0` barrier below `i++` in loop 2 (93.2%), dropped the do-while again and respelled the loop-1 value address as `base - (-(side * 2))` (97.0%), and `(0, gUnknown_08551D26)[..][0]` in loop 3 (98.31%). Both odd spellings are value-neutral; each is worth ~1% when removed (a.c/b.c tests: 96.2% / 97.0%).
Now 98.31%, size 472 exact, first diff +0x42. Residual (8 bytes): loop 1 preheader order only. ROM: `lsls r0,r7,#3; mov r9,r0` (side*40) BEFORE `ldr r1,=pool; mov sl,r1`, and the value address as `adds r0,r0,r1` (side*2 + sym) into ip; here the pool load / mov sl comes first and the add is commuted (`adds r1,r1,r0`). Not matched.
Proposed status: size-exact, 98.3% identical; left = order of two hoisted preheader computations in loop 1.

</details>
