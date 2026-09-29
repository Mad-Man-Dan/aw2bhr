# sub_0804FA2C

0x0804FA2C, 632 bytes, THUMB, parked.

Best score so far: 75.2% (best.c).

## What it does

Sets up the current sprite object (gUnknown_03001FBC) for one side and slot: its flip, palette, tile number and priority. It then places the sprite at the slot's starting position from the table sub_08057D44 returns.

## How close it is

Now compiles to the right size (632 bytes) where it used to be 4 too long, with 75.2% of bytes in place. The remaining difference starts 0x80 bytes in: the original keeps the address of gUnknown_03004580 in a spare register across the tile-number calculation, and this build reloads it, paying two register copies at the multiply.

## What is left

Find what makes the compiler load gUnknown_03004580's address one step later, into the register the original uses. Two matched sibling functions (sub_0804D290, sub_0804DCA8) had the same problem and were fixed with a comma expression, but here that fix costs gUnknown_0300453C its register because one more value is live.

## Already tried

- The siblings' exact fix (a comma expression naming gUnknown_03004580 inside sub_08057D44's first argument, plus a `do { } while (0)` around that call): 20 bytes too long, 16.6% identical.
- The comma expression alone, placed where the original loads the address or in the call: same size, 20 points worse.
- `do { } while (0)` around the tile-number statement, the call, the last statement or the four position writes: 8 to 20 bytes too long.
- Changing which variable the comma expression names (pos, row, an int, a register local): no effect; only its position matters.
- A pointer local for the gUnknown_03004580 reads: loses the shared base the original reuses across the four position writes.
- The older compiler build: worse. Turning off force-addr: identical output.
- Two permuter runs of over 300 seconds: only meaningless edits, both worse than the draft.

## Files

- `sub_0804FA2C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

75.16% -- 632 bytes, SIZE-EXACT (was 45.91% at +4 at wave 93 start)

### What still differs

&gUnknown_03004580 is materialised early and register-allocated, but into sl and ONE INSTRUCTION BEFORE `ldr r5, =gUnknown_085D6A48`, where the ROM has it in r8 one instruction AFTER -- so the last two literal-pool words are swapped and the tile multiply pays two extra copies (`adds r3,r1,#0; muls r3,r0; adds r0,r3,#0` against the ROM's bare `muls r0, r1`). The multiply is downstream of the allocation, not independent: both operand orders and a temporary for the table read give byte-identical output.

### Why it is close

This was the same failure mode that sub_0804D290 and sub_0804DCA8 had, and both of those are now MATCHED -- but WAVE 79 (W79-F) RE-TESTED THE TWINS' FIX IN ITS EXACT PROMOTED FORM AND IT IS WORSE HERE, so that lead is now closed rather than open. Of two address constants used the same number of times in the same statements, agbcc gives the callee-saved register to whichever pseudo is created first; the fix on the twins was a comma operator inside argument 1's subscript, INSIDE a `do { } while (0)` round the sub_08057D44 statement -- the two together, which is what c_0804DCA8.c actually does and which this function's earlier notes had only ever tried separately. Measured: the combination is 652 bytes (+20), 16.6%; the comma anchor alone moved into the tileNum statement's subscript, between the gUnknown_085D6A48 reference and the gUnknown_03004582 one where the ROM's `mov r8, r3` actually sits, is 636 bytes (+4) at 26.7% -- same size, 20 points worse. FA2C carries one more simultaneously live value than the twins, and the lever that buys &gUnknown_03004580 its register there costs &gUnknown_0300453C its r6 here. This is an allocno tie under a different live set, not a source construct. See 'Of two address constants, the pseudo created FIRST wins the register' in docs/agbcc-codegen.md.

### Already ruled out

- old_agbcc -- 624 bytes on the twins, worse here too
- -fno-force-addr -- BIT-IDENTICAL output, so these `ldr rN, =sym` are ordinary address loads and not -fforce-addr artefacts
- lc_screen reports 0 pool words, so this failure mode is invisible to it
- do { } while (0) round the tileNum statement, the sub_08057D44 statement, the sub_08015928 statement, or the four entry writes -- +8 to +20
- the comma anchor in either the tileNum or the sub_08057D44 statement -- same +4 size, worse bytes
- anchoring through pos, row, an int, or a register-qualified local -- all bit-identical, so the anchor's variable is irrelevant and only its position matters
- a u16 (*)[8] pointer local for the gUnknown_03004580 reads -- collapses the base-plus-constant form the ROM CSEs into an ldrh displacement
- two decomp-permuter runs of 300 s+ (3000 and 12000 iterations) -- best edits were noise (`new_var = 5` used as an array index) and both scored worse than the hand fix
- WAVE 79 (W79-F): the twins' COMBINATION -- do { } while (0) round the sub_08057D44 statement WITH (meta = gUnknown_03004580, ...) inside argument 1's subscript, exactly as c_0804DCA8.c writes it -- 652 bytes (+20), 16.6%
- WAVE 79 (W79-F): the comma anchor inside the tileNum statement's SUBSCRIPT (between the 085D6A48 reference and the 03004582 one, which is where the ROM's `mov r8, r3` sits) -- 636 bytes (+4), 26.7%, i.e. same size and 20 points worse than the plain draft

### Settled

