# sub_0801A718 — wave 92 (W92-B)

Draft unchanged at **68.94%, size-exact (132/132)**. The linker symbol this
function waited five waves for now exists and is in the draft; what is left is
register allocation. Seven spellings measured this wave, all negative, in two
compile runs.

## The sentinel symbol landed and it was worth 10 points

`aw2bhr.lds` now has `. = 0x00C618; gUnknown_0200C618 = .;` and
`include/unknown-globals.h` declares `extern struct Unk0808E5C8
gUnknown_0200C618;`. The draft's tail reads `gUnknown_030020A8.unk04 =
gUnknown_0200C618.unk04;` and the function went from 59.09% to 68.94%,
size-exact. Confirmed here by re-measuring the pre-symbol spelling
(`gUnknown_0808E5D0->unk04`) side by side: 59.09%, same size. The symbol is the
right fix and the old pointer-word workaround should not come back.

## Going through the ROM pointer words is worse, not better — measured

0x0808E5C8, 0x0808E5CC and 0x0808E5D0 are three consecutive ROM words holding
0x0200C618, 0x030020A8 and 0x0200C618, and this function's own literal pool
holds 0x0808E5CC (at +0x1C) and 0x0808E5D0 (at +0x48). That looks like an
invitation to name those words and read through them, and the candidate's
`R_ARM_ABS32 .rodata` at +0x1C where the ROM has `R_ARM_ABS32 gUnknown_0808E5CC`
looks like the difference. It is not. Both were tried:

    read gUnknown_030020A8 through gUnknown_0808E5CC ......  13.97%, size +4
    that plus the sentinel through gUnknown_0808E5D0 ......  14.29%, size +8
    sentinel through gUnknown_0808E5D0 only ...............  59.09%, size-exact

Naming a ROM address word and dereferencing it makes agbcc add its own
force-addr level *on top*, one load and four bytes per word. This generalises
wave 58's result from gUnknown_0808E5D0 to gUnknown_0808E5CC: no declaration of
one of these words reaches the ROM's load count, only naming the RAM object
does. Read the other way, the three words at 0x0808E5C8/CC/D0 behave exactly
like the address-constant cells agbcc emits for a multi-block reference, which
is what the header note already says; the candidate emitting its own `.rodata`
cell there is the honest spelling, and per the standing wave-18 rule the
promotion carries it.

## The `cur = base - 1` spelling axis is folded — four spellings, one probe

The ROM derives the list's head sentinel from the register that already holds
the node array's base, copying first:

    ROM     ldr r0, [pc, #16] / adds r2, r1, r0 / adds r3, r0, #0 / subs r3, #12
    draft   ldr r2, [pc, #28] / ... / adds r3, r0, r2 / subs r2, #12

so the ROM spends a copy the draft does not, and the draft loads the base
earlier. Four spellings were compiled to try to force the copy:

    base bound to a local, node and cur both derived from it .... byte-identical
    base bound to a local, cur still from the plain global ...... byte-identical
    cur = &gUnknown_0200C624[-1] ............................... byte-identical
    cur = (struct Unk0808E5C8 *)((u8 *)gUnknown_0200C624 - 12) .. byte-identical

All four are the current draft's 68.94% to the byte. Whether the base and `cur`
share a register is decided after the source is gone; it is not reachable by
rebinding or by respelling the subtraction. Together with the two already in the
parked entry, the spelling axis here is six deep and closed.

## Residual

Size-exact, 41 of 132 bytes differ, first difference at +0x2 — and that first
difference is a register number: the ROM keeps the first parameter in r3, the
draft in r5. Everything downstream follows from that and from the base/cur
sharing above. This is a pure allocation residual on a size-exact function,
which is the permuter's documented case (wave 59's repeated-run recipe). No
permuter run has been made on this function; it is the obvious next step and was
not started only because two runs were already occupying this agent's slots.

## Pool words owned

    +0x1C -> the address cell for gUnknown_030020A8 (ROM: 0x0808E5CC)
    +0x44 -> gUnknown_0200C624
    +0x48 -> the address cell for gUnknown_0200C618 (ROM: 0x0808E5D0)

# wave 93 (W93-B)

## First permuter run on this function: 83.33% reported, REJECTED as wrong C

900 s x 4 threads from the 68.94% draft. `permute.py` reported
`IMPROVED 68.94% -> 83.33%` and kept its output. Audited before adopting, and it
does not compute what the draft computes:

    if (((s16) gUnknown_030020A8.unk00) > 0x80)
    {
      return -1;
      node = &gUnknown_0200C624[(s16) gUnknown_030020A8.unk00];   /* unreachable */
    }
    (&gUnknown_0200C624[(s16) gUnknown_030020A8.unk00])->unk00 = (u32) a1;
    node->unk08 = key;

The only assignment to `node` was moved **after a `return`**, into dead code. On
the path that actually runs, `node` is never set and is then dereferenced four
times — `node->unk08`, `node->unk04` twice and `prev->unk04 = node` /
`cur->unk04 = node`. The first store escaped notice because the permuter rewrote
it to spell the address out in full, so only that one store still lands
correctly; every later use writes through an uninitialised pointer.

This is the wave-92 rejection class (a variable read before it is set) and the
run has been discarded. The draft is restored from `sub_0801A718.w92-start.c` and
re-measured at **68.94%, size-exact, first difference +0x2**. The permuter's
output is kept as `w93-perm1-8333.c.wrongc` and its `best.c` as
`best.c.wrongc`, both reference only and invisible to `drafts.py bases`;
`best.json` was deleted and regenerated from the restored draft.

## But WHY it scored 83.33% is the most useful thing this function has produced

Uninitialised `node` is not a random register. agbcc leaves `node`'s pseudo
holding whatever the address computation for the first store
(`&gUnknown_0200C624[...]`) already put in a register — so the wrong C gets
`node` and the node-array base to SHARE a register, and sharing them is worth
14 points.

That is the same fact as this function's recorded residual read from the other
side. The notes above record that the ROM copies the base before biasing it:

    ROM     ldr r0,[pc,#16] / adds r2,r1,r0 / adds r3,r0,#0 / subs r3,#12
    draft   ldr r2,[pc,#28] / ...           / adds r3,r0,r2 / subs r2,#12

and that six spellings of `cur = base - 1` all compile byte-identically. The
permuter has now shown that the register sharing IS reachable — just not by
respelling the subtraction. **What needs to share a register is `node` and the
array base, not `cur` and the array base**, and the previous six probes were all
aimed at `cur`. That is a new and specific target for the next attempt: find a
legal spelling in which `node` and the base are one pseudo.

The obvious candidates, none yet measured: computing `node` from a base local
that `cur` is also derived from; writing the first store through `node` rather
than through a separate address expression; and giving `node` and the base the
same live range by moving `node`'s assignment to sit between the two stores.

## Acting on that diagnostic closed 10.6 points with ordinary C — 68.94% -> 79.55%

Five spellings were compiled together, all aimed at the pseudo-creation ORDER of
the node-array base rather than at respelling `cur`:

    base bound to a local AFTER the bare first reference ... 68.94%  (no change)
    `cur` and `prev` swapped, `cur` still last ............. 68.18%  (worse)
    `cur = gUnknown_0200C624 - 1;` moved BEFORE `node` ..... 76.52%
    that, plus a `base` local both are derived from ........ 79.55%  <- adopted
    `cur` moved above the `unk00 > 0x80` guard ............. 10.29%, size +4

The adopted form is plain C with no compiler-fighting in it:

    base = gUnknown_0200C624;
    cur = base - 1;
    node = &base[(s16)gUnknown_030020A8.unk00];
    node->unk00 = (u32)a1;
    node->unk08 = key;
    prev = NULL;

`cur` is pure address arithmetic with no dependency on the two stores and
nothing between reads it, so hoisting it above `node` is equivalent; `base`
holds the array's address, which is a constant. This is very plausibly how the
original was written.

## It also corrects a recorded negative, the same way wave 93 corrected two others

The notes above record "base bound to a local, node and cur both derived from
it .... byte-identical". That is true — when `node` is computed first. With
`cur` computed first the same base local is worth a further 3 points. The two
levers are not independent, and the earlier measurement is evidence about the
pair of spellings tried, not about binding the base.

The mechanism is the documented one: of two address constants used the same
number of times, the pseudo created FIRST wins the register, and the loser is
rematerialised. Creating `cur`'s base pseudo before `node`'s is what makes the
ROM's `adds r3,r0,#0` copy appear instead of the draft's in-place `subs r2,#12`.

