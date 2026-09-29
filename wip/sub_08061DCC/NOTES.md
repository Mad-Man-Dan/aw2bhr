# sub_08061DCC — wave 92 (W92-B)

Draft unchanged at **27.21%, size-exact (136/136)**. Two things the wave brief
asserted about this function are false, and both are measured here.

## 1. There is no pool-word fix to do. 0x0816DB08 / 0x0816DB0C are real objects.

The wave brief said 0x0816DB08 "holds 0x03004784" and 0x0816DB0C "holds
0x085D5ABC", and told me to rewrite the draft to name gUnknown_03004784 and
gUnknown_085D5ABC directly, the way the batch-A/B pool-word fix works elsewhere.
That is the wrong reading. Read straight out of `baserom.gba`:

    sub_08061DCC's OWN literal pool, at +0x24 and +0x84:  0816DB08, 0816DB0C
    the word at 0x0816DB08:                               03004784
    the word at 0x0816DB0C:                               085D5ABC

So the function's own pool words already hold 0x0816DB08 / 0x0816DB0C. The
objects at those two addresses are a further level of indirection that exists in
the ROM, not something the compiler invented. The ROM really does execute three
loads to reach the data:

    ldr r2, [pc, #24]   ; r2 = 0x0816DB08   (this function's pool word)
    ldr r0, [r2, #0]    ; r0 = 0x03004784
    ldr r0, [r0, #0]    ; r0 = the u8 * stored there
    ldrb r0, [r0, #3]

Naming gUnknown_03004784 directly produces only two loads, which is exactly the
wave-37 measurement recorded in the parked entry ("16 bytes short"). That
negative was never stale — it was right, and its cause is this indirection
level, not a CSE accident. The current declarations

    extern u8 **volatile gUnknown_0816DB08;
    extern struct UnitType *volatile gUnknown_0816DB0C;

have the correct number of levels and should be left alone. The parked entry's
`left` field, which told the next agent to name the two globals directly, has
been corrected.

## 2. `-fno-force-mem` does NOT move this function. Its 66.91% is a false score.

The flag sweep ranked `-fno-force-mem` as this function's best lead: 27.21% ->
66.91% at the same size. Compiling both and reading the two diffs instruction by
instruction shows the flag changes nothing about the residual.

Under the flag, the first two blocks are byte-for-byte what they are without it —
same wrong order in the entry block, same missing address copy at the multiply.
What the flag does is insert one extra instruction in an unrelated block: at the
`unk04 & 0x780` test it stops forcing the memory operand out first, so instead of
the ROM's

    ldrh r1, [r4, #4] / movs r0, #240 / lsls r0, r0, #3 / ands r0, r1

it emits five instructions, `movs / lsls / adds r1, r0, #0 / ldrh / ands`. The
draft without the flag matches that block exactly and is one instruction short
overall; the flag breaks the block and is one instruction long there, so the two
errors cancel and every later instruction lands on the ROM's address. The whole
tail after `bl __divsi3` then compares equal and the score triples.

This is the positional-score trap in its purest form: **the flag bought byte
alignment by breaking a block that was already correct.** Anyone reading only the
percentage would rewrite a correct block to keep it. `-fno-force-mem` is refuted
for this function and should not be carried forward.

Read the other way, the same probe is evidence *for* the default: `-fforce-mem`
is what produces the ROM's `ldrh`-first order at that test, so the ROM was built
with it, as expected.

## 3. The residual is owned by cse, and it is the shared constant 92

The remaining difference is two instructions with one mechanism. The ROM
rematerialises the record stride at both multiplies and spends the freed
register on the second table address:

    ROM        movs r0, #92 / muls r0, r2        ... adds r5, r3, #0
    draft      movs r5, #92 / adds r1, r3, #0 / muls r1, r5

Because the draft keeps 92 alive in a callee-saved register across both
multiplies, the multiply's destination has to be a copy of the index instead of
the constant, and there is no callee-saved register left for the address copy
the ROM makes. Five cross-block values against the ROM's four.

Which pass merges the two constants is now settled. Counting `const_int 92` in
the per-pass RTL dumps (`python tools/rtldump.py sub_08061DCC --flags=-da`):

    rtl 4, jump 4, **cse 2**, addressof 2, loop 2, cse2 2, jump2 2, flow 2,
    combine 2, regmove 2, lreg 2, greg 2

The drop happens at cse and nowhere else, so this is cse substituting a register
already known to hold 92 for the second constant. It is not gcse (which declines
CONST_INT), not regmove and not the allocator, and none of those should be
targeted. Wave 89 had guessed the allocator.

That also says what a lever would have to do: stop cse from carrying the
constant's register into the second block. The two multiplies sit on either side
of a control-flow join (the two arms of the `||`), which wave 89 lists as an
EBB-table splitter, and the join evidently is not enough here. Nothing in the
source respelling space reaches a bare CONST_INT — wave 89 measured that a mask
on a constant folds before cse numbers it, and the fifth splitter needs a narrow
*variable* operand. The permuter remains the backstop.

## Pool words owned

    0x08061DF0  -> 0x0816DB08
    0x08061E48  -> 0x0816DB0C   (pool word at +0x84)

Both are plain address constants for the two ROM pointer objects named above.

## What `-fno-force-mem` stands in for: nothing. Replicated on the two AI-block siblings.

The orchestrator asked for the source construct behind this flag's gain here,
because it also lifts sub_08062FF4 (22.3% -> 61.6%) and sub_08061308
(13.5% -> 49.8%). There is no such construct. In all three the gain is the same
size-realignment artefact, and measuring the size and the first-difference
offset alongside the percentage shows it immediately:

    function        default                      with -fno-force-mem
    sub_08061DCC    27.2%, size match, +0x4      66.9%, size match, +0x4
    sub_08062FF4    22.3%, size -8,    +0x14     61.6%, size -4,    +0x14
    sub_08061308    13.5%, size -20,   +0x1c     49.8%, size +4,    +0xa

**Read the third column, not the first.** On sub_08062FF4 the first difference
does not move, and diffing the two candidates against the ROM side by side shows
the leading region is instruction-for-instruction identical under both settings —
the only change there is a branch whose target label has shifted by the four
bytes the flag adds further down. The flag changes nothing about the divergence;
it halves the size deficit, so the tail lands closer to the ROM's addresses and
the byte-identity count nearly triples.

**sub_08061308 is the clearest case and it points the other way.** The flag
moves the first difference from +0x1c to +0xa, so it *breaks eighteen bytes of
code the draft already had right*, and it overshoots the size from -20 to +4.
Structurally it is strictly worse than the draft, and it still scores 36 points
higher.

The mechanism is the one the flag's name describes: -fforce-mem copies a memory
operand into a register before the other operand of a binary op is expanded, so
turning it off flips the emission order of a load against the constant beside it
at *every* such site in the file. Where the draft's order was already the ROM's,
the flip breaks it; where it was wrong, the flip fixes it. On a function that is
short of the ROM's size, the net byte change is what drives the score.

So the flag is not a lever and there is nothing to carry across to those two.
What does carry across is the screen: **a flag or permuter result that changes
the candidate's SIZE has to be judged on its first-difference offset and on
which instructions it added, never on the percentage.** All three of these
functions are short of the ROM, which is exactly the condition that makes the
metric unreliable.

## wave 97 (W97-G)

Base: draft (27.21%). Moved to **32.4% size-exact, first difference +0x9** (was +0x4) by removing the early
`pa = &gUnknown_0816DB08;` / `pb = &gUnknown_0816DB0C;` statements and binding them at the first use:
`if (p->unk04_0 < (*(*(pa = &gUnknown_0816DB08)))[3])` and `pb = &gUnknown_0816DB0C;` right before the ammo test.
The bitfield load and shift now come first as in the ROM.

Residual: the ROM keeps the pool address in r2 for the first read and copies it to r6 AFTER that read
(`adds r6, r2, #0`); the draft copies first and reads through r6. Three respellings (comma-bind after, bind
in a `+ (pa = ..., 0)` term, bind at the start of the second block) all scored worse (25.7%, 27.2%, 19.9%).
Second half: ROM loads the unit id once into r2 and holds a copy in r3, multiplies by a fresh `movs #0x5c` each
time; draft shares the 92 constant in r5. Binding the id to a u8 local (`[id]` in both places) drops to 128/136
(-8); the fold-proof mask form on the local is byte-identical to the current 32.4%. So the fold-proof mask stays.
No wrong C: pa/pb are pointers to the volatile pointer objects (no local slot, frame unchanged).

Proposed summary: does = raises byte 9's 3-bit field to 2 when the unit's HP is below the first threshold; to 1
when ammo is used and HP is not full-fuel proportional. status = 32.4% size-exact. left = address copy order
(r6 copy after first read) and the 92 multiplier shared instead of rematerialised. tried = see above + waves 37-92.
