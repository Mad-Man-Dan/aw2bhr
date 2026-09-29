# sub_0801F4B4

0x0801F4B4, 572 bytes, THUMB, parked.

Best score so far: 96.5%.

## What it does

Computes a unit's movement range with a flood fill from cell (a1, a2). It seeds a queue at cost 0, then repeatedly takes cells from one queue and steps in every direction except straight back into the other, until no cells are left.

## How close it is

Compiles to the right size (572 bytes) with 96.5% of bytes identical, from one statement: `pp = &gUnknown_0300409C;` just before the `do`, used only in the `while` test. What is left: the original's pointer is a copy of the register holding the compiler-made address word itself, one load earlier than any C spelling of the global gives.

## What is left

At two places, where the two queue-swap branches meet and at the bottom of the inner loop, the original copies the address of the read cursor gUnknown_0300409C into a second register; our build reads it directly. Find source that makes the compiler make those two copies.

## Already tried

- Holding `&gUnknown_0300409C` in a pointer local, for the whole outer loop or only after the queue swap: the queue step compiles worse and the switch gets shorter; the two copies do not appear.
- A pointer-to-pointer local set after the queue swap: 564 bytes, 4 more bytes short.
- Pinning that local to the register the original uses: the compiler still loads through it instead of copying.
- Writing the inner loop as `while` instead of `do/while`: no improvement.
- A long permuter run (about 25,800 attempts): no improvement.

## Files

- `sub_0801F4B4.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 568/572 (-4), 58.6%. ROM has two isolated copies of the force-address pointer around the swap-loop merge; whole-loop, after-merge, fixed-r1 and pointer-to-pointer bindings fold or regress. Prior long permuter and loop-form probes are negative.

### Wave 95

Base: sub_0801F4B4.w95-start.c (58.6%, size -4). Draft unchanged so far.

Read of the ROM: the 0x08090928 word is a compiler pool word holding &gUnknown_0300409C. r4 holds ITS address (`ldr r4,=gUnknown_08090928`), every ordinary cursor access is `ldr rX,[r4]` then a load through that, and r1 = `adds r1,r4,#0` at the swap-merge and at the inner-loop bottom is a second pseudo `pp` holding the word's address, defined twice (merge + back edge) and used only at the loop head (`ldr r0,[r1]; ldr r0,[r0]; ldrb r0,[r0,#2]`, once in the pre-loop empty test, once as switch discriminant). r1 cannot live across the calls, hence the redefinition at the bottom.

Probe: declared `struct Unk300409C **const gUnknown_08090928;` in the header and wrote `pp = &gUnknown_08090928` at the merge and at the loop bottom, reading `(***pp).unk02` for the empty test and the switch. NEGATIVE: 584 bytes (+12) and the prologue changed (first difference +0x12): naming the word as a real symbol makes the compiler load its address through a second pool word (`mov r7,sl; ldr r1,[r7]` at the bottom) instead of reusing r4. Header edit reverted. The name-the-word form does not reproduce a copy of the existing force-addr register; the pp spelling needs an address VALUE that CSE shares with the force-addr word of the bare global, which only the bare global itself provides.
Not run: permuter chain (queue was full behind sub_0801C01C / sub_0802216C / sub_0801E9B0).

### Wave 96

Base: sub_0801F4B4.c (w95 state, 58.57%, size-4). Result: 96.50%, size-EXACT, first difference +0x10c, about 5 instructions differ. NOT matched.

What moved it (permuter, 600 s, then ablated by hand): ONE statement. `struct Unk300409C **pp = &gUnknown_0300409C;` placed just before the `do` and used ONLY in the loop condition (`while ((*pp)->unk02 != 0)`). The switch and the empty test keep naming the global. The permuter's other edits (`long long new_var = 2` as the index of `[2] = 1`, and a copy of `a3` passed to sub_0801F888) are noise: removing them leaves 96.50% and the same bytes. Removing the pp bind returns to 58.57%.
Mechanism: a bind used at exactly one site makes that one read go through a second pseudo (`ldr r0,[r1]` after `adds r1,rX,#0`) and stops cse from folding it into the address load the switch already did. That is the ROM's "copy at the loop bottom". It reproduces the bottom copy but not the top one.

Earlier waves' pp forms (bound at the merge and used everywhere, or reassigned at the loop end, wave 65/71) were the ones that lost bytes; the eight placements measured this wave (bind before the empty test / before the do / at the top of the body; used in empty test, switch, condition, in every subset; reassigned at the loop end) are all worse (raw instruction diff 70-80 lines against 20) except this one.

Left: at the merge the ROM has the copy too (`adds r1,r4,#0`) and the empty test and the switch discriminant read through it. Here that read is `ldr r1,[r4]; ldr r0,[r1]; ldrb` and the bind lands in r6. A second variable bound at the merge (two variables, as pre-registered) and used for the empty test and/or switch, with the condition using a separate one, is WORSE (raw 104 vs 20). So the pre-registered hypothesis holds for the loop-bottom copy only: one bind, one site.

Pool words: no new .rodata pool words owned.

Proposed summary: does = walks outward from (a1, a2) with two swapped queues; status = 96.5% identical, size exact; left = the copy at the loop entry that the empty test and the switch read through; tried = pp bind at eight placements, second bind at the merge, permuter (found the one-site bind).

### wave 96, final round
Looked once more at the residual with the eye of "one association or one bind": it is the bind. The ROM's `pp` is a copy of the force-addr register (the address of the rodata word), used by the empty test and the switch, and the load of &G goes through it each time. Every C spelling tried binds `&gUnknown_0300409C` (the VALUE loaded from that word), which puts the copy one load later. Reproducing the ROM needs the word's own address as a source value; wave 91's `pp = &<force-addr word>` form needs that word declared as a symbol, which wave 61-71 showed costs a second pool word. No additional probe this round. Kept: 96.50%, size-exact.

### Wave 97

wave 97 (second pass)
No probe spent; base unchanged (96.50%, size-exact). Reading of the previous rounds against the lead's lever list: lever 2 (mixed bind, first reference bare) is what the one-site `pp` bind already is; lever 1 (respell one of two identical expressions) has no second expression to respell, because the copy the ROM wants is of the REGISTER holding the compiler-made address word (the address of the .rodata word), which no C expression names -- `&gUnknown_0300409C` is the value one load later, and naming the word costs a second pool word (wave 95). Left for a tool-level answer (a levers.py site for "copy of a force-addr register"), not a source spelling.

wave 97 (W97-W)
Alias lever does not apply: gUnknown_08499598 (gPlayers) is used once in the function and has one pool word in the ROM; the residual is the copy of the .rodata word gUnknown_08090928 (pp), whose symbol is not aliased in aw2bhr.lds (the aliased ones are gMap, gBG*TilemapBuffer, gPlaySt, gPlayers, gGameClock, gTextTable etc.). Not probed further. Draft unchanged (96.50%).

wave 97 (W97-PG)
Permuter chain: 1 link, 96.50% -> 96.50%, NO-IMPROVEMENT.

</details>
