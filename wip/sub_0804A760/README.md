# sub_0804A760

0x0804A760, 920 bytes, THUMB, parked.

Best score so far: 92.1%.

## What it does

The per-frame input handler for an on-screen character grid, apparently a text-entry keyboard. A types the character under the cursor or acts on a special key, B deletes, Start calls sub_0804A6D8, and the d-pad moves the cursor with wrap-around.

## How close it is

Compiles to the right size (920 bytes) with 92.1% of bytes identical. The original reloads the pointer global through its compiler-made address word in each arm of the key loop; assigning `gp = &gUnknown_030044E0;` again at the top of the increment and decrement arms reproduces that. What is left: the `unk1e == 0` test in the decrement arm, which the original reads as a signed halfword; that spelling brings the hoist back.

## What is left

Two things remain: the construct that naturally gives flag, t, u and c the original's registers (the pins stand in for it), and the key-repeat do/while loop, where the original re-reads the gUnknown_030044E0 pointer in each branch but our compiler merges the loads into one copy before the loop, costing an extra literal-pool word (the 4 extra bytes). A spelling that blocks that merge in the loop only has not been tested.

## Already tried

- Removing the four register pins (with the other fixes in place): 924 bytes, 12.4%.
- Reading the pointer through a pointer alias to stop the merge: it stops it everywhere in the function, 10 bytes too long.
- About twenty variants (barriers, other pins, volatile views, reordering, signed and unsigned types, switch guards): none closer.
- All seven compiler profiles: no match.

## Files

- `sub_0804A760.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

924/920 (+4), 59.3% identical, 374 differing bytes, first difference +0xae (W80-D, configured). best.c 60.1% is the same draft with only fix 1 applied. The draft carries `register ... asm("rN")` pins from wave 76 that are NOT source (see remaining_diff 1) but score 47 points better than the unpinned form.

### What still differs

Instruction count 420 vs the ROM's 421 plus one extra literal-pool word, in three hunks. (1) `case 1` default arm: `add r6,r6,r0; ldrb r0,[r6]` against the ROM's `adds r0,r6,r0; ldrb r0,[r0]` -- local-alloc ties the sum's destination to the dying table-address pseudo (r0); with `t` pinned to r6 by `register int t asm("r6")` the hard reg wins instead. Unpinning all four (flag/t/u/c) is 12.4%, first difference +0x9: flag lands in r7 and the three `unk63 = flag` arms cross-jump into one `strb`. (2) `case 0x40` arm: the `& 0xff` mask is now present (multi-set local) but the value lives in r3 and the zero constant is materialised after the `orr` where the ROM has it in r4 right after the `adds #1` (before `movs r1,#0xff`). (3) THE +4: in the key `do { } while (sub_0804A18C(...) == u)` loop the ROM reloads the gUnknown_030044E0 force-addr word through r7 in EACH arm (`ldr r0,[r7]; ldr r1,[r0]`) and again after the shared `strh` (`ldr r5,[r7]`, which then feeds the post-loop switch); the draft hoists it before the loop as a NEW literal (`ldr r5,=gUnknown_030044E0`, the extra pool word), uses r5 in both arms, and reloads via r7 after the loop.

### Why it is close

Four named constructs closed this wave, each confirmed by compile_probe before the verdict: (a) the `cmp r6,#64` hunk is the W80-C fold rule -- `unk65 + 1 + table[t]` folds to `unk65 + (table[t] + 1)`, whose expansion forms the member address first, then the table load plus one, then the deferred member load: the ROM order exactly (the W77 header had read the emitted order back into the source); (b) the SECOND `?:` of each increment/decrement pair is an if/else STATEMENT in the ROM (`bne L; movs #K; b; L: subs` is what `if (x == 0) x = K; else x = x - 1;` emits; both `?:` polarities emit `beq; subs; b; movs`), while the first (`x > N ? 0 : x + 1`) is a genuine `?:` (the four-store if/else form puts `movs #0` first); (c) the `unk66` mask survives only as a multi-set local (`c = x + 1; c &= 0xff; c |= 0x80; x = c;`), every single-set spelling letting nonzero_bits drop it and re-canonicalise `| 0x80` as `| -0x80`. Hunk (3)'s mechanism: PRE merges the two arms' word loads into one pseudo above the key test, the movable then has savings >= 2 and clears move_movables' threshold, and it is re-emitted from its REG_EQUAL constant as a literal; the ROM's loads stayed separate. Untested: the source that keeps the two arms' loads distinct -- probes with the draft `?:` and with four stores both hoist in a reduced function.

