# sub_0807F434

0x0807F434, 240 bytes, THUMB, parked.

Best score so far: 17.7%, +8 bytes.

## What it does

Sets up a screen's sprites. It copies a per-entry list and loads graphics for each entry (sub_08043B14, sub_08043BA4), then decompresses one graphics blob and copies it to sprite VRAM at 0x06015000 in rearranged 256-byte blocks, and loads one sprite palette.

## How close it is

Compiles 8 bytes too long (248 against 240), 18% of bytes match; the score means little because the difference starts in the opening lines and shifts everything after it. The root cause is one extra instruction: in the second loop the compiler computes `i + 1` once at the top of the body (it is both the call's third argument and the loop step), which takes the register the original uses for a buffer address word, and the other extra bytes follow from that.

## What is left

Find a way to write the second loop so `i + 1` is not one shared expression computed at the top of the body; everything else should follow. The permuter has never been run on this draft and is the other open step (copy the draft first).

## Already tried

- `if (i != 2)` versus `if (i == 2) continue;`: identical.
- A separate counter for the third argument: worse, the body gets two increments.
- Naming gUnknown_0200FC50 directly in the copy loop instead of binding it to p: the stepping pointer is lost and one of the three compiler-made address words disappears.
- Assigning p in a statement before the Decompress call: also drops the third address word; the assignment inside the argument is kept.
- `do { } while (0)` in six places around the loops and calls: byte-identical; the shared `i + 1` is decided before register allocation, the only thing this trick can affect.

## Files

- `sub_0807F434.c`: the current draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

18.3% identical, candidate 248 bytes (+8), first difference at +0xe

### What still differs

REGISTER ALLOCATION with ONE root cause and three downstream symptoms. (1) Loop 2 emits one extra `adds r6, r5, #1` at the TOP of the body: `i + 1` is the third argument in the then-block AND the loop increment on both paths, so it is fully redundant and gcc hoists it to the dominator, where the ROM computes `adds r2, r5, #1` inside the block and `adds r5, #1` at the merge. (2) That hoisted pseudo takes r6, so the middle .rodata pool address (0x081D9370, for gUnknown_0200FC50) goes to r8 instead of the ROM's r6 and the prologue costs `ldr r0,[pc]; mov r8,r0` against the ROM's `ldr r6,[pc]` (+2). (3) At the Decompress call that forces `mov r1,r8; ldr r1,[r1]; mov r8,r1` against the ROM's `ldr r4,[r6]; adds r1,r4,#0` (+2), because a high register cannot be the base of the ldr. (4) In the blit nest j lands in r2 rather than r5 with src/dst swapped between r4 and r5 -- same instruction count, different bytes.

### Why it is close

Both bl-conditioned loops, the dbra rewrite of the inner blit loop, all three .rodata address-constant words (0x081D936C/9370/9374) and their order, and the whole tail come out exact.

### Already ruled out

- Prior wave, spot 1: `if (i != 2) { ... }` and `if (i == 2) continue;` are identical here.
- Prior wave, spot 1: a separate counter `n = 1; ... n++;` is WORSE -- gcc does not eliminate it against i, so the body gets two increments instead of one.
- Prior wave, the blit nest: naming gUnknown_0200FC50 directly instead of binding it to `p` loses the accumulator entirely (the base is re-ldr'ed from .rodata inside the innermost loop and no giv is formed) AND drops the third .rodata word. Assigning `p` in a statement BEFORE the call also drops the third word; `p = gUnknown_0200FC50` inside the argument is what restores all three and is the form kept in the draft.
- Wave 59 (W59-G): re-verified by exit code, still +8 / 18.3%. No new axis attempted.

### Notes

THE SINGLE HIGHEST-VALUE UNTRIED AXIS ON THIS FUNCTION IS A PERMUTER RUN, AND IT IS UNTRIED RATHER THAN RULED OUT. This is order-wrong/slot-wrong register allocation with the instruction sequence otherwise correct, which is exactly the class docs/agbcc-codegen.md names as decomp-permuter's real case (as opposed to a residual of one extra instruction, where it is useless). The wave-54 park's own NEXT STEP said to run it. It was NOT run in wave 59 because `mcp permute` is broken -- KeyError on 'exit_code', and it strands raw header-expanded output in work/<fn>/<fn>.c, destroying the draft; the docs claiming this was fixed in wave 17 are wrong. Copy the draft before running it.

### Wave 87

WAVE 87 (W87-F, do{}while(0) transfer test): pre-registered 'a wrapper around the then-block is a loop boundary for the i+1 hoist' REFUTED, cleanly -- SIX placements (then-block; whole loop-2 body; Decompress call; blit nest; then-block + Decompress composed) ALL BYTE-IDENTICAL to the baseline after label normalisation; draft unchanged (18.3%, 248/240 +8, first difference +0xe), 0 try_match. Mechanism: the hoist of `i + 1` is NOT LICM and NOT an allocation decision -- it is a GCSE common subexpression of the then-block and the loop increment hoisted to the dominating block; a wrapper leaves that block dominated by the loop head, and the wrapper's only effect (REG_N_REFS loop-depth weighting for allocno_compare) happens in global.c AFTER gcse. RULE: a do{}while(0) cannot move a value that a PRE-ALLOCATION pass (cse/gcse/loop.c) put where it is; it re-ranks allocnos only. The pool address going to r8 is a consequence of the hoist taking r6, so it is unreachable too -- transfer rate here is exactly zero on six placements. Next: the permuter, named by waves 54 and 59 and still NEVER run (wave 59 was blocked by the broken mcp permute; the shell tool works, waves 83-87) -- snapshot first. Beyond that, a source form in which `i + 1` is not a common subexpression of the then-block and the increment (the park already rules out `if (i == 2) continue;` and a separate counter). Do not spend another do/while probe here.

</details>