Kept as `w93-base-7955.c`; the 68.94% draft is `sub_0801A718.w93-start.c`.

## Residual

132/132, 27 of 132 bytes differ, 79.55%, first difference still at +0x2 — the
first parameter's register. A pure allocation residual at the exact size.

## Wave 94 (W94-A) - 79.55% -> 83.33%, and run 2's 88.64% is wrong C

Run 1: 79.55 -> 83.33, size-exact, first difference still +0x2. The mutation
binds `(s16) gUnknown_030020A8.unk00` to a local (renamed `freeIndex`) and uses
it for the node index. The field is read twice in the old form with nothing
writing it in between, so this is equivalent. It is NOT the wave-93 form that
also scored 83.33% and was quarantined - that one read `node` before setting
it.

Run 2 reported 83.33 -> 88.64 and the form is WRONG C, quarantined as
`w94-perm2-8864.c.wrongc`. It inserts `node = &base[freeIndex];` BEFORE
`base = gUnknown_0200C624;` - an uninitialised read of `base`. The value is
dead, because `node` is reassigned two lines later, but the read is undefined.
**`permute.py`'s `-Wuninitialized` gate did not catch this**, presumably
because `base` is assigned later in the same block; the gate is not sufficient
for a dead uninitialised read.

The effect is not reachable legitimately. Writing that early statement as
`node = &gUnknown_0200C624[freeIndex];` - the same address, no UB - scores
70.45%, well below the 83.33% base. What the mutation buys is a `node`
reference created before `base`'s set, which no defined C produces.