### Already ruled out

- Wave 55/66/77: see the draft header (goto-shaped outer loop settled; palette if-chain settled; int/u16/u8 `c` and a separate palette local identical; the -fforce-addr words are correct and carried by the promotion).
- Wave 76 (W76-C): twenty diff files in work/sub_0804A760/ -- barrier, fixed r8/r6 pins, fixed vars, int-separate, layout, pool symbol, reorder, s8, state pointer (key/vol), switch guard, u-int, u-r4, volatile view.
- Wave 79 (orchestrator): all seven compiler profiles, zero matches.
- WAVE 80 (W80-D): removing the four `register asm` pins with fixes (a)-(c) in place: 924/920, 12.4%. A statement split for the `cmp r6,#64` hunk is unnecessary -- the fold rule closes it without one.

### Settled

- `gUnknown_030044E0->unk65 + 1 + gUnknown_084C36E4[t]` -- the member first; fold's associate: rule makes the emitted order the opposite of the written one.
- `if (x == 0) x = K; else x = x - 1;` for the unk1e and unk20 decrements; `x > N ? 0 : x + 1` for the increments.
- `c = unk66 + 1; c &= 0xff; c |= 0x80; unk66 = c;` -- the mask is real and needs a multi-set pseudo to survive.
- The ROM's `adds r0,r6,r0` proves t is an ordinary pseudo, not a pinned register.

### Why it is parked

Two residuals left, both allocation-adjacent: the pins stand in for an unknown construct that gives flag r8 / t r6 / u r4 / c r3 naturally, and the do/while word-load hoist is a PRE + LICM decision whose source-side discriminator was not found in three probes.

### Wave 86

WAVE 86 (W86-D, constant-twin axis): the screen's twin c_0804A260.c IS already cited in the draft (its struct view is imported verbatim), so the axis supplies nothing new; the one untried inversion -- the bare `*(s16 *)&p->m` alias that kills PRE's hoisted address pseudo, read backwards -- was probed and REGRESSED 924 -> 930 (+10 vs baseline +4), restored. Mechanism CONFIRMED (the alias really does control reload-vs-hoist), transplant REFUTED: the lever is all-or-nothing at function scope and the ROM has NO hoist at all (reloads in each do-while arm and once after the shared strh into r5, which the post-loop switch arms reuse) -- the draft's single hoisted pool word is one decision, not two, and the alias overshoots it by four reloads. Open question, unmeasured: a spelling that scopes the PRE barrier to the loop only. Configured, 924/920, unchanged.

### Wave 96

Base: the wave-80 draft with the register pins (`sub_0804A760.w96-start.c`, 59.09%, size +4, first diff +0xae). Now `sub_0804A760.c` = `sub_0804A760.w96-gp2.c`: **920/920, 73 of 920 bytes differ, 92.1%, first diff +0xd8.**

What moved it (59.1% -> 92.1%, size +4 -> exact): the ROM reloads the pointer global through the compiler's `.rodata` cell IN EACH ARM of the key `do { } while` loop (`ldr r0,[r7]; ldr r1,[r0]`), while the draft hoisted the load out of the loop (a fresh `ldr r5,=gUnknown_030044E0` literal, the old +4). Binding the cell's address to a local, `struct Unk030044E0 **gp = &gUnknown_030044E0;`, and RE-ASSIGNING it (`gp = &gUnknown_030044E0;`) at the top of each of the increment and decrement arms, then reading the increment/decrement arms through `(*gp)`, keeps the loop from hoisting the chase (loop.c sees a different pseudo set in every arm). Binding once before the loop is the wrong half: it still hoists (57.3%, size -4, and `gp` declared at the top of the function moves the prologue).

Mechanism: this is the pre-registered early-cell / late-pool split from the link-cable and force-addr blocks seen from the LOOP side. The cell load is a loop-invariant memory read; move_movables hoists it when one pseudo carries it across several arms (savings >= 2). A separate address bind per arm gives each arm its own single-use pseudo (savings 1, below the threshold for a loop that contains a call), which is the ROM's per-arm reload.

