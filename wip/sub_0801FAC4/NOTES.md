# sub_0801FAC4 — wave 93 (W93-B)

## The 65.93% `best.c` was WRONG C and has been quarantined

The wave-start scan named `best.c` as a better base: 65.93%, size-exact 540,
against the draft's 36.30% at 536 (-4). It was read statement by statement and
it is not equivalent to the draft. Two independent defects, both in the
permuter's mutations:

**Case 2 reads `e` before it is set.** The draft has

    e = s + w;
    if (e > bound) e = bound;
    for (y = s; y < e; y++) ...

`best.c` deleted the first statement (leaving a bare `;`) and rewrote the test
as `if ((s + w) > bound) { e = bound; }`. So on any iteration where the clip
does **not** fire, `e` is never assigned: it is read uninitialised on the first
iteration and holds the previous iteration's value thereafter. The inner loop's
bound is garbage. This is the same defect wave 92 rejected on three functions.

**Case 3 clobbers its own loop bound.** The inner loop became

    for (y = s; y < e; y++) { e = x; gUnknown_03003340[y][e] = a5; }

`e` is the loop's exit bound, so assigning `e = x` inside the body changes the
termination condition from `y < clipped(s + w)` to `y < x`.

The file is now `best.c.wrongc` (reference only, and invisible to
`drafts.py bases`, which globs `*.c`). Its stale `best.json` was removed and
re-measured from the draft. **Do not let this score be quoted as a base again.**

Where its extra 4 bytes came from is worth keeping: `e = x` inside case 3's
inner loop is a register copy per iteration, which is what took 536 to 540. The
size-exactness was bought by nonsense code, not by finding the residual — the
brief's "a score that rose because the SIZE changed is not progress" case,
caught in the act.

## What this means for the function's history

The parked entry records "Permuter, about 25,500 attempts: no improvement", and
wave 65 recorded that a scoped-bound experiment "was invalidated by historical
permuter-mutated best.c". Since `best.c` has been wrong C for some time, any
permuter run that chained from it, and any comparison measured against it, was
anchored on code that computes the wrong thing. The 2026-08-29 tools pass and
the 2026-09-26 review both found the permuter had been running with a
mis-weighted scorer. Treat the 25,500-attempt negative as **not applicable to
the draft**: this wave's run is the first from the honest source.

## Residual (re-measured this wave, from the draft)

536 of 540 bytes (-4), 36.3% identical, first difference at +0xa. The top of the
function is `sub sp, #8` in the ROM against `sub sp, #4` here — the two-slot
versus one-slot fact the parked entry describes. Those two instructions are the
same size, so the missing 4 bytes are two register copies further in, not the
frame set-up itself.

## NEW BASE, found this wave: `volatile int bound` in case 0 — 45.56%, SIZE-EXACT

The two-stack-slot residual is closed. The lever is the bound local wave 71
already tried, with one qualifier added:

    case 0:
        bound = a2 + a4;                       /* volatile int bound; */
        for (y = a2; y < bound && y < *(u16 *)(gUnknown_08499590 + 2); k++, y++)

    wave 71, `int bound`   ... one slot,  536/540 (-4), 36.30%, first diff +0xa
    wave 93, volatile      ... two slots, 540/540 EXACT, 45.56%, first diff +0x24

`a2` and `a4` are parameters that the loop never modifies, so `bound` is set
before it is read and holds the same value on every iteration; `volatile` only
forces the reload. Equivalent C.

This is why wave 71's negative was not the end of the axis: an ordinary local
cannot create the second slot, because `local_alloc` coalesces it into a register
no matter where it is scoped. A `volatile` local is a MEM by definition and
always gets a slot. Generalised into `docs/agbcc-codegen.md` as the frame-slot
chapter; `sub_080726E8` is the same lever measured in the other direction.

Variants measured with it: `volatile bound` in cases 0, 2 and 3 is +8 at 29.38%,
and in all four cases +12 at 12.68%. **Case 0 alone is the answer** — which also
tells you the ROM's other three arms do NOT spend a second slot.

Draft backed up as `sub_0801FAC4.w93-start.c`, new base kept as `w93-base-4556.c`.

## Residual after the new base

540/540, 294 of 540 bytes differ, first difference at +0x24 — and it is now a
pure allocation residual, a swap of two high registers in the prologue:

    ROM        lsls/lsrs #16 (a4) -> mov r8, r3      lsls/lsrs #24 (a5) -> mov ip, r4
    candidate  lsls/lsrs #16 (a4) -> mov ip, r3      lsls/lsrs #24 (a5) -> mov r8, r4

Both promoted parameters land, both in high registers, with r8 and ip exchanged;
everything downstream follows from that one swap. Size-exact with the instruction
multiset right and two registers transposed is exactly decomp-permuter's case
(the wave-59 repeated-run recipe), and it is where this function should be
attacked next.

