# sub_0801E508 — parked at 15.7%, size -44 (932 vs 976), wave 54, W54-G

**The decomposition is settled and the whole `bl` sequence is structurally
right.** The score is positional noise from the -44. One `try_match` spent.

## What the function is

The affine sibling of `sub_0801E9B0`: same subsystem, same `a1 + n > 0x80`
guard, same `gUnknown_03002520[a1 * 4]` destination walked 8 bytes per record,
same "count halfword then N records" input. It rotates each OAM cell by
`gUnknown_0200F720[a5]`, which is reached as `s16 *e` — the idiom the promoted
sibling `src/decomp/c_0801E18C.c:32` already uses on the same record:

    e[0] = x scale   e[1] = y scale   e[2] = angle
    x = (int)( z*cos(e2)/e0 + v*sin(e2)/e0 + a2 - f(u) )
    y = (int)(-z*sin(e2)/e1 + v*cos(e2)/e1 + a3 - f(w) )

`sub_0808B91C` is cosf to `sub_0808B710`'s sinf — they are always called as a
matched pair on the same angle, which is what identifies B91C.
`(w & 0x300) == 0x300` selects the full-size arm; the `else` arm is the same
code with every `sub_0801E3B4` result halved (`lsrs #31; adds; asrs #1`).

## Two readouts that are already confirmed and cost nothing to reuse

- **`sub_0801E3B4` is pre-expanded to the head of its statement.** Written
  inline as `- (float)sub_0801E3B4(u)`, gcc 2.x's `preexpand_calls` hoists the
  call above the whole float chain and leaves the `/ 2` (in the else arm) at the
  point of use. That reproduces the ROM exactly, and it does NOT need a separate
  `k` local. Confirmed in the candidate's output.
- **`(s16)member` on a `u16` struct member compiles to `ldrsh`** — probed
  directly, see the new chapter in `docs/agbcc-codegen.md`. So
  `gUnknown_0200F720` did NOT need retyping, and the plain (uncast) `u16` read
  is refutable: it emits `ldrsh` PLUS a `cmp; bge; __addsf3 65536.0`
  unsigned-correction the ROM does not have.

## The remaining residual: too many simultaneously live values

The ROM keeps `e` in r9, `w` in r8, `u` in r7 and `v` in sl, and its frame is
36 bytes. The candidate spills `e` and `w` to stack and gets a 32-byte frame —
one fewer slot but two more spills, so the allocation diverges from `+0xa`.

Two concrete symptoms, both visible in the diff:

1. **`sub_0808B710(...)` / `sub_0808B91C(...)` are shared between the x and y
   statements in the candidate and RE-CALLED in the ROM.** The x statement needs
   cos-then-sin and the y statement sin-then-cos on the same angle, so the two
   statements offer each other identical subexpressions. Whatever keeps them
   apart in the ROM also frees the two registers the candidate is holding them
   in — this is almost certainly the whole -44.
2. `n` is spilled eagerly by the candidate; the ROM keeps it in r2 until the
   loop preheader (`str r2,[sp,#0xc]` after the `e` setup). Likewise the ROM
   writes `p` as a copy-then-add (`adds r0,r3,#0; adds r0,#2`), keeping `a4`
   live, where the candidate does a destructive `adds r3,#2`.

## Next things to try, in order

- Attack (1) first: it is the wave-35 W35-C family (two arms/statements the ROM
  keeps separate). Try writing the four angle terms so the two statements are
  not textually identical, or binding the sin/cos results to per-statement
  locals so the live ranges cannot merge.
- If (1) alone does not close it, (2) says the source probably does not reuse
  one variable for both the bound and the counter — try `for (i = n; i != 0;
  i--)` with a distinct `i` rather than `while (--n != 0)`.
- Above 256 bytes the brief mandates a permuter run before a third hand
  rewrite; it has not been run.

## Wave 65 final probe

A grouped-accumulator rewrite forced each soft-float term into its own statement,
precomputed the two `sub_0801E3B4` results, preserved `a4` with `p = a4; p++;`,
and used a distinct loop counter. It moved the wrong way: 900/976 bytes (-76)
with a 28-byte frame, versus this draft's 932/976 (-44) and 32-byte frame; the
ROM uses 36. The rewrite did not merely fail to break the unwanted reuse: it
shortened the live ranges further. The honest expression draft remains active.

## Header work done for this function, all swept for callers

`sub_0801E3B4`, `sub_0808B710` (promoted-but-undeclared) and `sub_0808B91C`
(draft only) declared in `include/unknown-functions.h`. `sub_0801E508` is the
only caller of any of them outside their own files.

## wave 96

Base: sub_0801E508.c (22.64%, size-28; the wave 71 draft). Result: 56.9%, size-EXACT (976), first difference +0x12. NOT matched; the instruction-level diff is still large (about 80 differing lines vs 104 before).

What moved it (permuter, 900 s, then cleaned by hand): a "mixed bind". `s16 angle = e[2];` assigned once per record before the `(w & 0x300) == 0x300` test, and used for exactly THREE of the four `(float)e[2]` arguments in the full-size arm (fx with cosine, fy with sine, fx of the y part with sine); the y part's cosine and all four reads in the half-size arm keep `e[2]`. Binding all of them, or all in the full-size arm, or all in the half-size arm, is far worse (14-30%, size -8 to -40): the size gain is the subset. This is the wave 95 "mixed bind" lever (item 2 in the brief) applied to a struct member read that is re-read across calls; it works on a subset here, and the failures match the brief's warning for re-read pointers.
The size is exact but the ROM still re-loads e[2] from memory with `movs r1,#4; ldrsh r0,[r3,r1]` at every use (r3 = r9 copy of e), where this draft sign-extends `angle` in registers (`lsls; asrs`). So the bind is an approximation: it repairs the size, not the mechanism. The ROM has no local for e[2]; something else makes the first arm's reads differ from the second's.

Not resolved: e is in r6 here, r9 in the ROM (the ROM keeps w in r8, v in sl, u in r7, e in r9, n in the stack); the high-to-low `mov`/`ldrsh` copy pairs before each sinf/cosf call are the +11 copy delta. The pre-registered "several source temps" hypothesis is therefore PARTLY right (the bind is a source temp) but the ROM's extra copies are the `mov rLo,rHi` before each call, which a temp for the ANGLE does not create; the hypothesis' mechanism did not hold.
A second permuter chain (900 s) from the 56.9% file found nothing.

Proposed summary: does = rotates and scales a list of OAM pieces by an affine entry, using soft-float; status = size exact, 56.9% identical; left = e/w/v/u register assignment and the per-call reload of e[2]; tried = temps for the accumulators (wave 65), distinct loop counter, angle temp at several subsets, permuter.

## wave 97 (W97-R)
Tried "respell one of identical expressions" per call site (full-size arm): each of the four `(float)e[2]`/`angle` sites as `e[2]`, `angle` or `*(e + 2)`, all 81 combinations. Best remains the wave 96 file (56.86%); `*(e + 2)` is the same tree as `e[2]` (byte-identical to it), the other combinations are 26-54%. No new lever. The ROM's `movs r1,#4; ldrsh r0,[r3,r1]` is just how Thumb loads a signed halfword; the real difference is that e lives in a high register (r9) in the ROM, so each call site copies it down. Not pursued further.
