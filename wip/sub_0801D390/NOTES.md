# sub_0801D390 — wave 93 (W93-B)

## `best.c` at 62.97% was NOT adopted, and the brief's instruction to adopt it is wrong

The wave-93 brief said "the draft is badly off, so best.c is the only sane base
if it is correct C". `best.c` **is** correct C — it is the honest `while (p !=
NULL)` loop rewritten as

    if (p == NULL) return;
    do { ... } while (p != NULL);

which is equivalent. But it is not a better base, and the repo already records
why (wave 56, in the draft's own header comment): the `if (p == NULL) return;`
guard is REDUNDANT — the loop's own test already covers it — and it compiles to
exactly the 4 bytes this function is short. The draft is 852 against 856; the
guard makes it 856. That is the brief's own "a score that rose because the SIZE
changed is not progress" case: nothing about the residual was found, the size
was padded until the tail re-synchronised and the byte count jumped.

Confirming that the first difference does not move: the whole residual is at the
loop top (+0x10) in both forms, and the guard additionally moves the first
literal pool ~40 bytes, so the guarded form is further from the ROM's layout,
not closer.

**The honest `while` draft is kept as `sub_0801D390.c`.** `best.c` is left in
place for reference but its score should not be read as a base.

## What the residual actually is

Unchanged and already well characterised. The ROM routes the 0xFFFFF000 mask
through three pseudos before the AND:

    ROM        ldrh rV,[p] / ldr rA,=0xFFFFF000 / adds r0,rA,#0 / adds rB,r0,#0 / ands rB,rV
    candidate  ldrh rV,[p] / ldr rB,=0xFFFFF000 / ands rB,rV

Two missing register copies, 4 bytes, and `sub_0801DCD4` has the identical
difference. Everything else lines up instruction for instruction.

## Why the permuter is the right tool here and why its old negative does not count

The residual is a PSEUDO COUNT, not instruction selection: the ROM's source made
the compiler create two more copies of one value. Inserting a temporary is
precisely the mutation decomp-permuter makes, and seventeen hand spellings
(operand order, `~0xFFF` / `-0x1000`, locals inside and outside the loop, `&=`
forms, chained mask locals, `static inline` identity helpers, register-pinned
locals, empty and volatile barriers) have all folded to one AND.

The recorded 600-second permuter negative predates the 2026-08-29 tools pass and
the 2026-09-26 review, both of which found the permuter had been running
undirected with a mis-weighted scorer. This wave runs it from the honest draft,
which is the first run that can reach 856 bytes by adding real copies rather
than by keeping a redundant guard.

## Residual

852/856 (-4), first difference at +0x10, everything after it is the pool shift.
The byte percentage is meaningless at the wrong size.

## The frame-slot lever does NOT apply here — checked, not guessed

Wave 93 found on two other functions that an ordinary local never creates a stack
slot and a `volatile` local always does, which makes a frame-size difference a
usable spelling lever (see the chapter in `docs/agbcc-codegen.md`). It was worth
checking here because this function is 4 bytes short and a volatile local costs
about that.

It does not apply. **Both frames are `sub sp, #24`** — the ROM allocates exactly
the same number of addressable locals as the candidate. So the two missing copies
are register-to-register, with no object behind them, and `volatile` cannot
produce them without also adding a slot the ROM has not got. No probe spent.

Re-read of the residual at +0x10 confirms the shape precisely:

    ROM        ldrh r1,[r7] / ldr r4,[pc,#44] / adds r0,r4,#0 / adds r2,r0,#0 / ands r2,r1
    candidate  ldrh r1,[r7] / ldr r2,[pc,#40] /                                 ands r2,r1

Three pseudos for the mask in the ROM (r4 -> r0 -> r2), one in the candidate. The
`ands` is destructive, so each copy implies the previous pseudo is still needed
afterwards. That is a pseudo-COUNT fact with no frame object and no instruction
selection in it, which is why seventeen respellings have all folded to one AND
and why this is the permuter's documented case rather than a source-construct
hunt.

## Permuter from the honest draft: 56.19% reported, REJECTED as wrong C

900 s x 4 threads, the first run from the honest `while` draft. `permute.py`
reported `IMPROVED 8.76% -> 56.19%` at the exact 856 bytes. Audited with comments
and formatting normalised away: the only semantic change is one mutation, and it
is a read-before-set.

The permuter bound `new_var = (struct Unk0801D390Bits *)&e->unk30;` inside the
`0x2000`-family arm, replacing that arm's
`((struct Unk0801D390Bits *)&e->unk30)->unk00_c` test — and then used
`new_var->unk00_c` for the SAME test in the `0xF000` / `0xF00` arm, where
`new_var` is never assigned. The two arms are mutually exclusive cases of one
`switch`, so on any pass that reaches `0xF000` first the dereference is of an
uninitialised pointer. That the arms sit inside a `while` loop does not save it:
the first iteration can take `0xF000` directly.

(The bind is value-correct where it IS assigned, because `e` does not change in
the loop. It is only the cross-arm reach that is wrong.)

Discarded. The output is kept as `w93-perm1-5619.c.wrongc`. `work/` is not
tracked by git and no backup of the draft had been taken before the run, so the
draft was reconstructed by reverting exactly those three edits (the declaration,
the assignment, and the two `new_var->unk00_c` uses). The reconstruction
re-measures at **8.76%, size -4, first difference +0x10** — the draft's recorded
numbers to the digit, which confirms it is faithful.

**Take the brief's instruction seriously: copy `work/<fn>/<fn>.c` before any
permuter run.** This function had no `.w93-start.c` because the draft was being
kept rather than replaced, and that is exactly when the copy gets skipped.

## `best.c` renamed so the next wave's base scan does not point at it again

`best.c` (the 62.97% `if (p == NULL) return; do { } while (p != NULL);` form) is
correct C but is a size-padding artefact, and naming it as a better base is what
sent this wave at it. It is now `best.c.redundant-guard` — still there to read,
but no longer matched by `drafts.py bases`, which globs `*.c`. Its stale
`best.json` was deleted and regenerated from the draft.
