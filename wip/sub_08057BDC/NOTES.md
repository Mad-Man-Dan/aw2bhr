# Wave 93 (W93-C)

Unchanged at 356/360 (-4), 58.06%, first difference +0x19. No probe moved it.

## best.c is padding, and has been renamed best.c.wrongc

It is the draft plus five dead statements inside the second loop:

    gUnknown_08551A04[0] += 0;
    gUnknown_08551A04 += 0;
    rows += 0;
    gUnknown_03004582[i][0] += 0;
    c = c;

They are all value-preserving, so this is not wrong C, but it reaches 360
bytes by padding. Its first difference is +0x19, IDENTICAL to the draft's.
Four extra bytes bought exactly nothing, and the 61.94% it scores is the
size-delta artefact the brief warns about. Do not adopt it, and do not let a
base scan recommend it again.

## +0x19 is a pool offset, not a real early divergence

+0x19 is an ODD address, so it is the high byte of the instruction at +0x18:
an `ldr rN, [pc, #imm]` whose immediate moved because the literal pool at the
END of the function was reordered. The park is right that everything up to
the second loop is instruction-identical. Read this residual from the second
loop, never from the score.

## The ROM's two hoists are LICM hoists, not source bindings

    _08057C9A:
        movs r7, #0                              <- i = 0, the SOURCE init
        ldr r1, =gUnknown_085D6A48 ; mov sb, r1  <- hoisted AFTER it
        ldr r2, =gUnknown_03004582 ; mov r8, r2  <- hoisted AFTER it

By the preheader rule (source init first, then LICM hoists), both address
loads sit after the counter's init, so the loop optimiser put them there.
The draft binds `rows` before the loop in SOURCE, which is the wrong
construct on paper. Measured, it makes no difference at all:

    pre-loop binding removed, cast inline at the use   57.78%  -4  +0x19
    binding moved inside the loop body                 BYTE-IDENTICAL
    selector gUnknown_03004582[i][0] in its own local  57.78%  -4  +0x19

So the park's "the compiler substitutes the global straight back" holds for
every placement of the binding, not only the one it measured. The binding's
position is byte-neutral here and is not the lever.

## What the residual actually is

In the ROM, `gUnknown_08551A04`'s ADDRESS is loaded INSIDE the loop
(`ldr r4, =gUnknown_08551A04`) and its value reloaded either side of the two
calls, while 085D6A48 and 03004582 are hoisted into sb and r8. The candidate
hoists 08551A04 -- it has two references per pass against one each for the
others -- and rematerialises the other two. It is one LICM priority
inversion. Nothing in the source-level vocabulary tried so far reaches it.
The permuter has never been run on this function; that is the obvious next
move.


## THE PERMUTER HAD NEVER BEEN RUN ON THIS FUNCTION, AND IT WAS THE ANSWER (W93-C)

**58.06%, 356/360 (-4), first difference +0x19  ->  76.94%, 360/360
SIZE-EXACT, first difference +0x90.**

Eight waves of parking, a long ruled-out list, and nobody had run
`tools/permute.py` on it. The residual the park describes -- one LICM priority
choice, 4 bytes short, instruction order otherwise correct -- is the exact case
the tool exists for. One 900 s run moved the first difference from +0x19 to
+0x90 and closed the size.

### What it found, and it is the lever this file had been looking for

    sub_0805772C(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);
    do { } while (0);                      <- HERE
    sub_080577E4(gUnknown_08551A04, idx, (struct Unk8057Pos *)p);

An empty zero-trip loop BETWEEN the two calls. That is wave 89's
"statement boundary splits cse's EBB table" splitter applied to the precise
place the park identified: gUnknown_08551A04 is the only one of the three
addresses used twice per pass, the two uses are what let it win the hoist, and
a boundary between them stops the pair being unified. Every lever the park had
tried (pointer self-use, pointee `+= 0`, self-assignment, dead local uses) acted
on the VALUE or on the loop as a whole; none of them put a boundary between the
two references.

Two more edits came with it, both audited and kept:

- `sel`, a `u16 (*)[8]` bound to gUnknown_03004582 at the top of the function.
  Load-bearing: removing it drops the draft to 59.44% AND loses the size.
- `idx` reused for `p->unk02` inside the first loop's else arm, after `p` has
  already been computed from the old `idx`. Value-identical (`idx << 5` ==
  `p->unk02 << 5`) and `idx` is recomputed at the top of the next pass.

### A measured warning about the one thing that still looks wrong

The second loop's guard is `active = 0 != gUnknown_030005E8[i];` followed by
`if (active && ...)`, and it compiles to the branchless `negs/orrs/cmp/bge`
form where the ROM has a plain `cmp r0,#0; beq`. That looks like an obvious
4-byte fix. It is not:

    active = 0 != gUnknown_030005E8[i]   (kept)     76.94%  size+0  +0x90
    active = gUnknown_030005E8[i]; active != 0      68.33%  size+0  +0x90
    the same, with sel bound late instead of early  68.33%  size+0  +0x90
    no sel at all, global written inline            59.44%  size-4  +0x90

All four share the first difference, so the "wrong" branchless form is the one
with the fewest differing bytes downstream. Do not tidy it without re-measuring.

### What is left

Size-exact, first difference +0x90. The ROM hoists gUnknown_085D6A48 and
gUnknown_03004582 into sb/r8 in the second loop's preheader (`movs r7,#0` then
the two `ldr;mov` pairs, so both are LICM hoists) and the candidate now
rematerialises all three inside the body, which is also why the tail pool words
sit in a different order. The 08551A04 half of the park is solved; the hoist of
the other two is not.

## wave 96
Base: the wave-93 draft (`sub_08057BDC.w96-start.c`, 76.94%). Now 81.11%, size+0, first diff +0xc1 (was +0x90): the FIRST LOOP IS NOW BYTE-EXACT.
- What moved it: delete the `idx = p->unk02;` reuse in the first loop's else arm and write `((p->unk02 << 5) + p->unk00)` directly. The reuse forced `p->unk02` to be loaded into a register BEFORE the `14 - c` arithmetic (`ldrh r5,[r6,#2]` first); the ROM loads it after, at the use. So the wave-93 note that `idx` is reused in the else arm was a compensation for the old second-loop state and is now wrong; the source comment above the function still mentions it (leave for the orchestrator, or trim when promoting).
- Second loop, unchanged residual: the ROM hoists gUnknown_085D6A48 (r9) and gUnknown_03004582 (r8) into the preheader after `movs r7,#0` and tests `gUnknown_030005E8[i]` with a plain `cmp r0,#0; beq`; the draft rematerialises both and uses the `negs/orrs/bge` form. Re-measured on the new base: plain `if (gUnknown_030005E8[i] != 0 && ...)` 63.9%, size-4 (with `rows` bound at the top or in-body, with `sel` bound or inline, with or without the do{}while(0)): 57.8-63.9%, all -4 at +0x19/+0xc0. Binding position of `rows` (top, in-body, inline cast): byte-identical at 81.11%. So the hoist is still unreached.
- Permuter (900 s, 2 threads, from the new base): NO-IMPROVEMENT (scores 2660 -> 2340 candidates were not size/verify-clean).
Proposed status: first loop matches; left = the second loop keeps two table addresses in registers in the ROM (LICM hoists) and the draft rematerialises them, plus the `ldrsh; cmp; beq` test form.