Negatives:
- Testing `unk1e == 0` through `(*gp)` in the else arm (either as `else if` or as a nested if after `gp = ...`) brings the hoist BACK (+4, 67.2%): the test and the decrement then share one pseudo inside the arm. A second pointer `gq` for the else arm is no better (59.1%).
- The current draft tests `gUnknown_030044E0->unk1e == 0` with the struct's u8 member (the ROM tests the s16 view with `ldrsh` after a shared `ldrh`). This is a semantic no-op for the range 0..14 but is NOT the original's read; it is the price of avoiding the hoist so far. Open.

Left (diff against the ROM, 73 bytes):
1. the decrement arm test above (0x24a..), plus the after-loop `ldr r5` copy (`adds r4,r5,#0` in ours, ROM keeps r5) which follows from it.
2. `adds r0,r6,r0` (ROM) vs `adds r6,r6,r0` (ours) at 0x112: the pinned `t` (asm("r6")). The unpinned draft is worse; the pin is a costume.
3. the `unk66` hunk at 0x158: ROM `movs r4,#0` placement and r0/r3 assignment for `c` (pin on r3).
4. `movs r0,r0` vs `movs r4,r0` at 0xd8.
Permuter: see below.

Proposed summary:
- does: on-screen keyboard input handler; moves the cursor with the d-pad, wraps rows and columns, jumps to a key on shoulder buttons and plays the click sound
- status: 92% at exact size; first difference at +0xd8
- left: the decrement arm compares the cursor column as a signed halfword after a shared load; the register pins on t and c hide two allocation choices
- tried: per-arm re-bind of the address cell in the key loop (the lever), once-only bind (hoists), bind at function top (moves the prologue), second pointer for the else arm

### Wave 97

wave 97 (W97-Q)
Base: `sub_0804A760.w97-start.c` (= w96-gp2, 92.07%, size exact, first diff +0xd8). `sub_0804A760.c` is restored to it (score-best); the better-shaped candidate is `w97-q.c` (67.97%, size +4, but only ~4 sites differ; the score is low only because +4 shifts every later byte).

Moved: the decrement-arm test. Writing the else arm as its own block with an if/else STATEMENT read entirely through `(*gp)` (`gp = &gUnknown_030044E0; if (V(*gp)->unk1e == 0) V(*gp)->unk1e = 0xe; else V(*gp)->unk1e = V(*gp)->unk1e - 1;`, w97-a.c) reproduces the ROM arm byte for byte (`ldrh r2` / `ldrsh` / `bne` / `movs #14` / `subs r0,r2,#1`), no hoist. The w96 note "test through (*gp) brings the hoist back" is not what the +4 is: the +4 is the post-loop copy `adds r4,r5,#0` (below) plus 2 bytes of pool padding. The ternary spelling (w97-b) gets the arm order wrong (beq/subs first), as wave 80 said.

Second: the unk66 arm used the pinned `c` (r3). Give it its own local (`k`: `k = unk66 + 1; k &= 0xff; k |= 0x80;`, w97-q.c) and the arm matches the ROM except the zero constant sits in r1 where the ROM has r4.

Left in w97-q.c (all else identical): (1) `adds r6,r6,r0` vs ROM `adds r0,r6,r0` (t pin); (2) zero for unk67 in r1 vs ROM r4; (3) after the key loop the ROM keeps the reloaded cell value in r5 for the whole post-loop switch, ours makes a second pseudo `adds r4,r5,#0` for the three cases 0x40/0x24/0x25 (case 0x23 keeps r5): this is the +4. Removing the pin on u, c or t individually does not remove the copy (w97-m,o,r). Cases stored through `(*gp)` (w97-k, frame +4), gp re-assigned after the loop (w97-l, 62%), while-cond through gp (w97-w/x/y) all worse.
Unpinning c alone gives size-exact 920 at 68.8% (w97-t): the tail then differs (`lsls #5` vs `lsls #21; lsrs #16`), so c's pin is needed for the tail.

Proposed summary:
- does: on-screen keyboard input handler (d-pad cursor, row/column wrap, jump keys, click sound)
- status: 92% at exact size in the committed draft; a shape-correct variant (w97-q.c) has only three residual sites but is 4 bytes long
- left: after the key loop the ROM reuses one register for the cell value in all four switch cases; ours copies it for three; also t pin and a zero constant register
- tried: decrement arm as if/else through the pointer (fixes the arm), separate local for the unk66 arm (fixes that hunk), unpinning u/c/t, routing later reads through the pointer local

</details>
