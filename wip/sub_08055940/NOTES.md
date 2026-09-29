# sub_08055940 — 248 bytes, 93.5% by hand / 96.4% after the permuter, NOT matched

Wave 51, W51-H. `trymatch` exit code 1. **Size is exact (248/248) and the
instruction MULTISET is exact.** Every differing byte is a register field or the
order of four preheader instructions. Nothing about the semantics is open.

## What the function is

```c
void sub_08055940(u16 a1, u16 a2)
{
    u16 counts[2];
    u16 i;

    counts[0] = 0;
    counts[1] = 0;

    for (i = 0; i < 5; i++)
    {
        if (gUnknown_020296BC[0][i] != 0xff) counts[0]++;
        if (gUnknown_020296BC[1][i] != 0xff) counts[1]++;
    }

    if (a1 == 2 && gUnknown_0300450C == 0) counts[1] = 0;
    if (a2 == 2 && gUnknown_0300450C == 1) counts[0] = 0;
    if (a1 == 2 && gUnknown_0300450C != 0) counts[0] = 0;
    if (a2 == 2 && gUnknown_0300450C != 1) counts[1] = 0;

    if (gUnknown_03004580[0][1] == 9 || gUnknown_03004580[0][1] == 0x14) counts[0] = 0;
    if (gUnknown_03004580[1][1] == 9 || gUnknown_03004580[1][1] == 0x14) counts[1] = 0;

    sub_08055A38(counts[0], 1);
    sub_08055A38(counts[1], 0);
}
```

That is `work/sub_08055940/sub_08055940.c` as it stands. Everything in it is
confirmed against the ROM instruction for instruction: the four `== 2` guards,
their **bound and sense** (guards 3 and 4 are `!= 0` / `!= 1`, not `== 1` /
`== 0` — the twin sub_08055654 spells the same four the other way and it is a
real source difference, not a normalisation artefact), the two `9 || 0x14`
tests, the argument order of `sub_08055A38(count, side)`, and the fact that
`side` is 1 for `counts[0]` and 0 for `counts[1]`.

`gUnknown_08136158` is **not a global** — it is a `-fforce-addr` `.rodata` pool
word holding 0x03004580. The honest spelling (`gUnknown_03004580[0][1]` /
`[1][1]`) reproduces it exactly; `trymatch` prints `R_ARM_ABS32 .rodata` against
the ROM's `gUnknown_08136158` and that half is correct and needs no work.

## The entire residual, and it is ONE fact

The ROM's literal pool holds the **bare symbol `gUnknown_020296E4`** (addend 0)
and derives the lower row with a register subtract:

```
    ldr  r5, =gUnknown_020296E4     @ 0x020296E4
    adds r6, r5, #0
    subs r6, #0x28                  @ 0x020296BC
```

Every spelling tried puts the **bare symbol at the LOWER address** in the pool
and derives the upper row by adding:

```
    ldr  r5, =gUnknown_020296BC
    adds r7, r5, #0
    adds r7, #0x28
```

Same instruction count, opposite direction, and the r6/r7 roles swap out of it —
which is the whole 16-byte diff. The two rows are 0x28 = 40 bytes apart and both
addresses have their own `aw2bhr.lds` symbol, but the ROM's SINGLE pool word plus
the subtract proves they are **one object**, not two neighbouring globals: two
distinct symbols cannot be CSEd into each other. That is what
`extern u16 gUnknown_020296BC[][20];` in `include/unknown-globals.h` records.

## Axes ruled out, with what each produced

- **`gUnknown_020296BC[0][i]` / `[1][i]` (the shape above)** — 93.5%. Pool holds
  the bare lower symbol, `adds #0x28`.
- **Flat array, `gUnknown_020296E4[i - 20]` / `[i]`** — much worse. gcc
  reassociates the −20 into the *index*: `add r0,r1,#0; sub r0,#0x14; lsl r0,#1;
  add r0,r0,r4`, four instructions inside the loop instead of a hoisted base.
- **2-D negative row, `gUnknown_020296E4[-1][i]` / `[0][i]`** — the pool word
  becomes the bare `gUnknown_020296E4`, **which is the right symbol**, but gcc
  again folds the −0x28 into the varying part (`lsl r1,r2,#1; add r0,r1,#0;
  sub r0,#0x28; add r0,r0,r5`) instead of hoisting `sym - 0x28` as its own
  invariant. So the negative row index gets you the right pool word and the
  wrong association.
- **Two pointer locals assigned before the loop** (`pB = gUnknown_020296BC[1];
  pA = gUnknown_020296BC[0];`) — this is the closest near-miss and worth
  re-reading. It produces `.word gUnknown_020296BC+0x28` (the RIGHT address,
  reloc naming a different symbol that resolves to it, which is the accepted
  false-mismatch class) **and** `sub r7, r7, #0x28` — the ROM's arithmetic,
  exactly. It fails only on PLACEMENT: the two `ldr`s land before the `for`-init
  because they are pre-loop source statements, whereas the ROM's whole preheader
  is LICM output sitting *after* `movs r2, #0`. Instruction multiset identical,
  order wrong.
