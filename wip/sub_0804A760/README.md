# sub_0804A760

0x0804A760, 920 bytes, THUMB, parked.

Best score so far: 59.9%, +4 bytes (best.c).

## What it does

The per-frame input handler for an on-screen character grid, apparently a text-entry keyboard. A types the character under the cursor or acts on a special key, B deletes, Start calls sub_0804A6D8, and the d-pad moves the cursor with wrap-around.

## How close it is

Compiles 4 bytes too long (924 against 920); 374 bytes differ (59.3% line up). The draft pins four variables to fixed registers, which is not real source but scores far better than without the pins (12.4%).

## What is left

Two things remain: the construct that naturally gives flag, t, u and c the original's registers (the pins stand in for it), and the key-repeat do/while loop, where the original re-reads the gUnknown_030044E0 pointer in each branch but our compiler merges the loads into one copy before the loop, costing an extra literal-pool word (the 4 extra bytes). A spelling that blocks that merge in the loop only has not been tested.

## Already tried

- Removing the four register pins (with the other fixes in place): 924 bytes, 12.4%.
- Reading the pointer through a pointer alias to stop the merge: it stops it everywhere in the function, 10 bytes too long.
- About twenty variants (barriers, other pins, volatile views, reordering, signed and unsigned types, switch guards): none closer.
- All seven compiler profiles: no match.

## Files

- `sub_0804A760.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
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

</details>
