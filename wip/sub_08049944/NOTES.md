# sub_08049944 -- analysis carried over from the draft comment

Moved out of the `.c` in wave 58 (W58-C): analysis belongs here, never in
the source file. Nothing below is edited.

Wave 43, W43-L. PARKED at 180 vs 188 bytes (+8). The 21.1% score is
POSITIONAL -- read the diff, not the number. Every branch, both calls, the
digit loop and the whole epilogue line up; the entire residual is ONE
conversion, described below.

WHAT IS SETTLED (and is already recorded in unknown-functions.h, where the
prototype was retyped from all-`int` to the widths below; sub_080499F8 was
re-run through trymatch afterwards and still matches byte-for-byte):

 - All five parameters narrow in the prologue, in argument order, before any
   other work -- so all five are declared narrow. An `int` parameter with a
   cast at its use puts the shifts at the use instead.
 - a2 and a3 are s16, not u16: both are re-widened `lsls #0x10; asrs #0x10`
   before sub_08014B0C's `int` parameters, which only a signed source
   produces. a1 is unsigned (`blo`/`bhs` against the digit ramp), a4 is u8,
   a5 is u16 and reaches sub_08014B0C's u16 fifth parameter unnarrowed.
 - The digit loop is DEAD in the compiled output -- neither i nor n is read
   after it -- and agbcc keeps it anyway. That is not a misreading; it must
   be reproduced. `n = n * 10` is a u16 multiply (`lsls #2; adds; lsls #0x11;
   lsrs #0x10`), read as the u16 truncation of a MULTIPLY per wave 16.
 - The two sub_08014B0C calls are the SAME source expression despite
   differing in the assembly: in the `a1 == 0` arm cse knows a1 == 0 and
   reuses r6 for the constant-zero sixth argument while materialising 0 into
   r3 for the fourth, and in the other arm it does the reverse. Do not try
   to spell the two call sites differently -- that was checked and the arms
   are mirror images of one cse choice, not of two source expressions.

REMAINING DIFF, all of it: the second argument of the sub_08012BC8 call.
  ROM        subs r1, r7, #5  /  lsls r1, r1, #0x10  /  lsrs r1, r1, #0x10
  candidate  lsls r1, r7, #0x10  /  ldr r2, =0xfffb0000  /  adds r1, r1, r2
             /  lsrs r1, r1, #0x10
That is one extra instruction plus a 4-byte literal-pool word, and the pool
word is what pushes the alignment and shifts every later offset. agbcc's
combine rewrites `(a2 - 5) << 16` into `(a2 << 16) - 0x50000` via
simplify_shift_const, paying a pool constant to save nothing; the ROM keeps
the naive form.

RULED OUT this wave, each by controlled compile_probe -- all FOUR produce
byte-identical reassociated output, so this is not a front-end shortening
question at all but an RTL combine one:
  1. `a2 - 5` with a2 declared s16          (the form above)
  2. `(u16)(a2 - 5)` with the cast explicit
  3. `int t; t = a2 - 5;` then pass t       -- an SImode temp does NOT help;
     combine folds straight through the intervening pseudo
  4. `u16 t; t = a2 - 5;` then pass t
  5. `(a2 - 5) & 0xffff` -- worth trying because an AND is not an ashift and
     should not be distributable, and it STILL comes out identical
  6. a2 declared u16 with explicit `(s16)` casts at the sub_08014B0C calls
     (probe w1) -- identical, so the parameter's signedness is not the axis

WHAT THE NEXT ATTEMPT SHOULD ATTACK. Combine only distributes when the
subtraction's result dies into the shift in the same basic block. Every
spelling above keeps them adjacent. The untried axis is therefore a source
shape that SEPARATES them -- giving `a2 - 5` a second reader, or computing
it on a path the masking does not share -- rather than another way of
writing the same adjacent pair. Do not spend another attempt varying the
arithmetic; six spellings of that have now been measured and they all
collapse to the same RTL.


# Wave 58 (W58-C) re-review: the draft is already correct on the W58-B axis

Checked against W58-B's reverse preheader rule. The digit loop's preheader is

    movs r1, #0xa          ; n = 10      <- init PRECEDES the counter's
    movs r2, #0            ; i = 0       <- the counter's init
    cmp r6, #0xa ; blo <after>

`n`'s init precedes the counter's, so by the rule `n` cannot be a giv and must
be a source variable -- and the draft below already spells it as one. This is a
**positive control** for the rule rather than a finding: read forwards on a
draft that is right, it says "right".

So the +8 is not an induction variable in either direction, and the wave-43
diagnosis stands unchanged: it is combine reassociating `(a2 - 5) << 16` into
`(a2 << 16) - 0x50000`.

**One thing wave 43 could not know, and it closes the question.** Wave 42
(W42-A2) and wave 43 (W43-L) both established that *a basic-block boundary is
the only thing that blocks constant reassociation*. The ROM has no basic-block
boundary between the subtraction and the narrowing -- they are adjacent inside
the same `if (a4 != 0)` arm, three consecutive instructions. So no spelling of
the source can separate them, and the axis wave 43 named as "untried" (give
`a2 - 5` a second reader, or compute it on a path the masking does not share)
is not reachable without changing what the function computes. **This is a
codegen park, not a derivation failure.**

## wave 97
Base: sub_08049944.c (== best.c, 92.22%, size-exact, first diff +0x22), unchanged.
Re-measured; the parked description holds (14 bytes: extra `adds r3,r6,#0` from the a1 split, and r3 vs r2 as the stack-constant scratch). Probes: (a) no split (plain `a1`) -> a1/a4 swap returns (a1 in r5, a4 in r6), i.e. the split is what fixes the swap; (b) same with `0` as the sixth arg of the zero arm -> identical swap. Two probes, nothing new; stopped per budget. Proposed left: "a1/a4 register pair only fixable with a split that costs one copy; ROM has none."

## wave 97 (W97-AA)
Base unchanged (92.22%). Four spellings of which name (a1 / new_var) each of the four later reads uses, via spellings.py: any
read of `new_var` at a sub_08014B0C site (either arm) or `a1` in the loop test gives 192 bytes (+12, frame gains r6), because the
split half then spans the calls; only the current form (split read in the digit loop alone) is 180. No new axis.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.
