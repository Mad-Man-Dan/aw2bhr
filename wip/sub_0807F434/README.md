# sub_0807F434

0x0807F434, 240 bytes, THUMB, parked.

Best score so far: 95.8%.

## What it does

Sets up a screen's sprites. It copies a per-entry list and loads graphics for each entry (sub_08043B14, sub_08043BA4), then decompresses one graphics blob and copies it to sprite VRAM at 0x06015000 in rearranged 256-byte blocks, and loads one sprite palette.

## How close it is

Compiles to the right size (240 bytes) with 95.8% of bytes identical; binding `lv = j * 0x800` before the inner loop fixed the shift order. What is left: the 0x06015000 add comes before the counter start where the original has it after, and the final zero is in r5 where the original has r4.

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
- `NOTES.md`: working notes
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

### Wave 97

wave 97
Base: the old draft (17.74%, 248 B, +8; kept as sub_0807F434.w97-start.c). Now **240 B size-exact, 91.67%**, first diff +0xF
(three register renumberings, nothing structural). Final source is work/sub_0807F434/sub_0807F434.c.

What moved it (each measured, in order):
1. `i + 1` in the second loop's call argument respelled `-~i` (17.74% +8 -> 85.42% size-exact). cse shared the `i+1` with the
   loop step and computed it at the top of the body, which took the register for a hoisted `.rodata` address word; `-~i` is
   not seen equal by cse but combine folds it to `adds r2, r5, #1` after the call, as the ROM has it. `(i + 2) - 1`,
   `(i * 2 + 2) / 2`, `(i + 3) - 2`, `i - (-1)` all fold in a pass before cse and stay 17.74%. `(s16)i + 1` / `(u8)i + 1`
   work on sharing but add an `lsls/asrs` pair (+4). A plain or s16 local copy `tn = i` did not help (+8 / +16).
2. Binding `p` no longer inside the Decompress argument: `Decompress(gUnknown_08234B10, gUnknown_0200FC50);` then loops that
   name gUnknown_0200FC50 directly (85.42 -> 87.92 ; with `for (j = 0, p = ...` 89.17).
3. The copy loop in the sibling sub_0807E980's spelling (`for (k = 0, nv = 0; ...) { x = 0x06015000 + j*0x800 + nv;
   CpuFastSet(&gUnknown_0200FC50[j*0x100 + k*0x400], (void *)x, 0x40); nv += 0x100; }`) -> 91.67%.

Residual (all register names, no size change): the ROM keeps the Decompress source word in r7 and the buffer word in r6;
we get them swapped (r6/r7). The outer counter `j` is in r5 in the ROM and in r2 for us, and the source/dest add order in
the loop preheader differs. Spellings of the Decompress buffer argument (`p` bound before, `&buf[0]`, `(void *)`, `+ 0`)
are byte-identical at 91.67%, and a separate `src = p + j*0x100` bind or an inline dest expression did not change it.

Proposed summary: does = as before; status = "size-exact (240), 92% of bytes; only register numbering differs";
left = "which of two hoisted address words gets r6 vs r7 and the outer counter's register"; tried = the above.

Update (end of wave 97): final draft is **94.17%, 240 B size-exact**. Permuter runs: 91.67 -> 93.33 (`new_var = &i` in the first
loop, un-shares `i * 12` -- valid C, no frame change) -> 93.75 (`j = 0; proc->unk4c = j;`) and the destination written inline
in the CpuFastSet call -> 94.17; a third run found nothing. Remaining 14 bytes: the two shifts `j<<8` / `j<<11` are in the
opposite order and `ldr r0,=0x06015000; adds r4,r1,r0` comes before `movs r6,#7` where the ROM has it after; final zero
`movs r4,#0` is in r5 for us. Reordering the operands of the src/dest index sums does not move them.

wave 97 (second pass)
Base: 94.17% draft (`sub_0807F434.w97-second-start.c`). Now **95.00%, 240 B size-exact**, first diff +0x1c (only the relocation display; code differs from +0x4d).
Lever: the copy-back outer loop (next index computed first, assigned back at the bottom): `for (j = 0; j <= 3; ) { int nj = j + 1; for (k...) {...} j = nj; }`. It gives the ROM's `adds r7,r5,#1` first in the preheader and the counter/pointer sharing r5.
Residual (12 bytes): `lsls r0,#8 / lsls r1,#0xb` in the opposite order; `ldr r0,=0x06015000; adds r4,r1,r0` before `movs r6,#7` (ROM after); final zero `movs r5,#0` vs ROM `movs r4,#0`.
Negatives: source pointer as a walker on top of the copy-back (89.2%; 88.3% without the copy-back); final zero via `k`/`nv`/`x`/literal gets `movs r4,#0` (ROM) but moves j's counter to r1 (93.3-94.2%), because the trailing `j = 0` is what keeps j in r5; using `i` for the copy loop (as the ROM appears to) needs the `&i` trick removed first: with it, i is stack-resident (9%).
Untried: replace the `new_var = &i` unshare trick with another cse-splitter so the copy loop can use `i`.
Followed up: dropping `new_var = &i` and using `i` itself for the copy loop (copy-back form, the first loop's `i * 12` spelled `(i*3)*4`, `(i<<2)*3`, `i*12`, `(i*6)*2`) gives 91.7-93.3% with the first difference back at +0xF (Decompress source/buffer words swapped r6/r7), so the `&i` unshare is still needed for those; the 95.00% draft keeps `j` and `&i`.

wave 97 (W97-AB)
Base: 95.00% draft (`sub_0807F434.w97ab-start.c`). Now **95.83%, 240 B size-exact**, first diff +0x1c (relocation display only; code differs from +0x4d). Kept as `sub_0807F434.w97ab-v1.c`.
Lever (from `levers.py --chain 3`, lever 5a; wrongc OK): bind `j * 0x800` in a plain int before the inner loop, `lv = j * 0x800;`, then `(0x06015000 + lv) + nv`. It fixes the `lsls #11` / `lsls #8` order (the ROM computes j*0x800 first).
Residual (10 bytes): `ldr r0,=0x06015000; adds r4,r1,r0` is emitted BEFORE `movs r6,#7` where the ROM has it after; the final zero is `movs r5,#0` (strh through r5) where the ROM has r4.
Negatives this pass: the destination as an explicit walker (`d = (u8 *)(0x06015000 + lv); d += 0x100;`, bound before the loop or in the for-init): 86.67% both; `x = ...` statement form, `0x06015000 + (lv + nv)`, `nv = 0` moved out of the for-init: byte-identical to 95.83%. A few of these were not re-tried against the earlier wave-97 negatives (permuter ran three times before).
Proposed summary status: size-exact, 95.8% identical; left: the invariant `0x06015000 + j*0x800` add is emitted before the counter start instead of after, and the final zero sits in r5 rather than r4.

</details>