The linker-symbol fix this park has been waiting on (a symbol at 0x0200C618)
was not attempted: agents in this wave are forbidden to edit `aw2bhr.lds`.

### Residual

132/132, 83.33%, first difference +0x2 - still the first parameter's register
(ROM r3, candidate r5). A pure allocation residual at the exact size.

## wave 97

Base: the wave-94 draft (`sub_0801A718.c`, 83.33%, size-exact, first difference +0x2); saved as `sub_0801A718.w97-start.c`. `best.c` (88.64%) was NOT adopted: it inserts `node = &base[freeIndex];` before `base = gUnknown_0200C624;`, an uninitialised read of `base` (the value is dead, but it is undefined C). It is the same wave-94 form already quarantined as `w94-perm2-8864.c.wrongc`; `best.c.wrongc` is that file.

Residual (read from the disassembly, 53 of 66 halfwords already equal): the ROM copies `a1` to r3 at entry and reuses r3 for `cur` once `a1` has been stored (`adds r3,r0,#0; subs r3,#12` comes AFTER `str r3,[r2]`), while `prev` gets r5. The draft puts `a1` in r5, shares it with `prev`, and gives `cur` r3.

Probed this wave (one-unit harness `build/probe/w97k.py`, 25 spellings, every one byte-identical or worse):
- `node` before/after `cur`, `base` bound before or after each, `cur` written as `gUnknown_0200C624 - 1`, `&gUnknown_0200C624[-1]`, or a u8 cast subtraction (51-53 of 66 halfwords equal, none better than the draft).
- Order of the three stores (`unk00`, `unk08`, `prev = 0`): all six permutations, 52-53 of 66.
- `a1` copied to a local `payload` first; declaration order of `node`/`prev`/`cur` reversed or rotated: identical.
- `cur = base - 1` moved to AFTER the `unk00` store, to give the ROM's instruction order (a1 dead before cur is set): 40 of 66, worse. The mechanism is that the pseudo CREATION order of `cur` (before `node`) is what wave 93 found worth 10 points; moving the assignment after the store gives up that order and loses more than the interference is worth.
- `base` bound at declaration time before the size guard: worse (5-38 of 66; +4 bytes in two forms).
- Permuter, 900 s x 2 threads from the draft: NO-IMPROVEMENT.

So the two effects (creation order of the base pseudos vs the ROM's instruction order for `cur`) are coupled and no defined spelling separates them. Proposed summary: does = insert a record into the sorted linked list; status = "132 bytes, size exact, 83.3%; only which register holds the first argument differs"; left = "the original copies a1 into r3 and reuses that register for the list cursor; ours keeps a1 in r5"; tried = the above.