- **decomp-permuter, 300 s / 4 threads** — reached 96.4% (kept in `best.c`,
  header-expanded) but no match. It moves registers, not the CSE
  canonicalisation, so it cannot reach this.

## What the next agent should try

The ROM's preheader order is `[&counts[1] copy, .rodata pool addr, &counts[0],
gUnknown_020296E4, &counts[1] copy 2, gUnknown_020296E4 - 0x28]`. The row-0 base
is emitted **last** because it *depends* on the row-1 base — that dependency is
the tell, and it is only produced when CSE canonicalises on the UPPER address.
So the source must name the object at 0x020296E4 and reach 0x020296BC as a
subtract, while keeping both inside the loop body so LICM does the hoisting.
The pointer-local experiment shows the arithmetic is reachable; what is missing
is a spelling that keeps it inside the loop. Try binding the row-1 base to a
`u16 *` **whose initialiser is itself loop-invariant but written inside the loop
body**, or a `u16 (*)[20]` cursor.

Nothing here is a type, layout or control-flow question. Do not re-derive the
body.

## Wave 58, W58-D — the arithmetic is now REACHABLE; only the ORDER is left

The "what the next agent should try" above was right and it works. Binding the
row-1 base to a `u16 *` **inside the loop body**, row 1 FIRST, gets the ROM's
canonicalisation:

```c
    for (i = 0; i < 5; i++)
    {
        u16 *p1 = gUnknown_020296BC[1];
        u16 *p0 = gUnknown_020296BC[0];

        if (p0[i] != 0xff)
            counts[0]++;
        if (p1[i] != 0xff)
            counts[1]++;
    }
```

93.1%, size exact, and the pool word is now `gUnknown_020296BC + 0x28` (the
ROM's `gUnknown_020296E4`, the accepted same-address reloc class) with
`subs r7, #40` — **the ROM's direction, with the bases still hoisted by LICM
rather than by a pre-loop statement.** Which symbol wins the pool word is
decided purely by which is referenced FIRST in the loop body; nothing else moves
it.

**What is left is four instructions of preheader ORDER, and only that:**

```
ROM        movs r2,#0 | adds r7,r0,#0 | ldr/mov sb | mov r3,sp | ldr r5,=E4 | adds r4,r7,#0 | adds r6,r5,#0 ; subs r6,#0x28
candidate  movs r2,#0 | adds r6,r0,#0 | ldr/mov sb | ldr r5,=E4 | adds r7,r5,#0 ; subs r7,#40 | mov r3,sp | adds r4,r6,#0
```

The ROM's order is `[&counts[0], E4, &counts[1], BC]` — i.e. **BC is hoisted
LAST, after the two count addresses**, which is `move_movables` deferring it to
a second pass because it depends on E4's pseudo. The candidate hoists both bases
first, in body order, because both are defined in the first statement of the
body. Reproducing the ROM needs BC's move insn to sit at the FIRST `if` while
naming a pseudo defined at the SECOND, which no C statement order produces.

Also measured this wave, both null:

- **The permuter's `do { } while (0)` around the second `if` is NOT additive.**
  It is what got `best.c` to 96.4% on the ORIGINAL body, but applied on top of
  the pointer-bound body above it is byte-identical to it (17 of 248, same
  diff). Do not stack the two.
**THE SHIPPED DRAFT IS NOW THE 96.4% BODY** — the plain statements with a
`do { } while (0)` around the SECOND `if`, in clean C. That is the permuter's
wave-51 find reproduced by hand; it had only ever existed as header-expanded
`best.c`, which is not a usable draft. 9 of 248 bytes differ and **the whole
rest of the function is byte-exact** — the r6/r7 role swap that used to run
through the tail is gone, and both bases now sit in the ROM's own registers
(r6 = the row-0 base used first, r5 = row 1):

```
ROM        mov r3,sp | ldr r5,=E4      | adds r4,r7,#0 | adds r6,r5,#0 ; subs r6,#40
candidate  ldr r6,=BC | mov r3,sp      | adds r5,r6,#0 ; adds r5,#40   | adds r4,r7,#0
```

So the residual is now ONLY the pool direction plus where the `ldr` sits.

- The pointer-bound body (93.1%) and the `do { } while (0)` body (96.4%) each
  fix ONE half and **cannot be combined** — stacking them measures 93.1%,
  because the in-body pointer declarations are what move the hoists ahead of
  `mov r3,sp`. Getting the E4-first reference without a pre-bound pointer is
  the whole remaining question.
- Score note: the pointer-bound body reads LOWER than the plain one while being
  strictly closer in mechanism (right symbol, right subtract). Positional score,
  again; paste the block above back in if you work this.

**Classification: basic-block / instruction ORDER across a loop boundary** —
the same unreachable family as `sub_080373F0`'s block layout and W58-A's LICM
hoist. Instruction multiset is exact.

## Wave 92 (W92-C) — the residual restated

Still 248/248 with 9 bytes differing, first difference at +0x28. Nothing in
the loop body differs; the whole residual is the order of the five values the
compiler lifts out of the loop.