## Permuter from the new 45.56% base: 61.85% reported, REJECTED as wrong C

900 s x 4 threads. `permute.py` reported `IMPROVED 45.56% -> 61.85%`. Audited
against the base with comments and formatting normalised away, and it has two
independent defects:

**`s = a1 + 1;` was hoisted out of case 0's loop into unreachable code.** It now
sits between case 1's `break;` and the `case 0:` label, where nothing can reach
it, and case 0's loop body starts at `s -= k;`. So `s` is read uninitialised on
case 0's first iteration, and on later iterations it accumulates `-= k` instead
of being reset to `a1 + 1` each time. The statement was never loop-invariant.

**`new_var` is assigned in case 2 and dereferenced in case 3.** The permuter
bound `(u16 *)(gUnknown_08499590 + 2)` to a local inside case 2's loop body and
then used `*new_var` for case 3's clip as well. Cases 2 and 3 are mutually
exclusive switch arms, so with `a3 == 3` the pointer is never assigned and the
clip dereferences it.

Discarded. Output kept as `w93-perm1-6185.c.wrongc` and the permuter's `best.c`
as `best.c.wrongc2`; `best.json` regenerated. The 45.56% base is restored and
re-measured: **45.56%, size-exact, first difference +0x24.**

## wave 96

Base: sub_0801FAC4.c (volatile `bound`, 45.56%, size-exact). Kept as the final source; nothing beat it on score.

Measured with a structural (register-blind) instruction diff against the ROM, because the score is dominated by the shifted bytes:

* Removing the volatile `bound` and writing `y < a2 + a4` inline in case 0 (`sub_0801FAC4.w96-g1-novolatile.c` is the best of these) gives 258 vs 259 instructions, a ONE-slot frame (`sub sp, #4`; ROM #8), 36.3%, size-4. It is structurally closer (11 differing instruction lines against 17 for the volatile draft) and, unlike the volatile draft, it puts the high registers where the ROM has them (a4 in r8, a5 in ip; the volatile draft swaps those two). So the ROM's a4/a5 hi-register order is NOT the volatile's doing; the volatile is only buying the frame slot.
* ROM case 0 recomputes `a2 + a4` in the loop head (`mov r6,r9; add r6,r8; str r6,[sp,#4]`) and reloads it at the bottom test, so the second slot is a spilled loop-invariant hoisted by the loop pass, not a source variable. The entry test uses registers directly. A `volatile` local reloads immediately after the store, which is not the ROM's shape.
* ROM case 0 keeps the address of gUnknown_08499590 in one register for the whole loop (`ldr r6,=G` for the entry test, `adds r7,r6,#0`, then `[r7]` in the body). Binding `u8 **gp = &gUnknown_08499590` (function scope, or at the top of the loop body) does not reproduce that: agbcc reloads the pool word instead (two `ldr rX,=G` in the loop). Negative, mechanism: a bound constant address is rematerialised from the pool, it is not kept in a register.
* The pre-registered copy hypothesis does not apply here (delta -1: the draft has one MORE copy than the ROM).

Proposed summary: does = fills a diamond, square or line of tiles by looping outward from a centre; status = size-exact but register assignment differs in the hi registers; left = the ROM spills the case 0 bound to a second slot and keeps the global's address in a register; tried = volatile bound, inline bound, address bind, all measured.
Permuter (600 s, --current, volatile draft): 'improved' 45.56% -> 49.07%, WRONG C: in case 0 it inserts `s = y;` between the clip and `for (x = s; ...)`, which overwrites the row's first column with the row index, and indexes the table with `[s]`. Kept as sub_0801FAC4.w96-perm1-WRONG.c; draft restored (45.56%, size-exact).

## wave 97 (W97-Y)
levers.py's best (58.7%) is WRONG (a byte written 0x01 vs unwritten; `w = s < 0` changes the value); ignored.
Base: `sub_0801FAC4.w96-g1-novolatile.c` (no volatile, 36.3%, 536 bytes / -4, one-slot frame); draft kept as `sub_0801FAC4.w97y-start.c`. Three chained permuter links (wrongc OK each, diffs read; every change is value-neutral): `x = k; s -= x;` in case 2 (536 -> 540, size-exact WITHOUT any volatile, 56.11%), `(e - 1) >= *(u16 *)(gUnknown_08499590 + 2)` for `e > ...` in case 4's clip (57.96%), and a `u16 *new_var` bound to that same address at that site (58.33%).
Now 58.33%, size 540 exact, no volatile, first diff +0xa. Residual not yet re-read beyond wave 96's (ROM keeps the global's address in one register across case 0's loop; frame is one slot vs the ROM's two).
Proposed status: size-exact and volatile-free at 58.3%; left = second spill slot and the case-0 address register.