- gUnknown_085D6A48's rows are STRUCTS, not u16[12] -- the ROM keeps column 9 in the ldrh displacement, which an array row cannot emit (4 bytes)
- (rN = pos[k])[i*5+j] and not pos[k][i*5+j] -- the ROM builds the row address first (12 bytes)
- ONE binding local PER STATEMENT: a single `row` reused across four statements is one pseudo spanning all of them and takes a low callee-saved register the ROM spends on the constant 180 (4 bytes)
- * 0x100 and not << 8 (4 bytes)

### Why it is parked

Register allocation with NO source construct behind it. The instruction stream, the type model and every read form are settled; what remains is one address constant landing one slot early in a function that is one live value tighter than its two matched twins, and wave 79 measured that the twins' own lever does not transfer in either of its two forms. Wave-77 class: register numbers with nothing behind them.

### Wave 93

- **result:** 45.91% at +4 bytes -> 75.16% SIZE-EXACT, first difference +0xc -> +0x80
- **base_adopted:** best.c/recovered.c (72.94%), audited as equivalent C and adopted. Its changes: a local holding &gUnknown_03004580 read twice later (renamed new_var -> sideData), a dead comma anchor holding the same address inside the tileNum subscript (renamed meta -> sideDataAnchor), and the draft's four write-only locals r1..r4 collapsed into one row assigned four times. An array's address is a constant, so binding it cannot change what the later reads load, and all five row spellings are dead stores.
- **permuter:** 900 s x 4 threads from the 72.94% base: 72.94% -> 75.16%. One mutation, audited: a do { } while (0); around the first eleven statements, from oam.hFlip through the .unk06 store, leaving the final .y store outside. No break or continue in the body and nothing reordered.
- **negatives_corrected:** The entry's claim that best.c held an unrelated variant is wrong -- it is this draft plus two address binds and the collapsed row local, and it is worth the +4 the function carried for four waves. It also re-opens the twins' lever: the wave-79 measurements that judged the sub_0804D290 / sub_0804DCA8 comma-anchor fix WORSE here (652 bytes at 16.6%, and 636 at 26.7%) were all taken with the four separate r1..r4 locals still in place. With them collapsed to one, an anchor of the same kind is worth 27 points and the exact size.
- **anchor_position_swept:** Against the new residual, four positions for the comma anchor: inside the subscript (the base) 75.16% first +0x80; inside the cast's operand 75.16% and byte-identical; as its own statement before oam.tileNum 75.16% but first difference EARLIER at +0x7e; in the first operand of the + 36.23% and size -4. The base's position is the optimum of the four.
- **residual:** 632/632, 157 of 632 bytes differ, first difference +0x80. The ROM loads three pool words before the index shift and parks the third in r8 (ldr r3,=B then mov r8,r3); the candidate loads two and pays adds r3,r1,#0 / muls r3,r0 / adds r0,r3,#0 where the ROM has a bare muls r0, r1. The copies are downstream of the allocation, not an operand-order choice.

### Wave 96

Base: sub_0804FA2C.c (75.16%, size-exact, first diff +0x80). Draft is now `sub_0804FA2C.w96-single.c` = the old draft with the two address binds merged into ONE variable (`sideData`, assigned inside the tileNum subscript by the comma anchor; the later `sideData = gUnknown_03004580;` statement is gone) and the sub_08057D44 second argument spelled `*(u16 *)((u8 *)sideData + 6 + side * 16)`. Score unchanged (75.16%): both are byte-neutral. Old draft kept as `.w96-start.c`.

Reading of the ROM (target.s): ONE pseudo holds &gUnknown_03004580 (`ldr r3,=word; mov r8,r3`, placed right after the 085D6A48 word and BEFORE the row shift), and is used with immediates: `mov r1,r8; adds r1,#6` (sub_08057D44 argument, [side][3]) and `movs r2,#0xa; add r2,r8; mov sb,r2` (the [5] read, kept in sb and reused by the four position stores). So the ROM spells both reads as base + constant + side*16, i.e. the same "+N first" form as sub_080506B0. The pool word order confirms it: the ROM's 03004580 word sits before 03004582; the draft's sits after 0300450C.

Negatives:
- Converting the four `[side][5]` reads to `(u8 *)sideData + 10 + side*16`: 628 bytes (-4), 41.8%, first diff +0xc. Converting only the first, or only the fourth: +4 / +24 bytes, first diff +0xc. Any change to the count of by-name references to gUnknown_03004580 moves the start of the function (the pool word order shifts), so the [5] form cannot be tried without also getting the early pseudo.
- Merged single variable: byte-neutral. cse propagates the constant address into the later uses (a pseudo set once to a symbol_ref is replaced by the symbol), so the held register the ROM has (r8 across the call) is not created. The comma anchor alone creates the early pool word in the old draft only because the write is dead there and survives as a separate pseudo.
Not run: the permuter (75% base, prior 900s x4 run in wave 93 found only the do/while).

Proposed summary:
- does: sets up the cursor sprite for a side and slot, positions it from the position table and starts its effect
- status: 75% at exact size; first difference at +0x80
- left: the ROM holds &gUnknown_03004580 in one register from before the tile-number multiply and adds +6 / +10 to it for two later reads; the draft rebuilds the address at each read
- tried: comma anchor (kept), do/while wrap (kept), single merged variable, +6/+10 byte-offset spellings of the later reads (see NOTES)

</details>