Original, in order: address of `counts[0]` (which is `sp`), the address of
row 1 loaded from the literal pool, a copy of the address of `counts[1]`, and
finally row 0, worked out as row 1 minus 40.

Draft, in order: row 0 loaded from the pool, address of `counts[0]`, row 1
worked out as row 0 plus 40, address of `counts[1]`.

Read as a list, the original's order is the draft's order **rotated by one**:
the draft leads with row 0 and the original puts row 0 last. That also settles
which row comes from the pool, because the compiler always derives whichever
row address it lifted *second* from the one it lifted first. So there is one
requirement, not two: the loop body has to mention `counts[0]` before it
mentions row 1, and row 0 after `counts[1]`, while still reading row 0 before
`counts[0]` in the emitted code. Every source order that produces the right
lift order also reorders the body, which is currently byte-exact.

No new spelling was measured this wave; the axes in `data/parked.json` cover
row spellings, pointer locals inside and outside the loop, block swaps,
opaque offsets and about 40,000 permuter attempts.

## Wave 93 (W93-F): the hoist order is reachable, and it costs a register

Wave 87 left the open problem as a rotation: the loop body refers to its four
loop-invariant values in the order [row0][counts0][row1][counts1], and the
original's preheader emits them rotated left by one,
[counts0][row][counts1][row'].

**That rotation is reachable, and it needs no pointer local.** Read counts[0]
into an ordinary local at the top of the body, do the row-1 test, and write
counts[0] back through the local in the row-0 test:

    for (i = 0; i < 5; i++)
    {
        u16 c;

        c = counts[0];
        do { if (gUnknown_020296BC[1][i] != 0xff) counts[1]++; } while (0);
        if (gUnknown_020296BC[0][i] != 0xff) counts[0] = c + 1;
    }

That is the same function -- counts[0] is not touched anywhere else in the
body -- and it moves counts[0]'s first reference ahead of any row reference.
The preheader comes out

    mov r4, sp | ldr r7, <row anchor> | mov r5, r8 | add r6, r7, #0 | add r6, #0x28

against the original's

    mov r3, sp | ldr r5, <row anchor> | adds r4, r7, #0 | adds r6, r5, #0 | subs r6, #0x28

which is the original's order, value class for value class. The old draft put
the row anchor's `ldr` first; this one does not.

**It still loses, and the reason is worth more than the order was: +12 bytes,
9.23% identical, first difference at +0x2.** The prologue gives it away. The
original saves two high registers (`mov r7, sb | mov r6, r8 | push {r6, r7}`);
this form saves three. Keeping the counts[0] value live across the row-1 test
is one more simultaneously live value, so the address constant that the
original rematerialises gets a callee-saved register instead, and the whole
function shifts.

That is the **same 12 bytes** every row-pointer-local form costs (waves 70, 77,
80, 87). Those waves read the tax as something about pointer locals inhibiting
the loop's induction variable. This measurement says it is not about pointers
at all: an ordinary `u16` value local costs exactly the same 12. **The tax is
one extra simultaneously live value, whatever it holds.** So the constraint on
this function is tighter than it looked -- any construct that reaches the
original's hoist order by keeping something alive across the body pays 12
bytes, and the original reaches that order with nothing extra alive.

What is left is the anchor direction, unchanged: the original loads
gUnknown_020296E4 (row 1) and derives row 0 with a runtime `subs #0x28`, and
every spelling that names row 1 folds to a single pool word of
gUnknown_020296E4-0x28 (wave 80). Draft restored unchanged.

## wave 96
Base: draft unchanged (96.37%, size+0, 9 bytes, first diff +0x28). Tried one new axis: swap the two row tests in the loop body (row 1 first, with and without the do/while(0) moving to row 0): 93.6% (first diff +0x21) and 95.6% (+0x28). Mechanism: row-1-first makes the row-1 constant the first LICM hoist, but the counts[0] address then also moves and the pair/anchor still comes out lower-address-first; it perturbs the counters without producing the deferred row-0 hoist. Mixed-bind/row-pointer hypothesis not run again: waves 63/70/77/80/87 measured row-pointer locals at -12 bytes (they inhibit the loop's GIV).
Proposed status: unchanged; left = which row address the pool word holds (bare row 1 plus a subtract vs bare row 0 plus an add) and the preheader order of four hoists.

## wave 97 (W97-S)
Draft unchanged (96.37%). gUnknown_020296E4 exists as its own extern (u16 [][20]). Probed spelling row 1 through it: `gUnknown_020296E4[0][i]` for row 1 with row 0 kept `gUnknown_020296BC[0][i]`: size-exact but 24.2% (two independent pool words, no run-time `subs #0x28`); `gUnknown_020296E4[-1][i]` for row 0: 236 bytes (-12), 8.5% (fold to one base). So the ROM's bare-E4-plus-subtract is neither the separate symbol nor the negative index.

## wave 97 (W97-PG)
Permuter chain: 1 link, 96.37% -> 96.37%, NO-IMPROVEMENT.
